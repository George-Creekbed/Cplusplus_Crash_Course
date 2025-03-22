#include <cstdint>
#include <cstdio>

struct AlphaHistogram {
  AlphaHistogram(const int argcount)
    : _argcount{argcount}, counts{new size_t[_argcount]{}} {};
  ~AlphaHistogram() {
    delete[] counts;
  };
  void ingest(const char* x, const size_t& current);
  void print() const;

  private:
  int _argcount;
  size_t* counts;
};

void AlphaHistogram::ingest(const char* x, const size_t& current) {
  size_t index{};
  while(const auto c = x[index]) {
    counts[current]++;
    index++;
  }
}

void AlphaHistogram::print() const {
  for(auto index{ 1 }; index < _argcount; index++) {
    printf("%u: ", index);
    auto n_asterisks = counts[index];
    while(n_asterisks--)
      printf("*");
    printf("\n");
  }
}

int main(int argc, char** argv) {
  AlphaHistogram hist{argc};
  for(int i{ 1 }; i < argc; i++) {
    hist.ingest(argv[i], i);
  }
  hist.print();
}
