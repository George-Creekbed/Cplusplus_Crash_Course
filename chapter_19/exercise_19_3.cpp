#include <iostream>
#include <queue>
#include <mutex>
//#include <concepts>
//#include <type_traits>
//#include <typeinfo>
#include <future>

// template<class T> class ThreadSafeQueue;
// template <typename U, typename T> 
// concept isQueue = std::is_same_v<U, std::queue<T> > || std::is_same_v<U, ThreadSafeQueue<T> >;

template<class T> class ThreadSafeQueue {
public:
  ThreadSafeQueue(){};

  T& front() {
    std::lock_guard<std::mutex> padlock(_mutex);
    return _queue.front();
  }


  T& back() {
    std::lock_guard<std::mutex> padlock(_mutex);
    return _queue.back();
  }


  bool empty() const {
    std::lock_guard<std::mutex> padlock(_mutex);
    return _queue.empty();
  }


  size_t size() const {
    std::lock_guard<std::mutex> padlock(_mutex);
    return _queue.size();
  }


  void push(const T& item) {
    std::lock_guard<std::mutex> padlock(_mutex);
    _queue.push(item);
  }


  void pop() {
    std::lock_guard<std::mutex> padlock(_mutex);
    _queue.pop();
  }


  template <class... Args> void emplace(Args&&... args) {
    std::lock_guard<std::mutex> padlock(_mutex);
    _queue.emplace(args...);
  }

  
  //template<isQueue<T> OtherQueue>
  void swap(ThreadSafeQueue& other) noexcept 
  {
    std::lock_guard<std::mutex> padlock(_mutex);

    // if (typeid(other) == typeid(std::queue<T>&))
    //   _queue.swap(other);
    // else
    _queue.swap(other._queue);
  }


  void clear() {
    std::lock_guard<std::mutex> padlock(_mutex);
    std::queue<T> empty_queue{};
    _queue.swap(empty_queue);
  }


  void printAndClear() {
    while (!empty() ) {
      std::cout << front() << " ";
      pop();
    }
    std::cout << std::endl;
  }

private:
  std::queue<T> _queue{};
  mutable std::mutex _mutex;
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
}


