cmake_minimum_required(VERSION 3.1...3.14)

project(chapter_15 LANGUAGES CXX)

catch_add_test(strings strings.cpp)
add_executable(c_str_example c_str_example.cpp)

add_executable(exercise_15_1 exercise_15_1.cpp)
add_executable(exercise_15_2 exercise_15_2.cpp)
add_executable(exercise_15_3 exercise_15_3.cpp)
add_executable(exercise_15_4 exercise_15_4.cpp)
add_executable(exercise_15_5 exercise_15_5.cpp)

target_link_libraries(strings PRIVATE ${Boost_SYSTEM_LIBRARY})
target_link_libraries(exercise_15_1 PRIVATE ${Boost_SYSTEM_LIBRARY})
target_link_libraries(exercise_15_2 PRIVATE ${Boost_SYSTEM_LIBRARY}) 
target_link_libraries(exercise_15_3 PRIVATE ${Boost_SYSTEM_LIBRARY}) 
target_link_libraries(exercise_15_4 PRIVATE ${Boost_SYSTEM_LIBRARY}) 
target_link_libraries(exercise_15_5 PRIVATE ${Boost_SYSTEM_LIBRARY}) 
