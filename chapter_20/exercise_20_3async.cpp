// #include <algorithm>
// #include <boost/algorithm/string/predicate.hpp>
// #include <boost/asio.hpp>
// #include <iostream>
// #include <sstream>
// #include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

using ResolveResult = tcp::resolver::results_type;
using Endpoint = tcp::endpoint;

// Report a failure
void
fail(beast::error_code ec, char const* what) {
  std::cerr << what << ": " << ec.message() << "\n";
}

class Request: public std::enable_shared_from_this<Request> {
public:
  explicit Request(net::io_context& ioc)
  : resolver_(net::make_strand(ioc)), 
    stream_(net::make_strand(ioc))
  {}
  
  void run(std::string_view host, std::string_view port, std::string_view target, const int version = 11) {
    // Set up an HTTP GET request message
    request_.version(version);
    request_.method(http::verb::get);
    request_.target(target.data());
    request_.set(http::field::host, host.data());
    request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Look up the domain name
    resolver_.async_resolve(
      host.data(), 
      port.data(),
      beast::bind_front_handler(
        &Request::onResolve, 
        shared_from_this()
      )
    );
  }

  void onResolve(beast::error_code ec, tcp::resolver::results_type results) {
    if(ec)
      return fail(ec, "resolve");

    stream_.expires_after(std::chrono::seconds(30));

    stream_.async_connect(
      results,
      beast::bind_front_handler(
        &Request::onConnect,
        shared_from_this()
      )
    );
  }

  void onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
    if(ec)
      return fail(ec, "connect");

    stream_.expires_after(std::chrono::seconds(30));

    http::async_write(
      stream_, 
      request_,
      beast::bind_front_handler(
        &Request::onWrite,
        shared_from_this()
      )
    );
  }

  void onWrite(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if(ec)
      return fail(ec, "write");

    http::async_read(
      stream_, 
      buffer_, 
      response_,
      beast::bind_front_handler(
        &Request::onRead,
        shared_from_this()
      )
    );
  }

  void onRead(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if(ec)
      return fail(ec, "read");

    std::cout << response_ << std::endl;

    stream_.socket().shutdown(tcp::socket::shutdown_both, ec);

    if(ec && ec != beast::errc::not_connected)
      return fail(ec, "shutdown");
  }

  const http::response<http::string_body>& getResponse() const noexcept {
    return response_;
  }

  private:
    tcp::resolver resolver_;
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_; // (Must persist between reads)
    http::request<http::empty_body> request_;
    http::response<http::string_body> response_;
};

int main() {
  net::io_context io_context;
  std::string host{"example.com"};
  std::string port{"80"};
  char target[]{'/'};

  try {
    std::shared_ptr<Request> request = std::make_shared<Request>(io_context);
    request->run(host, port, target);
    io_context.run();
  }
  catch(const std::exception& se) {
    std::cerr << "Error: " << se.what() << std::endl;
  }
}
