#include <cstdio>
#include <string>
#include <utility>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/case_conv.hpp>

std::string argvParser(const int argc, const char** argv) {
  std::string output;

  // parse input word by word ...
  for (auto word_index=1; word_index!=argc; ++word_index)
    // ... if current word is with no punctuation, add it to the string_view at once
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

bool checkPalindrome(const std::string& sentence) {
  auto bwrd{sentence.rbegin()};
  size_t mid_sentence{sentence.length()/2};
  for (auto forw=sentence.begin(); forw!=sentence.begin()+mid_sentence; ++forw) {
    if (*forw != *bwrd) {
      printf("Input sentence is not a palindrome.\n");
      return false;
    }
    ++bwrd;
  }

  printf("Success!!! Input sentence is a palindrome.\n");
  return true;
}

int main(int argc, char** argv) {
  std::string palindrome{
    std::move(
      argvParser(
        argc, 
        const_cast<const char**>(argv)
      )
    )
  };
  printf("%s\n", palindrome.c_str()); // debug
  checkPalindrome(palindrome);
}
