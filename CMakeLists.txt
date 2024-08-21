cmake_minimum_required(VERSION 3.10)

project(mycodetest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
add_subdirectory(thirdlib/googletest)


set(SOURCE_FILES
    src/main.cpp
    include/sample_head.cpp
    include/vector_process.cpp
    include/multi_physics_simulator.cpp
    include/Components/RobotComponent.cpp
    include/Components/RobotArm.cpp
    include/Components/Joint.cpp
    include/Components/EndEffector.cpp
)


set(HEADER_FILES
    include/sample_head.h
    include/vector_process.h
    include/multi_physics_simulator.h
    include/Components/RobotComponent.h
    include/Components/RobotArm.h
    include/Components/Joint.h
    include/Components/EndEffector.h
)

# add_executable(main ${SOURCE_FILES})

########################### cppcheck ###########################
set(CPPCHECK_EXECUTABLE "${CMAKE_SOURCE_DIR}/thirdlib/cppcheck/cppcheck.exe")
if(NOT EXISTS "${CPPCHECK_EXECUTABLE}")
    message(FATAL_ERROR "Cppcheck executable not found: ${CPPCHECK_EXECUTABLE}")
endif()

set(CPPCHECK_RESULTS_FILE "cppcheck_results.txt")

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}")
add_custom_command(
    OUTPUT ${CPPCHECK_RESULTS_FILE}
    COMMAND ${CPPCHECK_EXECUTABLE} --enable=all --verbose ${CMAKE_SOURCE_DIR}
    COMMAND echo "Running cppcheck" > ${CPPCHECK_RESULTS_FILE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(cppcheck DEPENDS ${CPPCHECK_RESULTS_FILE})
# add_dependencies(main cppcheck)

########################### test Code ###########################

set(TEST_SOURCE_FILES
    # tests/sample_head_test.cpp
    tests/components_test.cpp
)

add_executable(runTests ${TEST_SOURCE_FILES} ${SOURCE_FILES})
target_link_libraries(runTests gtest_main)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")


enable_testing()

add_test(NAME AllTests COMMAND runTests)


set(TEST_OUTPUT_FILE "test_output.txt")
add_custom_command(
    OUTPUT ${TEST_OUTPUT_FILE}
    COMMAND $<TARGET_FILE:runTests> > "${TEST_OUTPUT_FILE}"
    DEPENDS runTests
)


add_custom_target(runTestsAndSaveOutput
    DEPENDS ${TEST_OUTPUT_FILE}
)

add_custom_command(TARGET runTestsAndSaveOutput POST_BUILD
    COMMAND $<TARGET_FILE:runTests> > "${TEST_OUTPUT_FILE}"
)

add_dependencies(runTestsAndSaveOutput cppcheck)

# 打印测试可执行文件路径
message(STATUS "Test executable path: $<TARGET_FILE:runTests>")