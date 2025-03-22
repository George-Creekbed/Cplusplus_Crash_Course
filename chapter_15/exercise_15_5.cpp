#include <cstdio>
#include <exception>
#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/case_conv.hpp>

std::string argvParser(const int argc, char** argv) {
  std::string output;

  // parse input word by word ...
  for (auto word_index=1; word_index!=argc; ++word_index)
    // ... if current word is with no punctuation, add it to the string_view at once
    if (boost::algorithm::all(*(argv+word_index), boost::algorithm::is_alnum() || boost::algorithm::is_any_of("+-*/%()")))
      output.append(*(argv+word_index));
    // ... if punctuation present, add alphanum chars one by one
    else {
      const char* word{*(argv+word_index)};
      for (auto letter_index=0; letter_index!=strlen(word); ++letter_index)
        if (boost::algorithm::is_alnum()(*(word+letter_index)) || boost::algorithm::is_any_of("+-*/%()")(*(word+letter_index)))
          output.push_back(*(word+letter_index));
    }

  boost::algorithm::to_upper(output);

  return output;
}


int doArithmetic(const std::string& input, const int previous_symbol, const int this_symbol, const int next_symbol){
  const char operation{input[this_symbol]};
  std::string factor1_str{};
  if (previous_symbol==0 && input[previous_symbol]=='-')  // accounts for the - if 1st factor is negative number
    factor1_str.assign(std::string{input.substr(previous_symbol, this_symbol-previous_symbol)});
  else
    factor1_str.assign(std::string{input.substr(previous_symbol+1, this_symbol-previous_symbol-1)});
  std::string factor2_str{input.substr(this_symbol+1, next_symbol-this_symbol-1)};
  // printf("factor1: %s\n", factor1_str.c_str());
  // printf("factor2: %s\n", factor2_str.c_str());

  int factor1 = std::stoi(factor1_str);
  int factor2 = std::stoi(factor2_str);
  int result{};

  switch (operation) {
  case ('*'):
    result = factor1 * factor2;
    break;
  case ('/'):
    result = factor1 / factor2;
    break;
  case ('%'):
    result = factor1 % factor2;
    break;
  case ('+'):
    result = factor1 + factor2;
    break;
  case ('-'):
    result = factor1 - factor2;
    break;
  default:
    throw std::logic_error("Failed to read arithmetic symbol (*, /, +, - or %)\n");
    break;
  }
  // printf("result: %d\n", result);

  return result;
}


void parseMultDivMod(std::string& input) {
  int idx_product_symbol = input.find_first_of("*/%");

  // perform all */% operations present within the input
  while (idx_product_symbol != std::string::npos) {
    // printf("In function multiplication division:\n");
    // printf("%s\n", input.c_str());

    // locate the two factors of a */% operation via other symbols to their left and right
    int previous_symbol = input.find_last_of("+-*/%", idx_product_symbol-1);
    if (input[previous_symbol]=='-' && boost::algorithm::is_any_of("+-*/%")(input[previous_symbol-1]))
      previous_symbol--;
    if (previous_symbol == std::string::npos)
      previous_symbol = -1;
    int next_symbol = input.find_first_of("+-*/%", idx_product_symbol+1);
    if (input[idx_product_symbol+1]=='-')
      next_symbol = input.find_first_of("+-*/%", idx_product_symbol+2);
    if (next_symbol == std::string::npos)
      next_symbol = input.size();
    // printf("current symbol: %d, previous: %d, next: %d\n", idx_product_symbol, previous_symbol, next_symbol);

    // perform */% operation
    int res{doArithmetic(input, previous_symbol, idx_product_symbol, next_symbol)};

    // recast */% operation result from number to std::string
    std::string prod_str = std::to_string(res);
    std::string plug_result{};
    // if (prod_str[0] == '-')
    //   if (input[previous_symbol] == '-')
    //     prod_str.assign(prod_str.begin()+1, prod_str.end());
    if (previous_symbol > 0)
      plug_result = std::move(std::string{input.begin(), input.begin()+previous_symbol+1});
    plug_result.append(prod_str);
    plug_result = plug_result + input.substr(next_symbol, input.size()-next_symbol);
    input.assign(plug_result);  // using auxiliary std::string above + assign() method, helps preserve iterators
    idx_product_symbol = input.find_first_of("*/%");
  } 
}


