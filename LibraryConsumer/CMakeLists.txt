cmake_minimum_required(VERSION 3.24)

project(
  LibraryConsumer
  VERSION 1.0
  DESCRIPTION
    "shows the use of find_package and FetchContent to integrate the library"
  LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(FetchContent)
FetchContent_Declare(
  CMakeProject
  GIT_REPOSITORY https://github.com/EddyTheCo/CMakePresetsShow.git
  GIT_TAG v0.1.0
  FIND_PACKAGE_ARGS 0.1 CONFIG)
set(BUILD_SERVER OFF)
set(BUILD_CLIENT OFF)
FetchContent_MakeAvailable(CMakeProject)

add_executable(ClientAppExternal
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/client_app.cpp)
target_link_libraries(ClientAppExternal PRIVATE Amarula::Foolib)
set_target_properties(ClientAppExternal PROPERTIES WIN32_EXECUTABLE ON
                                                   MACOSX_BUNDLE ON)
