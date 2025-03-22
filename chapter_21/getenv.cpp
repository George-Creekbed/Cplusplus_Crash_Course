// useful as text file for testing mgrep as developed in exercise_20_4
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
  std::string variable_name{ "PATH" };
  std::string result{ std::getenv(variable_name.c_str()) };
  std::cout << "The variable " << variable_name << " equals " << result << "\n";
}