void parseAddSubtract(std::string& input) {
  int idx_add_subtract_symbol = input.find_last_of("+-");

  // same as above function, but for +- operations
  while (idx_add_subtract_symbol != std::string::npos) {
    // printf("In function addiction subtraction:\n");
    // printf("%s\n", input.c_str());

    int previous_symbol = input.find_last_of("+-", idx_add_subtract_symbol-1);
    if (input[previous_symbol]=='-' && previous_symbol!=0) {
      std::string aux{input.begin(), input.begin()+previous_symbol};
      aux.push_back('+');
      aux.append(input.begin()+previous_symbol, input.end());
      input.assign(aux);
      idx_add_subtract_symbol++;
      // printf("%s\n", input.c_str());
    }
    int next_symbol = input.find_first_of("+-", idx_add_subtract_symbol+1);
    // printf("current symbol: %d, previous: %d, next: %d\n", idx_add_subtract_symbol, previous_symbol, next_symbol);

    if (idx_add_subtract_symbol==0) {
      if (input[0] == '+' || input[1] == '0')
        input.assign(input.begin()+1, input.end());
      break;
    }

    if (previous_symbol == std::string::npos)
      previous_symbol = -1;   
    if (next_symbol == std::string::npos)
      next_symbol = input.size();

    int res{doArithmetic(input, previous_symbol, idx_add_subtract_symbol, next_symbol)};

    std::string add_subtract_str = std::to_string(res);
    std::string plug_result{};
    if (previous_symbol > 0)
      plug_result.assign(input.begin(), input.begin()+previous_symbol+1);
    if (add_subtract_str[0] == '-') {
      if (!plug_result.empty())
        plug_result.pop_back();
      if (input[previous_symbol] == '-' && previous_symbol!=0) {
        add_subtract_str.assign(add_subtract_str.begin()+1, add_subtract_str.end());
        plug_result.push_back('+');
      }
    }
    plug_result.append(add_subtract_str);
    plug_result = plug_result + input.substr(next_symbol, input.size()-next_symbol);
    input.assign(plug_result);
    idx_add_subtract_symbol = input.find_last_of("+-");
  }
}


void parseParentheses(std::string& input) {
  size_t idx_close_bracket = input.find_first_of(")");

  // starting from the innermost bracketed operation, progressively resolve all bracketed operations
  while (idx_close_bracket != std::string::npos) {
    // printf("In function find parentheses:\n");
    // printf("%s\n", input.c_str());

    // locate the matching opening parenthesis
    size_t idx_open_bracket = input.find_last_of("(", idx_close_bracket);
    if (idx_open_bracket == std::string::npos)
      throw std::logic_error("Parentheses don't match\n");

    std::string inside_parentheses{input.substr(idx_open_bracket+1, idx_close_bracket-idx_open_bracket-1)};
    // printf("inside parentheses: %s\n", inside_parentheses.c_str());
    // perform operations inside parentheses ( )
    parseMultDivMod(inside_parentheses);
    parseAddSubtract(inside_parentheses);

    // collate result of bracketed operation to std::string input
    std::string plug_result{input.begin(), input.begin()+idx_open_bracket};
    if (inside_parentheses[0] == '-') {
      if (!plug_result.empty())
        plug_result.pop_back();
      if (input[idx_open_bracket-1] == '-') {
        inside_parentheses.assign(inside_parentheses.begin()+1, inside_parentheses.end());
      }
    }
    plug_result.append(inside_parentheses);
    plug_result = plug_result + input.substr(idx_close_bracket+1, input.size()-idx_close_bracket);
    input.assign(plug_result);  // using auxiliary std::string above + assign() method, helps preserve iterators
    idx_close_bracket = input.find_first_of(")");
  } 
}


void parseOperation(std::string& input) {
  parseParentheses(input);
  parseMultDivMod(input);
  parseAddSubtract(input);
}


int main(int argc, char** argv) {
  try {
    std::string my_input{std::move(argvParser(argc, argv))};
    // printf("%s\n", my_input.c_str()); // debug
    parseOperation(my_input);
    printf("= %s\n", my_input.c_str());
  } 
  catch(const std::exception& e) {
    printf("Thrown exception:\n%s\n", e.what());
  }
}