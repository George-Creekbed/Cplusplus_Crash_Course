#include <cstdio>
#include <memory>

// using FileGuard = std::unique_ptr<FILE, int(*)(FILE*)>;
using FileGuard = std::shared_ptr<FILE>;

struct Hal {
  Hal(FileGuard file)
  : _file(file) { 
    fprintf(_file.get(), "HELLO DAVE\n");
    printf("ownership count constructor: %d\n", _file.use_count());
  }

  ~Hal() {
    fprintf(_file.get(), "STOP, DAVE.\n");
    printf("ownership count destructor: %d\n", _file.use_count());
  }

  void write_status() {
    fprintf(_file.get(), "I'm completely operational.\n");
    printf("ownership count \"write status\": %d\n", _file.use_count());
  }

  FileGuard _file;
};

int main() {
  auto file0 = fopen("HAL9000.txt", "w");
  auto file1 = fopen("HAL9001.txt", "w");
  auto file2 = fopen("HAL9002.txt", "w");
  if(!file0 || !file1 || !file2)
    return errno;
  Hal Hal9000{FileGuard{ file0, fclose }};
  Hal Hal9001{FileGuard{ file1, fclose }};
  Hal Hal9002{FileGuard{ file2, fclose }};
  // File open here
  Hal9000.write_status();
  Hal9001.write_status();
  Hal9002.write_status();
  printf("ownership count main: %d\n", Hal9000._file.use_count());
  printf("ownership count main: %d\n", Hal9001._file.use_count());
  printf("ownership count main: %d\n", Hal9002._file.use_count());
  // File closed here
}