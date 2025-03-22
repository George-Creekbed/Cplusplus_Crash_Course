#include <cstdio>

void readWhole(const char* string, const int length) {
  for (size_t i=0; i != length; i++) {
    printf("%c", string[i]);
  }
  printf("\n");
}

char readFrom(const char* string, const int length, const int idx_pick) {
  if (idx_pick < 0 || idx_pick > length-1)
    printf("Picked of choice is out-of-bounds\n");

  printf("Letter at index %d of %s is %c\n", idx_pick, string, string[idx_pick]);

  return string[idx_pick];
}

char* writeTo(char* string, const int length, const int idx_pick, const char ltr_pick) {
  if (idx_pick < 0 || idx_pick > length-1)
    printf("Picked index out-of-bounds\n");
  
  string[idx_pick] = ltr_pick;

  readWhole(string, length);

  return string;
}

int main() {
  char lower[] = "abc?e";
  char upper[] = "ABC?E";
  char* upper_ptr = upper; // Equivalent: &upper[0]

  int length_lower = sizeof(lower)/sizeof(lower[0]);
  int length_upper = sizeof(upper)/sizeof(upper[0]);

  lower[3] = 'd'; // lower now contains a b c d e \0
  upper_ptr[3] = 'D'; // upper now contains A B C D E \0

  printf("Apply functions on lower:\n");
  readWhole(lower, length_lower);
  writeTo(lower, length_lower, 3, 'g');
  readFrom(lower, length_lower, 3);

  printf("\nApply functions on upper:\n");
  readWhole(upper, length_upper);
  writeTo(upper, length_upper, 3, 'G');
  readFrom(upper, length_upper, 3);
}
