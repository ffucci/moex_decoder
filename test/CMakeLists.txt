enable_testing()

add_executable(
    test_simba_decoder
    main.cpp
    test_simba_decoder.cpp
)
target_link_libraries(
    test_simba_decoder
    task::processors
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_simba_decoder)