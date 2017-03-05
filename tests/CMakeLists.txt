file(GLOB SOURCES *.cpp)
include_directories(../include ../deps)
add_executable(lua++_tests ${SOURCES})
target_link_libraries(lua++_tests LINK_PUBLIC lua++ c++ c++abi)

