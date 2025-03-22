// example usage on command prompt: ./exercise_21_3 example.com / /index.html

#include <algorithm>
#include <boost/asio.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <boost/program_options.hpp>
#include <future>
#include <filesystem>
#include <regex>

using ResolveResult = boost::asio::ip::tcp::resolver::results_type;
using Endpoint = boost::asio::ip::tcp::endpoint;

struct Request {
  explicit Request(boost::asio::io_context& io_context, std::string host, std::string resource)
      : resolver{ io_context }
      , socket{ io_context }
      , host_{ host }
      , resource_{resource} {
    std::stringstream request_stream;
    request_stream << "GET "
                   << resource_
                   << " HTTP/1.1\r\n"
                      "Host: "
                   << host_
                   << "\r\n"
                      "Accept: text/plain\r\n"
                      "Accept-Language: en-us\r\n"
                      "Accept-Encoding: identity\r\n"
                      "Connection: close\r\n\r\n";
    request = request_stream.str();
  }

  void resolve_request() {
    resolver.async_resolve(
      host_, 
      "http", 
      [this](boost::system::error_code ec, const ResolveResult& results) {
        resolution_handler(ec, results);
      }
    );
  }

  void resolution_handler(boost::system::error_code ec, const ResolveResult& results) {
    if(ec) {
      std::cerr << "Error resolving " << host_ << ": " << ec << std::endl;
      return;
    }
    boost::asio::async_connect(
      socket, 
      results, 
      [this](boost::system::error_code ec, const Endpoint& endpoint) {
        connection_handler(ec, endpoint);
      }
    );
  }

  void connection_handler(boost::system::error_code ec, const Endpoint& endpoint) {
    if(ec) {
      std::cerr << "Error connecting to " << host_ << ": " << ec.message() << std::endl;
      return;
    }
    boost::asio::async_write(
      socket, 
      boost::asio::buffer(request), 
      [this](boost::system::error_code ec, size_t transferred) {
        write_handler(ec, transferred);
      }
    );
  }

  void write_handler(boost::system::error_code ec, size_t transferred) {
    if(ec) {
      std::cerr << "Error writing to " << host_ << ": " << ec.message() << std::endl;
    } 
    else if(request.size() != transferred) {
      request.erase(0, transferred);
      boost::asio::async_write(
        socket, 
        boost::asio::buffer(request), 
        [this](boost::system::error_code ec, size_t transferred) {
          write_handler(ec, transferred);
        }
      );
    } else {
      boost::asio::async_read(
        socket, boost::asio::dynamic_buffer(response), [this](boost::system::error_code ec, size_t transferred) {
          read_handler(ec, transferred);
        });
    }
  }

  void read_handler(boost::system::error_code ec, size_t transferred) {
    if(ec && ec.value() != 2)
      std::cerr << "Error reading from " << host_ << ": " << ec.message() << std::endl;
  }

  const std::string& get_response() const noexcept {
    return response;
  }

  const std::string& get_request() const noexcept {
    return request;
  }

private:
  boost::asio::ip::tcp::resolver resolver;
  boost::asio::ip::tcp::socket socket;
  std::string request, response;
  const std::string host_, resource_;
};

bool is_help{false};

boost::program_options::variables_map process_options(int argc, char** argv) {
  boost::program_options::options_description description{ 
    "./exercise_21_3 [options] /storage/dir(=current_path) host resource1 resource2 ..." 
  };

  // create storage dir for http responses
  std::filesystem::path storage_dir{std::filesystem::current_path() / "http_responses/"};
  if (!std::filesystem::is_directory(storage_dir))
    std::filesystem::create_directory(storage_dir);
  
  description.add_options()
    ("help,h", boost::program_options::bool_switch(&is_help), "display a help dialog")
    ("threads,t", boost::program_options::value<int>()->default_value(4), "number of threads to use")
    ("dir", boost::program_options::value<std::string>()->default_value(storage_dir), "directory storing http responses")
    ("host", boost::program_options::value<std::string>(), "host to make request to")
    ("resource", boost::program_options::value<std::vector<std::string>>(), "resource (path inside host) queried");

  boost::program_options::positional_options_description positional;
  //positional.add("dir", 1);
  positional.add("host", 1);
  positional.add("resource", -1);

  boost::program_options::command_line_parser parser{argc, argv};
  parser.options(description);
  parser.positional(positional);

  boost::program_options::variables_map vm;
  try {
    auto parsed_result = parser.run();
    store(parsed_result, vm);
    notify(vm);
  } catch(std::exception& e) {
    std::cerr << e.what() << "\n";
    std::exit(-1);
  }

  if(is_help) {
    std::cout << description;
    std::exit(0);
  }
  // if(vm["dir"].empty()) {
  //   throw std::runtime_error("You must provide a dir.\n");
  // }
  if(vm["host"].empty()) {
    throw std::runtime_error("You must provide a host.\n");
  }
  if(vm["resource"].empty()) {
    throw std::runtime_error("You must provide at least one resource.\n");
  }

  return vm;
}


int main(int argc, char** argv) 
{ 
  try {
    // read input from user
    boost::program_options::variables_map vm = process_options(argc, argv);
    
    const auto threads = vm["threads"].as<int>();
    const auto string_dir = vm["dir"].as<std::string>();
    const auto host = vm["host"].as<std::string>();
    auto resources = vm["resource"].as<std::vector<std::string>>();
    
    // check dir input
    const std::filesystem::path dir{string_dir};
    if (!is_directory(dir))
      throw std::runtime_error("dir must be a directory\n");

    // start process
    boost::asio::io_context io_context{threads};
    
    // store requests inside vector
    size_t n_requests{resources.size()};
    std::vector<Request> requests;
    requests.reserve(n_requests);
    std::transform(
      resources.begin(), 
      resources.end(), 
      std::back_inserter(requests), 
      [&io_context, host](const std::string& resource){
        return Request{ io_context, host, resource};
      }
    );
    // make requests asynchronously
    for(size_t i=0; i!=n_requests; ++i) {
      requests[i].resolve_request();
    }
    
    // manage async requests with futures
    std::vector<std::future<void>> futures;
    futures.reserve(n_requests);
    std::generate_n(
      std::back_inserter(futures), 
      n_requests, 
      [&io_context] {
        return std::async(
          std::launch::async, 
          [&io_context] { 
            io_context.run(); 
          }
        );
      }
    );

    for(auto& future : futures)
      future.get();

    // write http-responses to requests on file
    for (auto i=0; i!=n_requests; ++i) {

      std::string text_file{dir.string() + '/' + host};
      // clean resource name of symbols
      std::regex regex{ "[/.:@#?!$%&]" };
      resources[i] = std::regex_replace(resources[i], regex, "_");
      text_file += '_' + resources[i] + ".txt";
      std::cout << "debug: " << text_file << '\n';

      std::ofstream file{text_file, std::ofstream::out | std::ofstream::trunc};
      if (file.is_open()) {
        file << requests[i].get_response() << '\n';
        file.close();
      } else
        throw std::runtime_error("Could not open receiving file");
    }
  }   
  catch (std::exception& e) {
    std::cerr << e.what() << "\n";
  }
}
