// useful as text file for testing mgrep as developed in exercise_20_4
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
  std::string command{ "ping -c 4 google.com" };
  const auto result = std::system(command.c_str());
  std::cout << "The command \'" << command << "\' returned " << result << "\n";
}
