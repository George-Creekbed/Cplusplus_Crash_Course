#include <cstdio>

int sum(int a, int b) {
    return a + b;
}

int main() {
    int num1{1}, num2{2};
    printf("The sum of %d and %d is %d.\n", num1, num2, sum(num1, num2));
}