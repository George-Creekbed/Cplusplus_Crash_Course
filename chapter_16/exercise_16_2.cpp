#include <iostream>
#include <string>
#include <boost/algorithm/string/case_conv.hpp>

int main() {
  std::cout << "Input a sentence in lowercase to capitalize:\n";
  std::string my_sentence, my_sentence_capitalized;
  while (std::cin >> my_sentence) {
    boost::algorithm::to_upper(my_sentence);
    my_sentence_capitalized += my_sentence + " ";
  }

  std::cout << my_sentence_capitalized << std::endl;
}