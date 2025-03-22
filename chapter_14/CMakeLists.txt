cmake_minimum_required(VERSION 3.1...3.14)

project(chapter_14 LANGUAGES CXX)

catch_add_test(iterators iterators.cpp)
catch_add_test(exercise_14_1 exercise_14_1.cpp)
catch_add_test(exercise_14_2 exercise_14_2.cpp)
catch_add_test(exercise_14_3 exercise_14_3.cpp)

target_link_libraries(iterators PRIVATE ${Boost_SYSTEM_LIBRARY})
