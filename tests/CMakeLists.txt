file(GLOB TEST_SOURCES "*.cpp")
add_executable(run_tests ${TEST_SOURCES})

set(SOURCES_TEST
  ${CMAKE_SOURCE_DIR}/header/Lexer
  ${CMAKE_SOURCE_DIR}/header/ASTNode
  ${CMAKE_SOURCE_DIR}/header/ASTVisitor
  ${CMAKE_SOURCE_DIR}/header/Parser
  ${CMAKE_SOURCE_DIR}/header/utils
)

target_include_directories(run_tests PRIVATE ${SOURCES_TEST})
# 将 gtest 链接到你的测试程序
# gtest_main 会自动生成 main 函数
target_link_libraries(run_tests PRIVATE gtest_main)
target_link_libraries(run_tests PRIVATE my_compiler_library) 

# 将测试添加到 CTest
include(GoogleTest)
gtest_discover_tests(run_tests)