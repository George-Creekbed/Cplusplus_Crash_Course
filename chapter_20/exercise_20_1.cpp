#include <array>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
//#include <boost/bind/bind.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string() {
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class udp_server {
public:
  udp_server(boost::asio::io_context& io_context)
    : socket_(io_context, udp::endpoint(udp::v4(), 8413))
  {
    start_receive();
  }

private:
  void start_receive() {
    socket_.async_receive_from(
      boost::asio::buffer(recv_buffer_), 
      remote_endpoint_,
      std::bind(
        &udp_server::handle_receive, 
        this,
        std::placeholders::_1,
        std::placeholders::_2
      )
    );
  }

  void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
      std::shared_ptr<std::string> message(
        new std::string(make_daytime_string())
      );

      socket_.async_send_to(
        boost::asio::buffer(*message), 
        remote_endpoint_,
        std::bind(
          &udp_server::handle_send, 
          this,
          message,
          std::placeholders::_1,
          std::placeholders::_2)
        );

      start_receive();
    }
  }

  void handle_send(std::shared_ptr<std::string> message,
      const boost::system::error_code& ec,
      std::size_t bytes_transferred)
  {
    //*message = make_daytime_string();
  }

  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  std::array<char, 128> recv_buffer_;
};

int main()
{
  try
  {
    boost::asio::io_context io_context;
    udp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}