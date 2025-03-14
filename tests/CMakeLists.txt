add_executable(testfoo test.cpp)
target_link_libraries(testfoo PRIVATE Foolib)
add_test(NAME testfooo COMMAND testfoo)
