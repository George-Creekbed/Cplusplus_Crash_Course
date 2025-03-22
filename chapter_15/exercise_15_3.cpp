#include <cstdio>
#include <string>
#include <map>
#include <utility>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/case_conv.hpp>

std::string argvParser(const int argc, const char** argv) {
  std::string output;

  // parse input word by word ...
  for (auto word_index=1; word_index!=argc; ++word_index)
    // ... if current word is with no punctuation, swiftly add it to the string_view
    if (boost::algorithm::all(*(argv+word_index), boost::algorithm::is_alnum()))
      output.append(*(argv+word_index));
    // ... if punctuation present, add alphanum chars one by one
    else {
      const char* word{*(argv+word_index)};
      for (auto letter_index=0; letter_index!=strlen(word); ++letter_index)
        if (boost::algorithm::is_alnum()(*(word+letter_index)))
          output.push_back(*(word+letter_index));
    }
  
  boost::algorithm::to_upper(output);

  return output;
}

std::map<char, unsigned> vowelCounter(std::string_view input) {
  std::map<char, unsigned> vowels_count;

  for (auto letter : input)
    if (letter == 'A')
      vowels_count['a']++;
    else if (letter == 'E')
      vowels_count['e']++;
    else if (letter == 'I')
      vowels_count['i']++;
    else if (letter == 'O')
      vowels_count['o']++;
    else if (letter == 'U')
      vowels_count['u']++;

  return vowels_count;
}

void printVowelFrequency(const std::map<char, unsigned>& my_map) {
  unsigned n_asterisks{};
  for (auto vowel : my_map) {
    printf("%c: ", vowel.first);
    n_asterisks = vowel.second;
    printf("%u ", n_asterisks);
    while (n_asterisks--)
      printf("*");
    printf("\n");
  }   
}

int main(int argc, char** argv) {
  std::string my_input{
    std::move(
      argvParser(
        argc, 
        const_cast<const char**>(argv)
      )
    )
  };
  printf("%s\n", my_input.c_str()); // debug

  std::map<char, unsigned> vowel_count{vowelCounter(my_input)};

  printf("Count of vowels present in \"%s\":\n", my_input.c_str());
  printVowelFrequency(vowel_count);
}
