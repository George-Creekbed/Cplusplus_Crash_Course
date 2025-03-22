#include <cstdio>
#include <memory>

using FileGuard = std::unique_ptr<FILE, int(*)(FILE*)>;
// using FileGuard = std::shared_ptr<FILE>;

void say_hello(FileGuard file) {
  fprintf(file.get(), "HELLO DAVE");
}

int main() {
  auto file = fopen("HAL9000.txt", "w");
  if(!file)
    return errno;
  FileGuard file_guard{ file, fclose };
  // File open here
  say_hello(std::move(file_guard));
  // File closed here
}
