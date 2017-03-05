file(GLOB SOURCES *.cpp)
include_directories(../include)
add_library(lua++ SHARED ${SOURCES})
install(TARGETS lua++ DESTINATION /usr/lib)
