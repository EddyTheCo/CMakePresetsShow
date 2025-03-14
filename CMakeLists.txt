cmake_minimum_required(VERSION 3.24)

option(BUILD_EXAMPLES "Build the examples" ON)
option(BUILD_TESTS "Build the tests" ON)
option(BUILD_DOCS "Build documentation" ON)
option(BUILD_CLIENT "Build Client app" ON)
option(BUILD_SERVER "Build Client app" ON)
include(VersionFromGit.cmake)
version_from_git(LOG OFF TIMESTAMP "%Y%m%d%H%M%S")

project(
  CMakeProject
  VERSION ${VERSION}
  DESCRIPTION "Shows the use of Cmake presets"
  LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)
include(FetchContent)

find_package(Qt6 COMPONENTS Core)
find_package(Boost COMPONENTS program_options)
if(Qt6_FOUND AND Boost_FOUND)
  qt_standard_project_setup()

  qt_add_library(Foolib include/amarula/foo/foo.hpp src/foo.cpp)
  add_library(Amarula::Foolib ALIAS Foolib)
  target_link_libraries(Foolib PUBLIC Qt6::Core)
  target_link_libraries(Foolib PRIVATE Boost::program_options)
  target_include_directories(
    Foolib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT ${PROJECT_NAME}-dev)

  install(
    TARGETS Foolib
    EXPORT ${PROJECT_NAME}-config
    COMPONENT ${PROJECT_NAME}
    ARCHIVE COMPONENT ${PROJECT_NAME}-dev)

  if(BUILD_CLIENT)
    add_executable(ClientApp src/client_app.cpp)
    target_link_libraries(ClientApp PRIVATE Foolib)
    set_target_properties(ClientApp PROPERTIES WIN32_EXECUTABLE ON MACOSX_BUNDLE
                                                                   ON)
    install(
      TARGETS ClientApp
      EXPORT ${PROJECT_NAME}-config
      COMPONENT ${PROJECT_NAME}
      BUNDLE DESTINATION . COMPONENT ${PROJECT_NAME})

    add_custom_target(
      getClientCode
      COMMAND ClientApp --getCode
      DEPENDS ClientApp
      COMMENT "Getting client code")
  endif(BUILD_CLIENT)

  if(BUILD_SERVER)
    add_executable(ServerApp src/server_app.cpp)
    target_link_libraries(ServerApp PRIVATE Foolib)
    install(
      TARGETS ServerApp
      EXPORT ${PROJECT_NAME}-config
      COMPONENT ${PROJECT_NAME})
    if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/config.txt)
      add_custom_command(
        OUTPUT config.txt
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/config.txt
                ${CMAKE_CURRENT_BINARY_DIR}/config.txt
        COMMENT "Copying config.txt to build directory")
      add_custom_target(copy_config ALL DEPENDS config.txt)
    endif(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/config.txt)
    install(
      FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.txt
      DESTINATION ${CMAKE_INSTALL_BINDIR}
      COMPONENT ${PROJECT_NAME})
  endif(BUILD_SERVER)

  if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if(BUILD_EXAMPLES)
      add_subdirectory(examples)
    endif(BUILD_EXAMPLES)
    if(BUILD_TESTS)
      include(CTest)
      add_subdirectory(tests)
    endif(BUILD_TESTS)
    include(CPack)
  endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  install(
    EXPORT ${PROJECT_NAME}-config
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    NAMESPACE Amarula::
    COMPONENT ${PROJECT_NAME}-dev)
  install(
    FILES ${CMAKE_SOURCE_DIR}/LICENSE
    DESTINATION ${CMAKE_INSTALL_DATADIR}/Amarula/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME})
  include(CMakePackageConfigHelpers)
  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${VERSION}
    COMPATIBILITY SameMajorVersion)
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}-dev)
  export(
    EXPORT ${PROJECT_NAME}-config
    NAMESPACE Amarula::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake)

endif(Qt6_FOUND AND Boost_FOUND)
if(UNIX)
  find_program(PDFLATEX NAMES pdflatex)
  if(PDFLATEX)
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/Latex/document.tex CONTENT)

    # Extract title
    string(REGEX MATCH "\\\\title{([^}]*)" TITLE "${CONTENT}")
    string(REPLACE "\\title{" "" TITLE "${TITLE}")
    string(REPLACE "}" "" TITLE "${TITLE}")
    string(REPLACE "\ " "_" TITLE "${TITLE}")

    # Extract author
    string(REGEX MATCH "\\\\author{([^}]*)" AUTHOR "${CONTENT}")
    string(REPLACE "\\author{" "" AUTHOR "${AUTHOR}")
    string(REPLACE "}" "" AUTHOR "${AUTHOR}")
    string(REPLACE "\ " "_" AUTHOR "${AUTHOR}")
    add_custom_target(
      build_pdf
      COMMAND ${PDFLATEX} -jobname="${TITLE}-${AUTHOR}" document.tex
      COMMAND ${CMAKE_COMMAND} -E rename "${TITLE}-${AUTHOR}.pdf"
              ${CMAKE_CURRENT_SOURCE_DIR}/"${TITLE}-${AUTHOR}.pdf"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Latex/"
      COMMENT "building pdf")
  endif(PDFLATEX)
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
      set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
      set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/doc")
      set(DOXYGEN_EXCLUDE_PATTERNS "*/doc/*,*/examples/*,*/test*/*,*/*boost*/*")
      set(DOXYGEN_EXTRACT_ALL YES)
      set(DOXYGEN_EXTRACT_STATIC YES)
      doxygen_add_docs(doxygen_docs ${PROJECT_SOURCE_DIR}
                       COMMENT "Generate man pages")
    endif(DOXYGEN_FOUND)
  endif(BUILD_DOCS)
endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
