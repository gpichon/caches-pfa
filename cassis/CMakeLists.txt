cmake_minimum_required(VERSION 2.8)

project(caches-pfa)
set(EXECUTABLE_OUTPUT_PATH bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}") #~ -pedantic -Wall -Wextra -std=c99

find_package(LibXml2 REQUIRED)
find_package(LibXslt REQUIRED)
find_package(Lua51 QUIET)
find_package(Lua52 QUIET)
find_program(LUA_LIBRAIRIES llua lua5.2 QUIET)

if (NOT LUA_LIBRARIES)
  message(FATAL_ERROR "Please precise your lua libraries with -DLUA_LIBRARIES=")
endif (NOT LUA_LIBRARIES)

if (NOT LUA_INCLUDE_DIR)
  message(FATAL_ERROR "Please precise your lua include dir with -DLUA_INCLUDE_DIR=")
endif (NOT LUA_INCLUDE_DIR)

set(test_main_file tests/main_test.c)

option(TEST "Build tests" OFF)
set(main_file src/main_automate.c)
set(executable_name simulator)
set(src_dir ${CMAKE_SOURCE_DIR}/src)

execute_process(
  COMMAND java -jar bin/Smc.jar -c coherence.sm
  WORKING_DIRECTORY ${src_dir}/gen-automate)
file(GLOB source_files ${src_dir}/core-automate/*.c ${src_dir}/gen-automate/*.c)	
include_directories (${src_dir}/includes-automate ${src_dir}/gen-automate)

if(TEST)
  file(GLOB automate_test_source_files tests/tests-automate/*.c ${src_dir}/core-automate/*.c ${src_dir}/gen-automate/*.c)
  add_executable(
    test
    ${test_main_file} ${automate_test_source_files}
    )
  target_link_libraries(test ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LUA_LIBRARIES})
endif(TEST)

include_directories (AFTER ${LIBXML2_INCLUDE_DIR} ${LIBXSLT_INCLUDE_DIR} ${LUA_INCLUDE_DIR})

add_executable(
  ${executable_name}
  ${main_file} ${source_files}
  )
target_link_libraries(${executable_name} ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LUA_LIBRARIES})

#~ Documentation
find_package (Doxygen)
if (DOXYGEN_FOUND)
  set (doxygenConfigFile "${CMAKE_CURRENT_SOURCE_DIR}/documentation/doxygen.conf")
  add_custom_target (
    doc
    ${DOXYGEN_EXECUTABLE} ${doxygenConfigFile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/documentation	VERBATIM
    )
endif(DOXYGEN_FOUND)