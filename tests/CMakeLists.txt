## GTest and CTest ##
find_package(GTest REQUIRED)
include(CTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_registers test_registers.cpp)
target_link_libraries(test_registers registers ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestRegisters COMMAND test_registers)

add_executable(test_mmu test_mmu.cpp)
target_link_libraries(test_mmu mmu ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestMMU COMMAND test_mmu)
