#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <boost/algorithm/string/case_conv.hpp>

struct AlphaHistogram {
  void ingest(std::string_view x);
  void print() const;

private:
  std::map<char, size_t> counts;
};

void AlphaHistogram::ingest(std::string_view x) {
  for (const auto c : x)
    counts[c]++;
}

void AlphaHistogram::print() const {
  for (const auto c : counts) {
    printf("%c: ", c.first);
    auto n_asterisks = c.second;
    printf("%zu ", n_asterisks);
    while (n_asterisks--)
      printf("*");
    printf("\n");
  }
}

int main() {
  std::ofstream my_file{"my_file.txt", std::ios::out};
  my_file << "Size matters not. " << "Look at me. Judge me by my size, do you? Hmm? Hmm.\n"
          << "And well you should not. " << "For my ally is the Force, and a powerful ally it is.\n"
          << "Life creates it, makes it grow. " << "Its energy surrounds us and binds us.\n"
          << "Luminous beings are we, not this crude matter.\n"
          << "You must feel the Force around you; here, between you, me, the tree, the rock, everywhere, yes."
          << std::endl;
  my_file.close();

  std::ifstream read_file{"my_file.txt"};
  if (read_file.is_open()) {
    std::string a_word;
    size_t count_words{}, count_chars{};
    AlphaHistogram hist;
    while (read_file >> a_word) {
      boost::algorithm::to_upper(a_word);
      count_words++;
      count_chars += a_word.size();
      hist.ingest(a_word);
    }

    std::cout << "Number of words=" << count_words;
    std::cout << "\nNumber of single characters=" << count_chars;
    std::cout << "\nAverage word length=" << static_cast<double>(count_chars)/count_words;
    std::cout << "\nDistribution of single letters:" << std::endl;
    hist.print();
  }
}
