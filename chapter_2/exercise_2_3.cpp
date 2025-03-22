#include <cstdio>

enum class Operation {
    Add,
    Subtract,
    Multiply,
    Divide
};

struct Calculator {
    Calculator(Operation ini_op): my_op{ini_op} {}

    char getOperation() {
        switch (my_op) {
            case (Operation::Add): {
                return '+';
            } break;
            case (Operation::Subtract): {
                return '-';
            } break;
            case (Operation::Multiply): {
                return 'x';
            } break;
            case (Operation::Divide): {
                return ':';
            } break;
            default: {
                printf("Input one of the four arithmetic operations");
            }
        }

        return '0';
    }

    int calculate(int a, int b) {
        switch (my_op) {
            case (Operation::Add): {
                return a+b;
            } break;
            case (Operation::Subtract): {
                return a-b;
            } break;
            case (Operation::Multiply): {
                return a*b;
            } break;
            case (Operation::Divide): {
                return a/b;
            } break;
            default: {
                printf("Error: input one of the four arithmetic operations");
            }
        }

        return 0;
    }

private:
    Operation my_op;
};

int main(){
    Operation first_op{Operation::Add};
    Calculator my_calculator(first_op);
    printf("My operation is: %c\n", my_calculator.getOperation());
    int a{2}, b{3};
    printf("%d %c %d is equal to: %d\n", a, my_calculator.getOperation(), b, my_calculator.calculate(a,b));
}