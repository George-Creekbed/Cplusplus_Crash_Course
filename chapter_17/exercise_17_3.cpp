#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <filesystem>
#include <variant>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <concepts>
#include <typeinfo>

using namespace std;
using namespace std::filesystem;

/***********************
 *** PART OF THE CODE  *
 ***  DEALING WITH   ***
 *** DATA STRUCTURE   **
 ***   FOR FILES   *****
***********************/

struct Attributes {
  size_t size_bytes;
  size_t n_directories;
  size_t n_files;
  string path;

  Attributes operator+=(const Attributes& other) {
    size_bytes += other.size_bytes;
    n_directories += other.n_directories;
    n_files += other.n_files;

    return *this;
  }
};

enum class Flag {
  size, 
  dirs,
  files
};

bool lessThan(const Attributes& one, const Attributes& two, const Flag& flag) {
  bool ret;
  switch (flag) {
    case Flag::size:
      ret = one.size_bytes < two.size_bytes;
      if (one.size_bytes == two.size_bytes)
        ret = one.n_directories < two.n_directories;
      break;
    case Flag::dirs:
      ret = one.n_directories < two.n_directories;
      if (one.n_directories == two.n_directories)
        ret = one.size_bytes < two.size_bytes;
      break;        
    case Flag::files:
      ret = one.n_files < two.n_files;
      if (one.n_files == two.n_files) {
        ret = one.n_directories < two.n_directories;
        if (one.n_directories == two.n_directories)
          ret = one.size_bytes < two.size_bytes;
      }
      break;
    default:
      throw("Allowed flags: size, dirs, files\n");
      break;
  }

  return ret;     
}


/***********************
 *** PART OF THE CODE  *
 ***  DEALING WITH   ***
 *** TRAVERSING THE   **
 ***  GIVEN DIR PATH   *
***********************/

Attributes explore(const directory_entry& directory) {
  Attributes attributes{};
  attributes.path = directory.path().string();
  for(const auto& entry : recursive_directory_iterator{ directory.path() })
    if (entry.is_directory())
      attributes.n_directories++;
    else {
      attributes.n_files++;
      attributes.size_bytes += entry.file_size();
    }

  return attributes;
}

template <typename T>
concept isDirIter = is_same_v<T, directory_iterator> || is_same_v<T, recursive_directory_iterator>;

template <isDirIter dirIterator>
void traverse(dirIterator& my_iter, vector<Attributes>& my_vec) {    
  Attributes root_attributes{};
  root_attributes.path = my_iter->path().parent_path().string();
  root_attributes.n_directories = 1;
  for (const auto& entry : my_iter) {
    Attributes entry_attributes{};
    try {
      if (entry.is_directory()) {
        const auto attributes = explore(entry);
        entry_attributes = attributes;
        entry_attributes.n_directories++;
        if (typeid(my_iter) == typeid(recursive_directory_iterator)) 
          root_attributes.n_directories++;
        else if (typeid(my_iter) == typeid(directory_iterator))
          root_attributes += entry_attributes;
      } 
      else {
        entry_attributes.path = entry.path().string();
        entry_attributes.n_files++;
        error_code ec;
        entry_attributes.size_bytes += entry.file_size(ec);
        root_attributes += entry_attributes;
        if (ec)
          cerr << "Error reading file size: " << entry.path().string() << endl;
      }
      my_vec.push_back(entry_attributes);
    } 
    catch (const exception& e) {
      cerr << "Exception: " << e.what();
    }
  }

  my_vec.push_back(root_attributes);
}


/***********************
 *** PART OF THE CODE  *
 ***  DEALING WITH   ***
 ***  MANAGING THE   ***
 ***  INPUT OPTIONS  ***
 ***  GIVEN BY USER   **
***********************/

template <Flag my_flag>
void option(string_view options, vector<Attributes>& my_vec, const char pick) {
  if (options.find(pick) != string::npos) {
    make_heap(
      my_vec.begin(), 
      my_vec.end(), 
      bind(lessThan, placeholders::_1, placeholders::_2, my_flag) 
    );
    sort_heap(
      my_vec.begin(), 
      my_vec.end(), 
      bind(lessThan, placeholders::_1, placeholders::_2, my_flag) 
    );
    if (options.find('r') != string::npos) 
      reverse(my_vec.begin(), my_vec.end());
  }
}

