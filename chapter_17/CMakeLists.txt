cmake_minimum_required(VERSION 3.1...3.14)

project(chapter_17 LANGUAGES CXX)

catch_add_test(filesystem filesystem.cpp)
add_executable(listing_17_2 listing_17_2.cpp)
add_executable(listing_17_3 listing_17_3.cpp)
add_executable(listing_17_4 listing_17_4.cpp)
add_executable(listing_17_5 listing_17_5.cpp)
add_executable(listing_17_6 listing_17_6.cpp)
add_executable(listing_17_7 listing_17_7.cpp)
add_executable(listing_17_8 listing_17_8.cpp)
add_executable(listing_17_9 listing_17_9.cpp)
add_executable(exercise_17_1 exercise_17_1.cpp)
add_executable(exercise_17_2 exercise_17_2.cpp)
add_executable(exercise_17_3 exercise_17_3.cpp)
#add_executable(prova prova.cpp)

if (MSVC)
	target_link_libraries(filesystem PRIVATE ${Boost_SYSTEM_LIBRARY})
else()
	target_link_libraries(filesystem PRIVATE stdc++fs ${Boost_SYSTEM_LIBRARY})
	target_link_libraries(listing_17_2 PRIVATE stdc++fs)
	target_link_libraries(listing_17_3 PRIVATE stdc++fs)
	target_link_libraries(listing_17_4 PRIVATE stdc++fs)
	target_link_libraries(listing_17_5 PRIVATE stdc++fs)
	target_link_libraries(listing_17_6 PRIVATE stdc++fs)
	target_link_libraries(listing_17_7 PRIVATE stdc++fs)
	target_link_libraries(listing_17_8 PRIVATE stdc++fs)
	target_link_libraries(listing_17_9 PRIVATE stdc++fs)
endif()