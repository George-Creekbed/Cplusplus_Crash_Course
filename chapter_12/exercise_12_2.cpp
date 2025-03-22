#include <cstdio>
#include <cstring>
#include <random>

const char numerics[] = {"0123456789"};
const char upper_case[] = {"ABCDEFGHIJKLMNOPQRSTUWXYZ"};
const char lower_case[] = {"abcdefghijklmnopqrstuvwxyz"};
char alphanumeric[63];

int main() {
  strcpy(alphanumeric, numerics);
  strcat(alphanumeric, lower_case);
  strcat(alphanumeric, upper_case);
  // REQUIRE(alphanumeric == "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
  printf("alphanumeric: %s\n", alphanumeric);

  // random engine seed
  std::random_device critto{};

  // draw password length (clustering around ten ciphers approx normal)
  std::binomial_distribution<unsigned> binom_distr{20, 0.5};
  unsigned password_length{binom_distr(critto)};

  // draw password from alphanumeric with uniform distr
  printf("Generate alphanumeric password:\n");
  char password[password_length];
  std::uniform_int_distribution unif_distr{0, 61};
  for (auto i=0; i<password_length; ++i) {
    auto draw = unif_distr(critto);
    password[i] = alphanumeric[draw];
  }
  printf("%s\n", password);
  printf("of length: %d\n", strlen(password));
}