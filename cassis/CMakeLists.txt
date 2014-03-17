cmake_minimum_required(VERSION 2.8)

project(caches-pfa)
set(EXECUTABLE_OUTPUT_PATH bin)

find_package(LibXml2)
find_package(LibXslt)

#~ ADD_SUBDIRECTORY(src/gen-automate)

option(TEST "Build test main" OFF)
option(AUTOMATE "Build with smc diagram" ON)
if(TEST)
	set(main_file src/main_test.c)
	file(GLOB source_files src/tests/*.c src/core/*.c)
	set(executable_name test) 
elseif(AUTOMATE)
	set(main_file src/main.c)
	set(executable_name simulator)
	execute_process(
		COMMAND java -jar bin/Smc.jar -c coherence.sm
		WORKING_DIRECTORY src/gen-automate)
	file(GLOB source_files src/core-automate/*.c src/gen-automate/*.c)	
	include_directories (src/includes-automate src/gen-automate/*.h)
else()
	set(main_file src/main.c)
	set(executable_name simulator_nosmc)
	file(GLOB source_files src/core/*.c)
endif(TEST)

include_directories (AFTER src/includes ${LIBXML2_INCLUDE_DIR} ${LIBXSLT_INCLUDE_DIR})

add_executable(
	${executable_name}
	${main_file} ${source_files}
)

target_link_libraries(${executable_name} ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES})

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


