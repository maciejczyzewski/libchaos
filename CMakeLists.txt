CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7 FATAL_ERROR)

SET(CMAKE_BUILD_TYPE Release ... FORCE)

OPTION(LIBCHAOS_ENABLE_TESTING
	"Enable testing & benchmarking of the libchaos library." ON)

IF(POLICY CMP0050)
	CMAKE_POLICY(SET CMP0050 OLD)
ENDIF()

# Setting C++11 standard as default.
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_DISABLE_SOURCE_CHANGES ON)
SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Disable in-source builds.
IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	MESSAGE(FATAL_ERROR
		"In-source builds are not allowed. Please use ./install.sh to choose a build directory and initialize the build configuration. \n")
ENDIF()

# == BASICS ====================================================================

PROJECT(libchaos C CXX ASM)

# Package information.
SET(PACKAGE_NAME      "libchaos")
SET(PACKAGE_VERSION   "1.1-dev")
SET(PACKAGE_STRING    "${PACKAGE_NAME} ${PACKAGE_VERSION}")
SET(PACKAGE_TARNAME   "${PACKAGE_NAME}-${PACKAGE_VERSION}")
SET(PACKAGE_BUGREPORT "https://github.com/maciejczyzewski/libchaos/issues")

MARK_AS_ADVANCED(CLEAR CMAKE_INSTALL_PREFIX)
IF(APPLE)
	# CMake really likes finding libraries inside OS X frameworks. This can
	# create super unexpected results, such as the LDAP framework, where the
	# ldap.h header there just consists of "#include <ldap.h>" -- obviously
	# assuming /usr/include appears on the include path before that framework
	# (which wasn't really supposed to be on the include path at all). This
	# leads to a hilarious recursive include and general fireworks. Instead,
	# tell CMake to search frameworks *last*, if it doesn't find something in
	# /usr (or MacPorts/Homebrew).
	SET(CMAKE_FIND_FRAMEWORK "LAST")
	MARK_AS_ADVANCED(CMAKE_OSX_ARCHITECTURES
		CMAKE_OSX_DEPLOYMENT_TARGET
		CMAKE_OSX_SYSROOT)
ENDIF()

CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# == DISTRIBUTION ==============================================================

# Tarball information.
SET(CPACK_SOURCE_GENERATOR "ZIP;TGZ")
SET(CPACK_PACKAGE_NAME ${PACKAGE_NAME})
SET(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_TARNAME})
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${PACKAGE_STRING})
SET(CPACK_SOURCE_IGNORE_FILES "/build/;/.bzr/;~$;${CPACK_SOURCE_IGNORE_FILES}")

# Package generator.
INCLUDE(CPack)

ADD_CUSTOM_TARGET(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

# == FLAGS =====================================================================

ADD_COMPILE_OPTIONS(
	-std=c++11
	-Wall -Wextra
	-Wstrict-aliasing
	-Wunreachable-code
	-O3 -flto
	-mtune=native
	-march=native
	)

# == COMPONENTS ================================================================

# Find the appropriate compiler.
IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	EXECUTE_PROCESS(
		COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	IF(NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
		MESSAGE(FATAL_ERROR "Project requires g++ 4.7 or greater.")
	ENDIF()
ELSEIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
ELSE()
	MESSAGE(FATAL_ERROR "Your C++ compiler does not support C++11.")
ENDIF()

# Enable ccache if present and not already enabled system wide.
OPTION(SKIP_CCACHE
	"Skip enabling for ccache - no effect if ccache enabled system wide" OFF)
IF(NOT SKIP_CCACHE)
	FIND_PROGRAM(CCACHE_FOUND ccache)
	IF(CCACHE_FOUND)
		IF(NOT ("${CMAKE_CXX_COMPILER} ${CMAKE_C_COMPILER}" MATCHES ".*ccache.*"))
			set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_FOUND})
			set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_FOUND})
			MESSAGE(STATUS
				"Found ccache: ${CCACHE_FOUND} - enabling ccache as compiler wrapper")
		ELSE()
			MESSAGE(STATUS
				"Found ccache - ccache already in use as C and/or CXX compiler wrapper")
		ENDIF()
	ENDIF(CCACHE_FOUND)
ENDIF(NOT SKIP_CCACHE)

