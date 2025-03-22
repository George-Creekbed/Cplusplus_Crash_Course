#include <filesystem>
#include <iomanip>
#include <iostream>
#include <type_traits>
#include <concepts>

using namespace std;
using namespace std::filesystem;

struct Attributes {
  size_t size_bytes;
  size_t n_directories;
  size_t n_files;
};

void print_line(const Attributes& attributes, string_view path) {
  cout << setw(12) << attributes.size_bytes << setw(6) << attributes.n_files << setw(6) << attributes.n_directories
       << " " << path << "\n";
}

Attributes explore(const directory_entry& directory) {
  Attributes attributes{};
  for(const auto& entry : recursive_directory_iterator{ directory.path() }) {
    if(entry.is_directory()) {
      attributes.n_directories++;
    } else {
      attributes.n_files++;
      attributes.size_bytes += entry.file_size();
    }
  }
  return attributes;
}


template<typename T> concept isIter = 
  is_same<T, directory_iterator>::value || is_same<T, recursive_directory_iterator>::value;


template<isIter dirIterator> Attributes traverse(dirIterator& my_iterator) {
  cout << "Size         Files  Dirs Name\n";
  cout << "------------ ----- ----- ------------\n";
  
  Attributes my_attributes{};
  for(const auto& entry : my_iterator) {
    try {
      if(entry.is_directory()) {
        const auto attributes = explore(entry);
        print_line(attributes, entry.path().string());
        my_attributes.n_directories++;
      } else {
        my_attributes.n_files++;
        error_code ec;
        my_attributes.size_bytes += entry.file_size(ec);
        if(ec)
          cerr << "Error reading file size: " << entry.path().string() << endl;
      }
    } 
    catch(const exception& e) {
      cerr << e.what();
    }
  }

  return my_attributes;
}


int main(int argc, const char** argv) {
  if(argc != 3) {
    cerr << "Usage: treedir PATH";
    return -1;
  }
  const path sys_path{ argv[2] };
  const std::string options{argv[1]};

  Attributes root_attributes{};
  if (options[0]=='-') {
    for (const char option : options)
      if (option == 'R') {
        recursive_directory_iterator my_iter(sys_path, directory_options::skip_permission_denied);
        root_attributes = traverse(my_iter);
        print_line(root_attributes, sys_path.string());
        return 0;
      } 
  }

  directory_iterator my_iter(sys_path);
  root_attributes = traverse(my_iter);
  print_line(root_attributes, sys_path.string());
}