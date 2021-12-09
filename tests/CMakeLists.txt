find_package(GTest REQUIRED)

set(TEST_REGISTERS_SOURCES test_registers.cpp)

set(TEST_REGISTERS_LIBRARIES registers ${GTEST_LIBRARIES}
                             ${GTEST_MAIN_LIBRARIES} pthread)
add_executable(test_registers ${TEST_REGISTERS_SOURCES})
target_include_directories(test_registers PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(test_registers registers ${GTEST_LIBRARIES}
                      ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestRegisters COMMAND test_registers)
