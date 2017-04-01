cmake_minimum_required(VERSION 2.8)

project (hello-boost)

set(Boost_DEBUG 1)

set(BOOST_ROOT "d:/cppworkspace/boost_1_58_0-bak/output")
#find_package(Boost REQUIRED)
#find_package(Boost COMPONENTS system REQUIRED) # DON'T WORK
find_package (Boost 1.58.0 REQUIRED COMPONENTS program_options)

if(NOT Boost_FOUND)
    message(fatal_error "Boost was not found. See FindBoost.cmake for details.")
else ()
    message(STATUS "Boost_INCLUDE_DIR=${Boost_INCLUDE_DIR}")
    message(STATUS "Boost_LIBRARY_DIR=${Boost_LIBRARY_DIR}")
    message(STATUS "Boost_FILESYSTEM_LIBRARY=${Boost_FILESYSTEM_LIBRARY}")
    message(STATUS "Boost_SYSTEM_LIBRARY=${Boost_SYSTEM_LIBRARY}")
    message(STATUS "Boost_CHRONO_LIBRARY=${Boost_CHRONO_LIBRARY}")
    message(STATUS "Boost_UNIT_TEST_FRAMEWORK_LIBRARY=${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
    message(STATUS "Boost_SIGNALS_LIBRARY=${Boost_SIGNALS_LIBRARY}")
    message(STATUS "Boost_THREAD_LIBRARY=${Boost_THREAD_LIBRARY}")
    message(STATUS "Boost_LIBRARY=${Boost_LIBRARY}")
    message(STATUS "Boost_LIBRARIES=${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

include_directories(${Boost_INCLUDE_DIR})

#build executable using specified list of source files
add_executable(hello main.cc)


# $ export BOOST_ROOT=/d/cppworkspace/boost_1_58_0
#
# $ cmake ..
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:515 ] _boost_TEST_VERSIONS = 1.58.0;1.58;1.57.0;1.57;1.56.0;1.56;1.55.0;1.55;1.54.0;1.54;1.53.0;1.53;1.52.0;1.52;1.51.0;1.51;1.50.0;1.50;1.49.0;1.49;1.48.0;1.48;1.47.0;1.47;1.46.1;1.46.0;1.46;1.45.0;1.45;1.44.0;1.44;1.43.0;1.43;1.42.0;1.42;1.41.0;1.41;1.40.0;1.40;1.39.0;1.39;1.38.0;1.38;1.37.0;1.37;1.36.1;1.36.0;1.36;1.35.1;1.35.0;1.35;1.34.1;1.34.0;1.34;1.33.1;1.33.0;1.33
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:517 ] Boost_USE_MULTITHREADED = TRUE
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:519 ] Boost_USE_STATIC_LIBS =
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:521 ] Boost_USE_STATIC_RUNTIME =
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:523 ] Boost_ADDITIONAL_VERSIONS =
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:525 ] Boost_NO_SYSTEM_PATHS =
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:577 ] Declared as CMake or Environmental Variables:
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:579 ]   BOOST_ROOT =
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:581 ]   BOOST_INCLUDEDIR =
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:583 ]   BOOST_LIBRARYDIR =
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:585 ] _boost_TEST_VERSIONS = 1.58.0;1.58;1.57.0;1.57;1.56.0;1.56;1.55.0;1.55;1.54.0;1.54;1.53.0;1.53;1.52.0;1.52;1.51.0;1.51;1.50.0;1.50;1.49.0;1.49;1.48.0;1.48;1.47.0;1.47;1.46.1;1.46.0;1.46;1.45.0;1.45;1.44.0;1.44;1.43.0;1.43;1.42.0;1.42;1.41.0;1.41;1.40.0;1.40;1.39.0;1.39;1.38.0;1.38;1.37.0;1.37;1.36.1;1.36.0;1.36;1.35.1;1.35.0;1.35;1.34.1;1.34.0;1.34;1.33.1;1.33.0;1.33
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:678 ] location of version.hpp: D:/cppworkspace/boost_1_58_0/output/include/boost-1_58/boost/version.hpp
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:702 ] version.hpp reveals boost 1.58.0
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:787 ] guessed _boost_COMPILER = -vc140
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:797 ] _boost_MULTITHREADED = -mt
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:840 ] _boost_RELEASE_ABI_TAG = -
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:842 ] _boost_DEBUG_ABI_TAG = -gd
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:890 ] _boost_LIBRARY_SEARCH_DIRS = D:/cppworkspace/boost_1_58_0/output/lib;D:/cppworkspace/boost_1_58_0/output/stage/lib;D:/cppworkspace/boost_1_58_0/output/include/boost-1_58/lib;D:/cppworkspace/boost_1_58_0/output/include/boost-1_58/../lib;D:/cppworkspace/boost_1_58_0/output/include/boost-1_58/stage/lib;PATHS;C:/boost/lib;C:/boost;/sw/local/lib
# -- [ C:/Program Files (x86)/CMake/share/cmake-3.2/Modules/FindBoost.cmake:1088 ] Boost_FOUND = 1
# -- Boost version: 1.58.0
# -- Boost_INCLUDE_DIR=D:/cppworkspace/boost_1_58_0/output/include/boost-1_58
# -- Boost_LIBRARY_DIR=
# -- Boost_FILESYSTEM_LIBRARY=
# -- Boost_SYSTEM_LIBRARY=
# -- Boost_CHRONO_LIBRARY=
# -- Boost_UNIT_TEST_FRAMEWORK_LIBRARY=
# -- Boost_SIGNALS_LIBRARY=
# -- Boost_THREAD_LIBRARY=
# -- Boost_LIBRARY=
# -- Configuring done
# -- Generating done
# -- Build files have been written to: D:/cppworkspace/condiment/cmake/06-find-boost/build
