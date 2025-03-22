//#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

http::response<http::dynamic_body> make_request(net::io_context& io_context, std::string_view host, std::string_view port, std::string_view target, const int version = 11) {
  tcp::resolver resolver(io_context);
  beast::tcp_stream stream(io_context);
  const auto endpoints = resolver.resolve(host, port);
  stream.connect(endpoints);

  http::request<http::string_body> request{http::verb::get, target.data(), version};
  request.set(http::field::host, host.data());
  request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  http::write(stream, request);

  beast::flat_buffer buffer;
  http::response<http::dynamic_body> response;
  http::read(stream, buffer, response);

  beast::error_code ec;
  stream.socket().shutdown(tcp::socket::shutdown_both, ec);
  if(ec && ec != beast::errc::not_connected)
    throw beast::system_error{ec};

  return response;
}

int main() {
  net::io_context io_context;
  std::string host{"example.com"};
  std::string port{"80"};
  char target[]{'/'};

  try {
    const auto response = make_request(io_context, host, port, target); 
    std::cout << response << "\n";
  } 
  catch(const std::exception& se) {
    std::cerr << "Error: " << se.what() << std::endl;
  }
}