#include <cstdio>
#include <string>
#include <map>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
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

int main(int argc, char** argv) {
  std::string input_string;
  // parse input word by word ...
  for (auto word_index=1; word_index!=argc; ++word_index)
    // ... if current word is with no punctuation, add it at once to the string_view
    if (boost::algorithm::all(*(argv+word_index), boost::algorithm::is_alnum()))
      input_string.append(*(argv+word_index));
    // ... if punctuation present, add alphanum chars one by one
    else {
      char* word{*(argv+word_index)};
      for (auto letter_index=0; letter_index!=strlen(word); ++letter_index)
        if (boost::algorithm::is_alnum()(*(word+letter_index)))
          input_string.push_back(*(word+letter_index));
    }

  boost::algorithm::to_upper(input_string);

  AlphaHistogram hist;
  hist.ingest(input_string);
  hist.print();
}
