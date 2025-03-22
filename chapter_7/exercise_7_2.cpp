#include <cstddef>
#include <cstdio>
#include <new>

struct LargeBucket {
  const static size_t data_size{1'048'576};
  std::byte data[data_size];
};

struct Bucket {
  const static size_t data_size{ 4096 };
  std::byte data[data_size];
};

struct Heap {
  void* allocate(size_t bytes) {
    if(bytes > LargeBucket::data_size)
      throw std::bad_alloc();
    else if(bytes > Bucket::data_size)
      for(size_t i{}; i < n_heap_large_buckets; i++) {
        if(!large_bucket_used[i]) {
          large_bucket_used[i] = true;
          return large_buckets[i].data;
        }
      } 
    else if (bytes > 0)
      for(size_t i{}; i < n_heap_buckets; i++) {
        if(!bucket_used[i]) {
          bucket_used[i] = true;
          return buckets[i].data;
        }
      }

    throw std::bad_alloc();
  }

  void free(volatile void* p) {
    for(size_t i{}; i < n_heap_large_buckets; i++) {
      if(large_buckets[i].data == p) {
        large_bucket_used[i] = false;
      }
    }

    for(size_t i{}; i < n_heap_buckets; i++) {
      if(buckets[i].data == p) {
        bucket_used[i] = false;
        return;
      }
    }

    return;
  }

  static const size_t n_heap_large_buckets{ 10 };
  static const size_t n_heap_buckets{ 10 };
  LargeBucket large_buckets[n_heap_large_buckets];
  Bucket buckets[n_heap_buckets]{};
  bool large_bucket_used[n_heap_large_buckets]{};
  bool bucket_used[n_heap_buckets]{};
};

Heap heap;

void* operator new(size_t n_bytes) {
  return heap.allocate(n_bytes);
}

void operator delete(void* p, size_t n_bytes) {
  return heap.free(p);
}

int main() {
  printf("Buckets:   %p\n", heap.buckets);
  printf("Large Buckets:   %p\n", heap.large_buckets);
  auto breakfast = new unsigned int{ 0xC0FFEE };
  auto dinner = new unsigned int{ 0xDEADBEEF };
  printf("Breakfast: %p 0x%x\n", breakfast, *breakfast);
  printf("Dinner:    %p 0x%x\n", dinner, *dinner);
  try {
    delete breakfast;
    delete dinner;
    size_t i{};
    while(true) {
      new char;
      printf("Allocated a char.\n");
      new unsigned int{34};
      i += 1'048'576;
      printf("Allocated an unsigned 34 at address %d.\n", i);
    }
  } catch(const std::bad_alloc&) {
    printf("std::bad_alloc caught.\n");
  }
}