using DirIterVariant = variant<directory_iterator, recursive_directory_iterator>;

DirIterVariant option_R(string_view options, const path& my_path) {
  if (options.find('R') != string::npos) {
    cout << "Will scan subdirectories recursively" << endl;
    return recursive_directory_iterator(my_path, directory_options::skip_permission_denied);
  } else {
    cout << "Will scan directory only" << endl;
    return directory_iterator(my_path);
  }
}

void option_h() {
  cout << setw(5) << "-R" <<  "     " << setw(35) <<  "List subdirectories recursively\n";
  cout << setw(5) << "-s" <<  "     " << setw(35) <<  "Sort by file size\n"; 
  cout << setw(5) << "-d" <<  "     " << setw(35) <<  "Sort by number of directories\n"; 
  cout << setw(5) << "-f" <<  "     " << setw(35) <<  "Sort by number of files\n";
  cout << setw(5) << "-r" <<  "     " << setw(35) <<  "Reverse sorting order\n";  
  cout << setw(5) << "-p" <<  "     " << setw(35) <<  "Display permissions\n"; 
  cout << setw(5) << "-h" <<  "     " << setw(35) <<  "Display this information\n"; 
}


/***********************
 *** PART OF THE CODE  *
 ****  DEALING WITH   **
 ****  PRINTING THE   **
 ***  RESULTS TO THE   *
 *****   CONSOLE    ****
***********************/

void printOption_p(Attributes file) {
  perms my_permits = status(file.path).permissions();

  auto show = 
    [=](char op, perms perm) {
      cout << ( (perm & my_permits) == perms::none ? '-' : op );
    };

  show('r', perms::owner_read);
  show('w', perms::owner_write);
  show('x', perms::owner_exec);
  show('r', perms::group_read);
  show('w', perms::group_write);
  show('x', perms::group_exec);
  show('r', perms::others_read);
  show('w', perms::others_write);
  show('x', perms::others_exec);

  cout << endl;
}

void printFiles(string_view options, const vector<Attributes> my_vec) {
  size_t longest_pathname{};
  for (const auto& file : my_vec)
    longest_pathname = max(longest_pathname, file.path.size());

  if (options.find('p') != string::npos) {
    cout << "Size(bytes)  Files   Dirs " << string(6, ' ') << "File path " 
         << string(longest_pathname-10, ' ') << "Permissions\n";
    cout << string(12, '-') << " " << string(6, '-') << " " << string(6, '-') << " "
         << string(longest_pathname+3, '-') << " " << string(13, '-') << '\n';
    for (const auto& file : my_vec) {
      cout << right << setw(12) << file.size_bytes << " " << setw(6) << file.n_files << " " << setw(6) 
           << file.n_directories << " " << left << setw(longest_pathname+3) << file.path << " ";
      printOption_p(file);
    }
  } else {
    cout << "Size(bytes)  Files  Dirs Name\n";
    cout << "------------ ----- ----- ------------\n";
    for (const auto& file : my_vec)
      cout << right << setw(12) << file.size_bytes << setw(6) << file.n_files << setw(6) << file.n_directories
           << " " << left << setw(longest_pathname+3) << file.path << "\n";
  }
}


/*********************
 *****   MAIN   ******
 ********************/

int main(int argc, const char **argv) {
  if(argc != 3) {
    cerr << "Exactly two arguments needed: "
         << "(1) list of option flags preceded by hyphen '-', and "
         << "(2) file path.\n"
         << "Type -h for more help\n";
    return -1;
  }

  const path sys_path{argv[2]};
  const string options{argv[1]};

  vector<Attributes> files;

  DirIterVariant file_iterator_v;
  if (options[0] == '-') {
    if (options.find('h') != string::npos) {
      option_h();
      return 0;
    }
    if (!is_directory(sys_path)) {
      cout << "Not a directory.\n";  
      return -1; 
    } 
    else {
      file_iterator_v = option_R(options, sys_path);
      visit(
        [&](auto&& file_iterator) {
          traverse(file_iterator, files);
        }, 
        file_iterator_v
      );  

      option<Flag::dirs >(options, files, 'd');
      option<Flag::files>(options, files, 'f');
      option<Flag::size >(options, files, 's');
    }
  } 
  else {
    cout << "No flag specified, simply print shallow file search" << endl;
    directory_iterator my_iter(sys_path);
    traverse(my_iter, files);
  }
  printFiles(options, files);
}