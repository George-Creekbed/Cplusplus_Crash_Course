#include <cstdio>
#include <type_traits>

class Account {
public:
  virtual ~Account() {}

  virtual const long getId() = 0;
  virtual void setId(const long) = 0;
  virtual const double getBalance() = 0;
  virtual void setBalance(const double) = 0;
  virtual void addToBalance(const double) = 0;
};

class CheckingAccount: public Account {
public:
  CheckingAccount() = default;
  CheckingAccount(const long id, const double balance): _id{id}, _balance{balance} {}
  ~CheckingAccount() {}

  const long getId() {
    return _id;
  }
  void setId(const long id) {
    _id = id;
  }

  const double getBalance() {
    return _balance;
  }
  void setBalance(const double balance) {
    _balance = balance;
  }

  void addToBalance(const double sum) {
    _balance += sum;
  }

private:
  long _id;
  double _balance;
};

class SavingAccount: public Account {
public:
  SavingAccount() = default;
  SavingAccount(const long id, const double balance): _id{id}, _balance{balance} {}
  ~SavingAccount() {}

  const long getId() {
    return _id;
  }
  void setId(const long id) {
    _id = id;
  }

  const double getBalance() {
    return _balance;
  }
  void setBalance(const double balance) {
    _balance = balance;
  }

  void addToBalance(const double sum) {
    _balance += sum;
  }
  
private:
  long _id;
  double _balance;
};


////////////////////////////////////////////////////////////////
template<typename T> class Logger {
public:
  virtual void log_transfer(T, T, const double) = 0;
};

template<typename T> class ConsoleLogger : public Logger<T> {
public:
  void log_transfer(T from, T to, const double amount) override {
      printf("[console] %ld -> %ld: %f\n", from, to, amount);
  }
};

template<> class ConsoleLogger<Account*> : public Logger<Account*> {
public:
  void log_transfer(Account* from, Account* to, const double amount) override {
    printf("[console] %ld -> %ld: %f\n", from->getId(), to->getId(), amount);
  }
};


template<typename T> class FileLogger : public Logger<T> {
public:
  void log_transfer(T from, T to, const double amount) override {
    printf("[file] %ld, %ld, %f\n", from, to, amount);
  }
};

template<> class FileLogger<Account*> : public Logger<Account*> {
public:
  void log_transfer(Account* from, Account* to, const double amount) override {
    printf("[file] %ld, %ld, %f\n", from->getId(), to->getId(), amount);
  }
};


//////////////////////////////////////////////////////////////////// 
template<typename T> struct Bank {
  void set_logger(Logger<T>* new_logger) {
    logger = new_logger;
  }

  void make_transfer(T from, T to, const double amount)
    requires(!std::is_same_v<T, Account>) {
    if(logger)
      logger->log_transfer(from, to, amount);
  }
  void make_transfer(T* from, T* to, const double amount) {}

private:
  Logger<T>* logger;
};

template<> struct Bank<Account*> {
  void set_logger(Logger<Account*>* new_logger) {
    logger = new_logger;
  }

  void make_transfer(Account* from, Account* to, const double amount) {
    if(logger)
      logger->log_transfer(from, to, amount);
    from->addToBalance(-amount);
    to->addToBalance(amount);
    printf("Account no.%ld balance is now %f\n", from->getId(), from->getBalance());
    printf("Account no.%ld balance is now %f\n", to->getId(), to->getBalance());    
  }

private:
  Logger<Account*>* logger;
};


/////////////////////////////////////////////
int main() {
  ConsoleLogger<long> console_logger;
  FileLogger<long> file_logger;
  Bank<long> bank;
  bank.set_logger(&console_logger);
  bank.make_transfer(1000L, 2000L, 49.95);
  bank.set_logger(&file_logger);
  bank.make_transfer(2000L, 4000L, 20.00);
  
  CheckingAccount* a = new CheckingAccount{500, 2600};
  CheckingAccount* as = new CheckingAccount{500, 20'000}; 
  SavingAccount* b = new SavingAccount{1000, 10'000};
  SavingAccount* bs = new SavingAccount{1000, 50'000};

  printf("Account no.%ld start balance is %f\n", a->getId(), a->getBalance());
  printf("Account no.%ld start balance is %f\n", b->getId(), b->getBalance());
  ConsoleLogger<Account*> console_logger2;
  FileLogger<Account*> file_logger2;
  Bank<Account*> bank2;
  bank2.set_logger(&console_logger2);
  bank2.make_transfer(a, b, 49.95);
  bank2.set_logger(&file_logger2);
  bank2.make_transfer(b, a, 20.00);
}
