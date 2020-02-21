cmake_minimum_required (VERSION 3.15)
project (torus CXX)
set(default_build_type "Release")


add_library(logistic src/logistic.cc src/logistic.h src/controller.cc src/classdef.h)

add_executable(torus src/main.cc)
find_package(Boost REQUIRED COMPONENTS iostreams)
find_package(ZLIB REQUIRED)
find_package(GSL REQUIRED)
target_include_directories(torus PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(torus PUBLIC ${GSL_INCLUDE_DIRS})
target_link_libraries(logistic PUBLIC boost_iostreams)
target_link_libraries(logistic PUBLIC GSL::gsl)
target_link_libraries(torus PUBLIC logistic)

set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")
CONFIGURE_FILE("dap.pc.in" "dap.pc" @ONLY)
install(TARGETS torus
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )
