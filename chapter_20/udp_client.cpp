
#include <array>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    std::stringstream request_stream;
    request_stream << "GET / HTTP/1.1\r\n"
                    "Host: "
                 << "http://example.com"
                 << "\r\n"
                    "Accept: text/html\r\n"
                    "Accept-Language: en-us\r\n"
                    "Accept-Encoding: identity\r\n"
                    "Connection: close\r\n\r\n";
    const auto request = request_stream.str();

    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint =
      *(resolver.resolve(udp::v4(), argv[1], "daytime").begin());
    std::cout << receiver_endpoint << std::endl;

    udp::socket socket(io_context);
    socket.open(udp::v4());

    std::array<char, 1> send_buf  = {{ 0 }};
    socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
    for (auto item : send_buf) {
      std::cout << item; 
    }
    std::cout << std::endl;

    std::array<char, 128> recv_buf;
    udp::endpoint sender_endpoint;
    size_t len = socket.receive_from(
        boost::asio::buffer(recv_buf), sender_endpoint);
    for (auto item : recv_buf) {
      std::cout << item;
    } 
    std::cout << std::endl;

    std::cout.write(recv_buf.data(), len);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}