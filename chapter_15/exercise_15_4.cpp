#include <cstdio>
#include <exception>
#include <string>
#include <tuple>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/case_conv.hpp>

std::string argvParser(const int argc, const char** argv) {
  std::string output;

  // parse input word by word ...
  for (auto word_index=1; word_index!=argc; ++word_index)
    // ... if current word is with no punctuation, add it to the string_view at once
    if (boost::algorithm::all(*(argv+word_index), boost::algorithm::is_alnum() || boost::algorithm::is_any_of("+-*/")))
      output.append(*(argv+word_index));
    // ... if punctuation present, add alphanum chars one by one
    else {
      const char* word{*(argv+word_index)};
      for (auto letter_index=0; letter_index!=strlen(word); ++letter_index)
        if (boost::algorithm::is_alnum()(*(word+letter_index)) || boost::algorithm::is_any_of("+-*/")(*(word+letter_index)))
          output.push_back(*(word+letter_index));
    }

  boost::algorithm::to_upper(output);

  return output;
}

std::tuple<int, char, int> parseOperation(std::string_view input) {
  size_t index_operator = input.find_first_of("+-*/");
  if (index_operator == 0 || index_operator == input.size()-1)
    throw std::logic_error("Operator sign must be placed between two numbers\n");
  
  char operation = input[index_operator];

  std::string factor1_str{input.substr(0, index_operator)};
  std::string factor2_str{input.substr(index_operator+1, input.size())};
  int factor1{}, factor2{};

  if (boost::algorithm::all(factor1_str, boost::algorithm::is_digit()) 
      && 
      boost::algorithm::all(factor2_str, boost::algorithm::is_digit())) {
    factor1 = std::stoi(factor1_str);
    factor2 = std::stoi(factor2_str);
  } else 
    throw std::logic_error("Arithmetic symbol must be placed between integer numbers.\n" 
      "If input not delimited by quotes \"\", arithmetic symbol must be preceded by a backslash \\ \n"
    );
  
  return std::make_tuple(factor1, operation, factor2);
}

int doArithmetic(const int factor1, const char& operation, const int factor2) {
  int result{};

  switch (operation) {
    case ('+'):
      result = factor1 + factor2;
      break;
    case ('-'):
      result = factor1 - factor2;
      break;
    case ('*'):
      result = factor1 * factor2;
      break;
    case ('/'):
      result = factor1 / factor2;
      break;
    default:
      throw std::logic_error("Unrecognised operation symbol\n");
      break;
  }
  
  return result;
}

int main(int argc, char** argv) {
  try {
    std::string my_input{
      std::move(
        argvParser(
          argc, 
          const_cast<const char**>(argv)
        )
      )
    };
    printf("%s\n", my_input.c_str()); // debug

    int factor1{}, factor2{};
    char operation;
    std::tie(factor1, operation, factor2) = parseOperation(my_input);

    int result = doArithmetic(factor1, operation, factor2);
    printf("= %d\n", result);
  }

  catch(const std::exception& e) {
    printf("Thrown exception:\n%s\n", e.what());
  }
}