# == BUILD =====================================================================

# API
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/api DESTINATION ${CMAKE_BINARY_DIR})

# Docs
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/docs DESTINATION ${CMAKE_BINARY_DIR})

# Tests
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests DESTINATION ${CMAKE_BINARY_DIR})

##################################################################################
# add_executable(_testu01 src/_testu01.cpp) ######################################
# target_link_libraries (_testu01 chaos testu01 probdist mylib m) ################
##################################################################################

##################################################################################
# add_executable(_sandbox src/_sandbox.cpp) ######################################
# target_link_libraries (_sandbox chaos testu01 probdist mylib m) ################
##################################################################################

##################################################################################
# add_executable(chaosdeep src/chaosdeep.cpp) ####################################
# target_link_libraries (chaosdeep chaos) ########################################
##################################################################################

# == LIBRARY ===================================================================

FILE(GLOB_RECURSE FILES src/*.cc)
ADD_LIBRARY(chaos ${FILES})

# Define headers for library. PUBLIC headers are used for compiling
# the library, and will be added to client build paths.
TARGET_INCLUDE_DIRECTORIES(chaos PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	PRIVATE src)

# If we have compiler requirements for library, list them here.
TARGET_COMPILE_FEATURES(chaos
	PUBLIC cxx_auto_type
	PRIVATE cxx_variadic_templates)

# Installation 'make install' to the correct location.
INSTALL(TARGETS chaos
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)  # Windows
INSTALL(DIRECTORY include/ DESTINATION include)

# Cross-platform tool to compute hashes. (FIXME)
# INSTALL(TARGETS chaosdeep RUNTIME DESTINATION bin)

# This makes the project importable from the build directory.
EXPORT(TARGETS chaos FILE ChaosConfig.cmake)

## == FINAL ====================================================================

# Show build configuration overview.
MESSAGE("\n[${PACKAGE_NAME} v${PACKAGE_VERSION}]\n
##       compiler | ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}
##          flags | ${CMAKE_CXX_FLAGS}
##      build dir | ${CMAKE_BINARY_DIR}
## install prefix | ${CMAKE_INSTALL_PREFIX}
##        testing | ${LIBCHAOS_ENABLE_TESTING}
")

IF(LIBCHAOS_ENABLE_TESTING) ######################################### TESTING ##
	ENABLE_TESTING()

## == TESTS: TESTING ===========================================================
# https://github.com/google/googletest

	# Configuring library.
	SET(GMOCK_BUILD_TESTS OFF CACHE BOOL
		"Enable testing of the testing library.")

	# External package for testing.
	ADD_SUBDIRECTORY(deps/googletest EXCLUDE_FROM_ALL)
	INCLUDE_DIRECTORIES(${GTEST_SOURCE_DIR}/include ${GTEST_SOURCE_DIR})

	# All files containing tests should be listed here.
	FILE(GLOB FILES tests/t_*.cc)
	ADD_EXECUTABLE(tchaos cmake/googletest_header.cc ${FILES})

	# Linking our libchaos and libgtest.
	TARGET_LINK_LIBRARIES(tchaos chaos
		gtest gtest_main gmock gmock_main)

	# Final tests (TODO: split to engines/library tests).
	ADD_TEST(tchaos tchaos)

## == TESTS: BENCHMARKING ======================================================
# https://github.com/google/benchmark

	# Configuring library.
	SET(CMAKE_BUILD_TYPE Release)
	SET(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL
		"Enable testing of the benchmarking library.")

	# External package for benchmarking.
	ADD_SUBDIRECTORY(deps/benchmark EXCLUDE_FROM_ALL)
	INCLUDE_DIRECTORIES(${BENCH_SOURCE_DIR}/include ${BENCH_SOURCE_DIR})

	# All files containing tests should be listed here.
	FILE(GLOB FILES tests/b_*.cc)
	ADD_EXECUTABLE(bchaos cmake/benchmark_header.cc ${FILES})

	# Linking our libchaos and libgtest.
	TARGET_LINK_LIBRARIES(bchaos chaos
		benchmark)

	# Final tests (TODO: split to engines/library tests).
	ADD_TEST(bchaos bchaos)

ENDIF() ############################################################# TESTING ##
