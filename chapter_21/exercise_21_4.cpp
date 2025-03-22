// invoke on command line with (example): ./exercise_21_4 -t 4 -p string -p cout getenv.cpp ping.cpp 
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <regex>
#include <map>
#include <set>
#include <iomanip>
#include <future>
#include <atomic>
#include <mutex>

// static bool switches
bool is_help = false;
bool is_count = false;
bool is_invert = false;


// process input options
boost::program_options::variables_map process_options(int argc, char** argv) {
  using namespace boost::program_options;

  options_description description{ "./exercise_21_4 [options] -p pattern1 -p pattern2 ... path1 path2 ..." };
  description.add_options()
    ("help,h", bool_switch(&is_help), "display a help dialog")
    ("threads,t", value<int>()->default_value(4), "number of threads to use")
    ("count,c", bool_switch(&is_count), "print number of pattern matches")
    ("invert,v", bool_switch(&is_invert), "look for not(pattern)")
    ("pattern,p", value<std::vector<std::string> >(), "pattern to search for")
    ("path", value<std::vector<std::string> >(), "path to search");

  positional_options_description positional;
  positional.add("path", -1);

  command_line_parser parser{argc, argv};
  parser.options(description);
  parser.positional(positional);

  variables_map vm;
  try {
    auto parsed_result = parser.run();
    store(parsed_result, vm);
    notify(vm);
  } 
  catch(std::exception& e) {
    std::cerr << e.what() << "\n";
    std::exit(EXIT_FAILURE);
  }

  if(is_help) {
    std::cout << description;
    std::exit(EXIT_SUCCESS);
  }
  if (is_count && is_invert) {
    throw std::runtime_error("Cannot count inverted matches. "
      "Either use -c or -v, but not the two together\n"
    );
  }
  if(vm["pattern"].empty()) {
    throw std::runtime_error("You must provide at least one pattern.\n");
  }
  if(vm["path"].empty()) {
    throw std::runtime_error("You must provide at least one path.\n");
  }

  return vm;
}


int main(int argc, char** argv) {
  try {
    // read input from user
    boost::program_options::variables_map vm = process_options(argc, argv);

    const auto threads = vm["threads"].as<int>();
    const auto& patterns = vm["pattern"].as<std::vector<std::string> >();
    const auto& paths = vm["path"].as<std::vector<std::string> >();

    // store #occurrences for each match in below
    std::map<std::string, unsigned> table_of_results;

    // for each input search pattern...
    for (auto& pattern : patterns) {
      std::cout << "\nMatches for ";
      if (is_invert) {
        std::cout << "[not-]";
      } 
      std::cout << pattern << "...\n";

      unsigned counter{}; // number of matches to be found  // could be std::atomic_uint

      // for each filepath given on input...
      for (auto& filepath : paths) {
        std::cout << "In file " << filepath << "...\n";
        const std::filesystem::path current_path {
          std::filesystem::path("/home/giorgiog/C++code/Crash_Course_exercises/chapter_21") / filepath
        };
        std::ifstream file{current_path, std::ios_base::in};

        std::regex regex{pattern};
        std::smatch match;

        if (file.is_open()) {
          std::string buffer;
          unsigned line{}; // could be std::atomic_uint

          std::mutex rgx_mutex;
          // ... perform the regex search asynchronously
          std::future<void> future =
            std::async(
              std::launch::async, 
              [&]() {
                while (std::getline(file, buffer)) {
                  std::lock_guard<std::mutex> rgx_guard(rgx_mutex);
                  ++line;
                  bool match_found{false};
                  while (std::regex_search(buffer, match, regex)) {
                    match_found = true;
                    if (!is_count && !is_invert) {
                      std::cout << "Line " << line << ": " << buffer << std::endl;
                    } 
                    for (auto m : match) {
                      counter++;
                    }
                    // need line below because std::regex_search stops at 1 match per line
                    buffer = match.suffix().str(); 
                  } 
                  if (is_invert && !match_found) {
                    std::cout << "Line " << line << ": " << buffer << std::endl;
                  }
                }
              }
            )
          ;
          future.get();
        } 
        else {
          throw std::runtime_error("File could not be opened\n");
        }
      }
      table_of_results[pattern] = counter;
    }
    
    if (is_count) {
      // cannot order std::map by value only by key
      // so std::set below for ordering results by #occurrences is needed
      std::set<std::pair<unsigned, std::string> > aux; 
      for (const auto& pair : table_of_results) {
        aux.emplace(pair.second, pair.first);  // Flip pairs
      }
      std::cout << "\nOccurrences" << "---------" << "Pattern\n";
      std::set<std::pair<unsigned, std::string> >::reverse_iterator riter = aux.rbegin();
      for (riter=aux.rbegin(); riter!=aux.rend(); ++riter) {
        std::cout << std::setw(6) <<  riter->first << std::setw(20) << std::right << riter->second << '\n';
      }
    }
  }
  catch(std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}