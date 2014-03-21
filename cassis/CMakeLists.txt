cmake_minimum_required(VERSION 2.8)

project(caches-pfa)
set(EXECUTABLE_OUTPUT_PATH bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wextra")

find_package(LibXml2 REQUIRED)
find_package(LibXslt REQUIRED)
#~ find_package(Lua52)
find_program(LUA_LIBRAIRIES lua lua.5.2)
#~ find_package(Lua52)
#~ def LUA_ tester si existe
#~ preciser src et bin et include
#~ 0x6fffffffffffful : initialiser bien!
#~ virer les atoi!
#~ protéger scanf
#~ coherence_sm.h pas de ./
#~ CFLAGS -std=c99
#~ -pedantic -Wall -Wextra
set(test_main_file tests/main_test.c)

option(TEST "Build tests" OFF)
set(main_file src/main_automate.c)
set(executable_name simulator)
execute_process(
  COMMAND java -jar bin/Smc.jar -c coherence.sm
  WORKING_DIRECTORY src/gen-automate)
file(GLOB source_files src/core-automate/*.c src/gen-automate/*.c)	
include_directories (src/includes-automate src/gen-automate)

if(TEST)
  file(GLOB automate_test_source_files tests/tests-automate/*.c src/core-automate/*.c src/gen-automate/*.c)
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