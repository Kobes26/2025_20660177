cmake_minimum_required(VERSION 3.16)

project(Worksheet5 LANGUAGES CXX)

# Choose package type by platform
if(WIN32)
  set(CPACK_GENERATOR "NSIS")
else()
  set(CPACK_GENERATOR "ZIP")
endif()

include(CPack)

# Build the maths library from adder.cpp
add_library(maths adder.cpp)

# Build calc executable from calc.cpp and link to maths
add_executable(calc calc.cpp)
target_link_libraries(calc maths)

# Install rules for packaging
install(TARGETS calc maths
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
)
