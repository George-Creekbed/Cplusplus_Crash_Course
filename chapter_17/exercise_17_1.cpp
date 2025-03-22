#include <iostream>
#include <string>
#include <filesystem>

using namespace std;

int main(int argc, const char** argv) {
  if(argc != 3) {
    cerr << "Usage: treedir PATH";
    return -1;
  }

  try {
    const filesystem::path sys_path{argv[1]};
    const char* my_extension{argv[2]};
    auto iterator = filesystem::recursive_directory_iterator{
      sys_path, 
      filesystem::directory_options::skip_permission_denied
    };

    cout << "Found following files in path " << sys_path << " with extension \"" << my_extension << "\":\n";
    for(const auto& entry : iterator) {
      if (entry.is_regular_file())
        if (entry.path().extension().string() == my_extension)
          cout << filesystem::relative(entry.path(), "/home/your/path/here") << '\n';  
    }
  } catch (std::exception& e) {
    cerr << e.what() << endl;
  }
}
