# /*
#  * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
#  * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
#  *
#  * This Source Code Form is subject to the terms of the Mozilla Public
#  * License, v. 2.0. If a copy of the MPL was not distributed with this
#  * file, You can obtain one at http://mozilla.org/MPL/2.0/.
#  */

cmake_minimum_required(VERSION 2.8)

project(caches-pfa)
set(EXECUTABLE_OUTPUT_PATH bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -DDEFAULT_LUA_FILE=${CMAKE_CURRENT_SOURCE_DIR}/manage_threads.lua -DXSL_DOC_FILE=${CMAKE_CURRENT_SOURCE_DIR}/architecture/archi_maker.xsl")
find_package(LibXml2 REQUIRED)
find_package(LibXslt REQUIRED)
find_package(Java QUIET)
find_package(Lua51 QUIET)
find_package(Lua52 QUIET)
find_program(LUA_LIBRAIRIES llua lua5.2 QUIET)

if (NOT LUA_LIBRARIES)
  message(FATAL_ERROR "Please precise your lua libraries with -DLUA_LIBRARIES=-llua5.2 for example")
endif (NOT LUA_LIBRARIES)

if (NOT LUA_INCLUDE_DIR)
  message(FATAL_ERROR "Please precise your lua include dir with -DLUA_INCLUDE_DIR=/usr/include/lua5.2 for example")
endif (NOT LUA_INCLUDE_DIR)

set(test_main_file src/main_test.c)


option(DEBUG "Build debug" OFF)
option(OPT "Build with optimizations" OFF)
if (DEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
elseif (OPT)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif (DEBUG)

option(TEST "Build tests" OFF)
set(main_file src/main.c)
set(executable_name cassis)
set(src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

execute_process(
  COMMAND java -jar bin/Smc.jar -c coherence.sm
  WORKING_DIRECTORY ${src_dir}/state-machine
  ERROR_VARIABLE err_smc)
if (err_smc)
  message(FATAL_ERROR "Bad smc file :\n${err_smc}")
endif (err_smc)
file(GLOB source_files ${src_dir}/core/*.c ${src_dir}/state-machine/*.c)	
include_directories (${src_dir}/includes ${src_dir}/state-machine)

if(TEST)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFILE_ARCHI=${CMAKE_CURRENT_SOURCE_DIR}/architecture/architest.xml -DOUT_ARCHI=${CMAKE_CURRENT_SOURCE_DIR}/architecture/architest.cassis.xml")
  file(GLOB test_source_files ${src_dir}/tests/*.c ${src_dir}/core/*.c ${src_dir}/state-machine/*.c)
  include_directories (${src_dir}/includes ${src_dir}/state-machine ${src_dir}/tests)
  add_executable(
    test
    ${test_main_file} ${test_source_files}
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
