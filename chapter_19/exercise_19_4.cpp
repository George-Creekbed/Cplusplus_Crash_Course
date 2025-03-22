#include <iostream>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>

template<class T> class ThreadSafeQueue {
public:
  ThreadSafeQueue(){};

  T& front() {
    std::scoped_lock<std::mutex> padlock(_mutex);
    return _queue.front();
  }


  T& back() {
    std::scoped_lock<std::mutex> padlock(_mutex);
    return _queue.back();
  }


  bool empty() const {
    std::scoped_lock<std::mutex> padlock(_mutex);
    return _queue.empty();
  }


  size_t size() const {
    std::scoped_lock<std::mutex> padlock(_mutex);
    return _queue.size();
  }


  void push(const T& item) {
    std::scoped_lock<std::mutex> padlock(_mutex);
    _queue.push(item);
    
    _cv.notify_all();
  }


  void pop() {
    std::scoped_lock<std::mutex> padlock(_mutex);
    _queue.pop();
  }


  template <class... Args> void emplace(Args&&... args) {
    std::scoped_lock<std::mutex> padlock(_mutex);
    _queue.emplace(args...);

    _cv.notify_one();
  }

  
  void swap(ThreadSafeQueue& other) noexcept 
  {
    std::scoped_lock<std::mutex> padlock(_mutex);
    _queue.swap(other._queue);

    _cv.notify_all();
  }


  void clear() {
    std::scoped_lock<std::mutex> padlock(_mutex);
    std::queue<T> empty_queue{};
    _queue.swap(empty_queue);
  }


  void printAndClear() {
    std::scoped_lock<std::mutex> padlock(_mutex2); // use 2nd mutex since calling class methods below

    while (!empty() ) {
      std::cout << front() << " ";
      pop();
    }
    std::cout << std::endl;
  }


  T& waitAndPop() {
    std::unique_lock<std::mutex> padlock(_mutex2); // use 2nd mutex since calling class methods below

    _cv.wait(padlock, [&](){return !empty();} );

    T& ret = front();
    pop();

    return ret;
  }

private:
  std::queue<T> _queue{};
  mutable std::mutex _mutex{}, _mutex2{};
  std::condition_variable _cv{};
};


int main() {
  ThreadSafeQueue<int> my_queue;

  for (int i=1; i!=11; i++) {
    my_queue.push(i);
  }

  auto print1 = std::async(std::launch::async, [&](){my_queue.printAndClear();});
  print1.get();

  ThreadSafeQueue<int> other_queue;

  int aux[]{11, 12, 13, 14, 15};
  for (auto item : aux)
    other_queue.emplace(item);

  my_queue.swap(other_queue);

  auto print2 = std::async([&](){my_queue.printAndClear();});
  print2.get();

  auto wait_for_this = std::async([&](){return my_queue.waitAndPop();});

  my_queue.push(16);

  int waited_int = wait_for_this.get();
  std::cout << "Result of previously run 'waitAndPop(): " << waited_int << std::endl;
}