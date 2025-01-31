# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\db_productivity_test_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\db_productivity_test_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\untitled1_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\untitled1_autogen.dir\\ParseCache.txt"
  "db_productivity_test_autogen"
  "untitled1_autogen"
  )
endif()
