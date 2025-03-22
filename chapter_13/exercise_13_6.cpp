#define CATCH_CONFIG_MAIN
#include <vector>
#include <exception>
#include "./Catch2/catch.hpp"

template <typename T>
struct Matrix {
  constexpr Matrix(size_t input_rows, std::initializer_list<T> val)
    : rows{ input_rows }
    , data(rows, std::vector<T>{}) 
  {
    size_t cols = val.size() / rows;
    if (val.size() % rows != 0)
      throw std::logic_error("Not enough elements to form matrix with as many rows as requested.");

    auto itr = val.begin();
    for(size_t row{}; row < rows; row++) {
      data[row].assign(itr, itr + cols);
      itr += cols;
    }
  }

  T& get(size_t row, size_t col) {
    if(row >= rows || col >= data[0].size())
      throw std::out_of_range{ "Index out of range." };
    return data[row][col];
  }

  size_t size() const {
    return data.size() * data[0].size();
  }

  size_t getNumberOfRows() const {
    return rows;
  }

  const size_t rows;

private:
  std::vector<std::vector<T>> data;
};

TEST_CASE("Matrix and std::initializer_list") {
  Matrix<int> mat(2, { 1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });

  printf("Print Matrix: \n");
  for (auto i=0; i!=mat.getNumberOfRows(); ++i){
    for (auto j=0; j != mat.size() / mat.getNumberOfRows(); ++j)
      printf("%d ", mat.get(i, j));
    printf("\n");
  }

  REQUIRE(mat.rows == 2);
  mat.get(0, 5) = 6;
  REQUIRE(mat.get(0, 5) == 6);
  REQUIRE(mat.get(1, 2) == 11);
}