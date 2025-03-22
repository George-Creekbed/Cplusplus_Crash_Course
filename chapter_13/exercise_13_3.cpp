#include <cstdio>
#include <cstring>
#include <set>

bool compareAlphanumerics(const char* a, const char* b) {
  const char* shorter;
  if (strlen(a) >= strlen(b))
    shorter = b;
  else
    shorter = a;

  for (auto i=0; i!=strlen(shorter); ++i)
    if (a[i] != b[i]) 
      return a[i] < b[i];
  
  if (shorter == a)
    return true;
  else
    return false;
}

int main(int argc, char* argv[]) {
  printf("Number of command line arguments: %d\n", argc-1);  // exclude path to .exe argument

  std::set<const char*, bool(*)(const char*, const char*)> my_args{argv+1, argv+argc, compareAlphanumerics};
  printf("of which unique: %d.\n", my_args.size());

  printf("Arguments in alphanumerical order:\n");
  for (auto item : my_args)
    printf("%s\n", item);
}