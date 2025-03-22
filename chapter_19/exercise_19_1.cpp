#include <iostream>
#include <future>
#include <atomic>


class SpinLock {
public:
  SpinLock(): _lock{false} {}
  SpinLock(const SpinLock& source) = delete;
  SpinLock(SpinLock&& source) = delete;


  void lock() {
    // while( _lock) { //.load() ){

    // };
    // _lock = true; 
    while (_lock.exchange(true)){}  // use this, otherwise try_lock() does not work
  }
  

  bool try_lock() {
    bool check{!_lock};
    lock();

    return check;
  }


  void unlock() {
    _lock = false;
  }


private:
  std::atomic<bool> _lock;
};


void goat_rodeo() {
  const size_t iterations{ 1'000'000 };
  int tin_cans_available{};
  SpinLock tin_can_spinlock;
  auto eat_cans = std::async(std::launch::async,
    [&]{
      for(size_t i{}; i < iterations; i++) {
        std::lock_guard<SpinLock> guard{ tin_can_spinlock };
        // tin_can_spinlock.try_lock();
        // tin_cans_available--;
        // tin_can_spinlock.unlock();
      }
    }
  );
  auto deposit_cans = std::async(std::launch::async, 
    [&]{
      for(size_t i{}; i < iterations; i++) {
        std::lock_guard<SpinLock> guard{ tin_can_spinlock };
        // tin_can_spinlock.try_lock();
        // tin_cans_available++;
        // tin_can_spinlock.unlock();
      }
    }
  );
  eat_cans.get();
  deposit_cans.get();
  std::cout << "Tin cans: " << tin_cans_available << "\n";
  // debug try_lock():
  // std::cout << "return try_lock(): " << std::boolalpha << tin_can_spinlock.try_lock(); 
  // tin_can_spinlock.unlock();
  // std::cout << std::endl; 
}

int main() {
  goat_rodeo();
  goat_rodeo();
  goat_rodeo();
}