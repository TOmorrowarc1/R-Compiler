cmake_minimum_required(VERSION 3.10)
project(RCode)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# 包含 FetchContent 模块以下载 gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# 使 gtest 可用
FetchContent_MakeAvailable(googletest)

# 对应命令行中的 -I，不过 target_include_directories更强调针对目标对象的引入。
set(HEADER_ALL
  header/Lexer
  header/ASTNode
  header/ASTVisitor
  header/Parser
  header/utils
)

set(EXECUTABLE_SOURCES
  src/ASTNode/ASTRootNode.cpp
  src/ASTNode/ExprArrayIndexNode.cpp
  src/ASTNode/ExprArrayNode.cpp
  src/ASTNode/ExprBlockNode.cpp
  src/ASTNode/ExprCallNode.cpp
  src/ASTNode/ExprFieldNode.cpp
  src/ASTNode/ExprFlowNode.cpp
  src/ASTNode/ExprGroupNode.cpp
  src/ASTNode/ExprIfNode.cpp
  src/ASTNode/ExprLiteralNode.cpp
  src/ASTNode/ExprLoopNode.cpp
  src/ASTNode/ExprMatchNode.cpp
  src/ASTNode/ExprMethodNode.cpp
  src/ASTNode/ExprOperBinaryNode.cpp
  src/ASTNode/ExprOperUnaryNode.cpp
  src/ASTNode/ExprPathNode.cpp
  src/ASTNode/ExprStructNode.cpp
  src/ASTNode/ExprTupleIndexNode.cpp
  src/ASTNode/ExprTupleNode.cpp
  src/ASTNode/ItemConstNode.cpp
  src/ASTNode/ItemEnumNode.cpp
  src/ASTNode/ItemFnNode.cpp
  src/ASTNode/ItemImplNode.cpp
  src/ASTNode/ItemModuleNode.cpp
  src/ASTNode/ItemStructNode.cpp
  src/ASTNode/PathQualifiedNode.cpp
  src/ASTNode/PathSimpleNode.cpp
  src/ASTNode/PatternGroupNode.cpp
  src/ASTNode/PatternIDNode.cpp
  src/ASTNode/PatternLiteralNode.cpp
  src/ASTNode/PatternMultNode.cpp
  src/ASTNode/PatternPathNode.cpp
  src/ASTNode/PatternSliceNode.cpp
  src/ASTNode/PatternStructNode.cpp
  src/ASTNode/PatternTupleNode.cpp
  src/ASTNode/PatternWildNode.cpp
  src/ASTNode/StmtEmptyNode.cpp
  src/ASTNode/StmtExprNode.cpp
  src/ASTNode/StmtItemNode.cpp
  src/ASTNode/StmtLetNode.cpp
  src/ASTNode/TypeArrayNode.cpp
  src/ASTNode/TypeInferNode.cpp
  src/ASTNode/TypeNeverNode.cpp
  src/ASTNode/TypeParentNode.cpp
  src/ASTNode/TypePathNode.cpp
  src/ASTNode/TypeSliceNode.cpp
  src/ASTNode/TypeTupleNode.cpp
  src/Lexer/lexer.cpp
  src/Lexer/token.cpp
  src/Parser/Parser.cpp
  src/Parser/ParserExpr.cpp
  src/Parser/ParserItem.cpp
  src/Parser/ParserPath.cpp
  src/Parser/ParserPattern.cpp
  src/Parser/ParserStmt.cpp
  src/Parser/ParserType.cpp
)
# 将以上文档编译为.so准备用于静态链接。
add_library(compiler_lib STATIC ${EXECUTABLE_SOURCES})
target_include_directories(compiler_lib PUBLIC ${HEADER_ALL})

# 创建编译目标：可执行文件code 并链接对应的静态库。
add_executable(code src/main.cpp)
target_link_libraries(code PRIVATE compiler_lib)
target_include_directories(code PUBLIC ${HEADER_ALL})

# 开启测试功能
enable_testing()
# 添加测试目录
add_subdirectory(tests)