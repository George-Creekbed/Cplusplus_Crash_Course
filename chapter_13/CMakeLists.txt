cmake_minimum_required(VERSION 3.1...3.14)

project(chapter_13 LANGUAGES CXX)

catch_add_test(associative_containers associative_containers.cpp)
catch_add_test(graph graph.cpp)
catch_add_test(ptree ptree.cpp)
catch_add_test(sequential_containers sequential_containers.cpp)
add_executable(exercise_13_1 exercise_13_1.cpp)
add_executable(exercise_13_2 exercise_13_2.cpp)
add_executable(exercise_13_3 exercise_13_3.cpp)
add_executable(exercise_13_45 exercise_13_45.cpp)
catch_add_test(exercise_13_6 exercise_13_6.cpp)
