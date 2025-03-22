#include <cstdio>
#include <unordered_map> 
#include <stdexcept>

int mode(const int* values, size_t length) {
    if (length==0)
        throw std::invalid_argument ("Input array is empty\n");
    std::unordered_map<int, unsigned int> counter{};
    int mode{}, mode_counter{};
    bool is_single{false};
    for (size_t i=0; i!=length; i++) {
        counter[values[i]]++;
        if (counter[values[i]] > mode_counter){
            mode_counter = counter[values[i]];
            mode = values[i];
            is_single = true;
        } else if (counter[values[i]] == mode_counter)
            is_single = false;
    }

    if (!is_single)
        throw std::runtime_error ("More than one mode present\n");
    
    return mode;
}

int main() {
    try {
        int my_array[]{340, -41, 822, 33, -41, 3, 3, 16, 0, 0, 9, 7432};    //{0, 1, 2, 3, 4, 3, 3};    //{};
        printf("The mode of my array is %d\n", mode(my_array, sizeof(my_array)/sizeof(my_array[0]))); //std::size(my_array)));
    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
        return 1;
    } catch (...) {
        printf("Unknown exception\n");
        return 2;
    }
}