#include <cstdio>

template<typename T> struct Logger {
  virtual ~Logger() = default;
  virtual void log_transfer(T from, T to, double amount) = 0;
};

template<typename T> struct ConsoleLogger : Logger<T> {
  void log_transfer(T from, T to, double amount) override {
    printf("[cons] %ld -> %ld: %f\n", from, to, amount);
  }
};

template<typename T> struct FileLogger : Logger<T> {
  void log_transfer(T from, T to, double amount) override {
    printf("[file] %ld,%ld,%f\n", from, to, amount);
  }
};

template<typename T> struct Bank {
  void set_logger(Logger<T>* new_logger) {
    logger = new_logger;
  }
  void make_transfer(T from, T to, double amount) {
    if(logger)
      logger->log_transfer(from, to, amount);
  }

  private:
  Logger<T>* logger{};
};

int main() {
  ConsoleLogger<long> console_logger;
  FileLogger<long> file_logger;
  Bank<long> bank;
  bank.set_logger(&console_logger);
  bank.make_transfer(1000, 2000, 49.95);
  bank.set_logger(&file_logger);
  bank.make_transfer(2000, 4000, 20.00);
}
