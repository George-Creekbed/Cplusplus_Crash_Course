#include <cstdio>

class AccountDatabase {
public:
    virtual ~AccountDatabase() = default;
    virtual const long getId() const = 0;
    virtual void setId(const long) = 0;
    virtual const double getAmount() const = 0;
    virtual void setAmount(const double) = 0;
};

class InMemoryAccountDatabase: AccountDatabase {
public:
    InMemoryAccountDatabase(long id, double amount): 
    _id{id}, _amount{amount} 
    {}

    const long getId() const override {
        return _id;
    }

    void setId(const long id) override {
        _id = id;
    }

    const double getAmount() const override {
        return _amount;
    }

    void setAmount(const double amount) override {
        _amount = amount;
    }

private:
    long _id;
    double _amount;
};

struct Logger {
  virtual ~Logger() = default;
  virtual void log_transfer(long from, long to, double amount) = 0;
};

struct ConsoleLogger : Logger {
  ConsoleLogger(const char* intro_string): 
  _prepend{intro_string} 
  {} 

  void log_transfer(long from, long to, double amount) override {
    printf("%s - [cons] %ld -> %ld: %f\n", _prepend, from, to, amount);
  }

  const char* _prepend;
};

struct FileLogger : Logger {
  void log_transfer(long from, long to, double amount) override {
    printf("[file] %ld,%ld,%f\n", from, to, amount);
  }
};

struct Bank {
  Bank(Logger* logger, InMemoryAccountDatabase& database): logger{logger}, _database{database} {}

  void set_logger(Logger* new_logger) {
    logger = new_logger;
  }

  InMemoryAccountDatabase& getDatabase() const {
    return _database;
  }

  void make_transfer(long from, long to, double amount) {
    if(logger)
      logger->log_transfer(from, to, amount);
    _database.setId(to);
    _database.setAmount(amount);
  }

  private:
  Logger* logger{};
  InMemoryAccountDatabase& _database;
};

int main() {
    InMemoryAccountDatabase clients(1000L, 30.56);
    printf("Client %ld\n", clients.getId());
    printf("Client's deposit %f\n", clients.getAmount());
    clients.setId(1001L);
    printf("Client now %ld\n", clients.getId());

    ConsoleLogger my_logger{"This logger is on console"};
    Bank my_bank{&my_logger, clients};
    my_bank.make_transfer(1200, 1400, 45.65);
    printf("Values in bank, ID: %ld; amount: %f\n", my_bank.getDatabase().getId(), my_bank.getDatabase().getAmount());
}