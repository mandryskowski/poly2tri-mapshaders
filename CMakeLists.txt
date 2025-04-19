cmake_minimum_required(VERSION 3.12)

project(poly2tri LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

option(P2T_BUILD_TESTS "Build tests" OFF)
option(P2T_BUILD_TESTBED "Build the testbed application" OFF)

file(GLOB SOURCES poly2tri/common/*.cc poly2tri/sweep/*.cc)
file(GLOB HEADERS poly2tri/*.h poly2tri/common/*.h poly2tri/sweep/*.h)

add_library(poly2tri ${SOURCES} ${HEADERS})
target_include_directories(poly2tri INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# pthread for emscripten
if (EMSCRIPTEN)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_link_libraries(poly2tri PRIVATE Threads::Threads)
	add_link_options("SHELL:-s USE_PTHREADS -matomics -mbulk-memory -s SIDE_MODULE=1")
	set_target_properties(poly2tri PROPERTIES
		COMPILE_FLAGS "-s USE_PTHREADS -s SIDE_MODULE=1"
		POSITION_INDEPENDENT_CODE ON
	)


	if (Threads_FOUND)
		message("-- OK.")
	else ()
		message(FATAL_ERROR "Threads not found.")
	endif ()
endif()

get_target_property(poly2tri_target_type poly2tri TYPE)
if(poly2tri_target_type STREQUAL SHARED_LIBRARY)
  target_compile_definitions(poly2tri PRIVATE P2T_SHARED_EXPORTS)
else()
  target_compile_definitions(poly2tri PUBLIC P2T_STATIC_EXPORTS)
  set_target_properties(poly2tri PROPERTIES
	POSITION_INDEPENDENT_CODE ON
  )
#  if (WIN32)
#	set_target_properties(poly2tri PROPERTIES
#		COMPILE_OPTIONS "$<$<CONFIG:Debug>:/MTd>$<$<CONFIG:Release>:/MT>"
#	)
#  endif()
endif()

if(P2T_BUILD_TESTS)
    enable_testing()
    add_subdirectory(unittest)
endif()

if(P2T_BUILD_TESTBED)
    add_subdirectory(testbed)
endif()
