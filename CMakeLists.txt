cmake_minimum_required(VERSION 2.8.9)
project(cpp-arcon)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP
                BUILD missing)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})
target_link_libraries(main PocoFoundation PocoDataODBC PocoDataSQLite PocoZip PocoXML PocoJSON ${CONAN_LIBS})
