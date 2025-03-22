#include <cstdio>

struct Element {
  Element* next{}, *previous{};
  void insert_after(Element* new_element) {
    new_element->next = next;
    new_element->previous = this;
    next = new_element;
  }
  void insertBefore(Element* new_element) {
    new_element->previous = previous;
    new_element->next = this;
    previous = new_element;
  }
  char prefix[2];
  short operating_number;
};

int main() {

  // test insert_forward
  Element trooper1, trooper2, trooper3;
  trooper1.prefix[0] = 'T';
  trooper1.prefix[1] = 'K';
  trooper1.operating_number = 421;
  trooper1.insert_after(&trooper2);
  trooper2.prefix[0] = 'F';
  trooper2.prefix[1] = 'N';
  trooper2.operating_number = 2187;
  trooper2.insert_after(&trooper3);
  trooper3.prefix[0] = 'L';
  trooper3.prefix[1] = 'S';
  trooper3.operating_number = 005;

  printf("Forward traversal\n");
  for(Element* cursor = &trooper1; cursor; cursor = cursor->next) {
    printf("Storm Trooper %c%c-%d\n", cursor->prefix[0], cursor->prefix[1], cursor->operating_number);
  }
  printf("\nBackward traversal\n");
  for(Element* cursor = &trooper3; cursor; cursor = cursor->previous) {
    printf("Storm Trooper %c%c-%d\n", cursor->prefix[0], cursor->prefix[1], cursor->operating_number);
  }

  // test insertBefore
  Element country1, country2, country3, country4;
  country1.prefix[0] = 'U';
  country1.prefix[1] = 'S';
  country1.operating_number = 350;
  country2.prefix[0] = 'R';
  country2.prefix[1] = 'U';
  country2.operating_number = 150;
  country3.prefix[0] = 'C';
  country3.prefix[1] = 'N';
  country3.operating_number = 1350;
  country4.prefix[0] = 'I';
  country4.prefix[1] = 'N';
  country4.operating_number = 1550;
  country2.insertBefore(&country1);
  country1.insertBefore(&country3);
  country3.insertBefore(&country4);

  printf("\nForward traversal (countries order by population in M)\n");
  for(Element* cursor = &country4; cursor; cursor = cursor->next) {
    printf("Country code and pop: %c%c-%d\n", cursor->prefix[0], cursor->prefix[1], cursor->operating_number);
  }
  printf("\nBackward traversal (countries order by population in M)\n");
  for(Element* cursor = &country2; cursor; cursor = cursor->previous) {
    printf("Country code and pop: %c%c-%d\n", cursor->prefix[0], cursor->prefix[1], cursor->operating_number);
  }
}
