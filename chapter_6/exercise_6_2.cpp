#include <cstdio>
#include <unordered_map> 
#include <stdexcept>

template<typename T> T mode(const T* values, size_t length) {
    if (length==0)
        throw std::invalid_argument ("Input array is empty\n");

    std::unordered_map<T, unsigned int> counter{};
    T mode{};
    unsigned int mode_counter{};
    bool is_single{false};

    for (size_t i=0; i!=length; i++) {
        counter[values[i]]++;
        if (counter[values[i]] > mode_counter) {
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
        int my_array[]{0, 1, 2, 3, 4, 3, 3};   //{340, -41, 822, 33, -41, 3, 3, 16, 0, 0, 9, 7432};     //{};
        long my_other_array[]{23, 45, 54, 67, 54, 23, 7, 4, 23};
        double array3[]{4.5, -2.14, 3.14, 0.001, 3.14};
        printf("The mode of my array is %d, while that of my_other_array is %ld, and finally that of a third array of floating numbers is %f\n", 
            mode(my_array, sizeof(my_array)/sizeof(my_array[0])), 
            mode(my_other_array, sizeof(my_other_array)/sizeof(my_other_array[0])),
            mode(array3, sizeof(array3)/sizeof(array3[0]))
        ); //std::size(my_array)));
    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
        return 1;
    } catch (...) {
        printf("Unknown exception\n");
        return 2;
    }
}