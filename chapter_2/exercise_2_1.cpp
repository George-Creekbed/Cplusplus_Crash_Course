#include <cstdio>

enum class Operation {
    Add,
    Subtract,
    Multiply,
    Divide
};

int main() {
    Operation my_operation{Operation::Add};
    printf("This is my operation: %d\n", my_operation);
}