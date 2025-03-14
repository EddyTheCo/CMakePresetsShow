add_executable(example example.cpp)

set_target_properties(example PROPERTIES WIN32_EXECUTABLE ON MACOSX_BUNDLE ON)
target_link_libraries(example PRIVATE Foolib)
install(
  TARGETS example
  COMPONENT ${PROJECT_NAME}-examples
  BUNDLE DESTINATION . COMPONENT ${PROJECT_NAME}-examples
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          COMPONENT ${PROJECT_NAME}-examples)
