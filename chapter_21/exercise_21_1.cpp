#include <boost/algorithm/string/case_conv.hpp> // Listing 14-31
#include <boost/asio.hpp>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory> // Chapter 11
#include <string>
#include <atomic>
#include <csignal>
#include <cstdlib>

using namespace boost::asio;

std::atomic<bool> interrupted{};

struct Session : std::enable_shared_from_this<Session> {
  explicit Session(ip::tcp::socket socket)
    : socket{ std::move(socket) } {}
      
  void read() {
    if (interrupted) {
      std::exit(SIGINT);
    } else {
      async_read_until(
        socket,
        dynamic_buffer(message),
        '\n',
        [self = shared_from_this()](boost::system::error_code ec, std::size_t length) {
          if (ec || self->message == "\n")
            return;
          boost::algorithm::to_upper(self->message);
          self->write();
        }
      );
    }
  }

  void write() {
    if (interrupted) {
      std::exit(SIGINT);
    } else {
      async_write(
        socket, 
        buffer(message), 
        [self = shared_from_this()](boost::system::error_code ec, std::size_t length) {
          if(ec)
            return;
          self->message.clear();
          self->read();
        }
      );
    }
  }

private:
  ip::tcp::socket socket;
  std::string message;
};

void serve(ip::tcp::acceptor& acceptor) {
  std::signal(SIGINT, [](int signal){interrupted = true;});

  if (interrupted)
    std::exit(SIGINT);
  else {
    acceptor.async_accept(
      [&acceptor](boost::system::error_code ec, ip::tcp::socket socket) {
        serve(acceptor);
        if(ec)
          return;
        auto session = std::make_shared<Session>(std::move(socket));       
        session->read();
      }
    );
  }
}

int main() {
  try {
    io_context io_context;
    ip::tcp::acceptor acceptor{ io_context, ip::tcp::endpoint(ip::tcp::v4(), 8123) };

    std::atexit([](){std::cout << "\nProgram terminated by keyboard interrupt" << std::endl;});

    serve(acceptor);
    io_context.run();
  } 
  catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
