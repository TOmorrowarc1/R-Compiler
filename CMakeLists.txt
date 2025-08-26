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
  header/Parser
  header/ASTNode
  header/ASTVisitor
  header/ASTAnnotation
  header/ASTAnnotator
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
  src/ASTNode/ExprMethodNode.cpp
  src/ASTNode/ExprOperBinaryNode.cpp
  src/ASTNode/ExprOperUnaryNode.cpp
  src/ASTNode/ExprPathNode.cpp
  src/ASTNode/ExprStructNode.cpp
  src/ASTNode/ItemConstNode.cpp
  src/ASTNode/ItemEnumNode.cpp
  src/ASTNode/ItemFnNode.cpp
  src/ASTNode/ItemImplNode.cpp
  src/ASTNode/ItemStructNode.cpp
  src/ASTNode/PathNode.cpp
  src/ASTNode/PatternIDNode.cpp
  src/ASTNode/PatternLiteralNode.cpp
  src/ASTNode/PatternPathNode.cpp
  src/ASTNode/PatternWildNode.cpp
  src/ASTNode/PatternReferNode.cpp
  src/ASTNode/StmtEmptyNode.cpp
  src/ASTNode/StmtExprNode.cpp
  src/ASTNode/StmtItemNode.cpp
  src/ASTNode/StmtLetNode.cpp
  src/ASTNode/TypeArrayNode.cpp
  src/ASTNode/TypePathNode.cpp
  src/ASTNode/TypeReferNode.cpp
  src/Lexer/Lexer.cpp
  src/Lexer/Token.cpp
  src/Parser/Parser.cpp
  src/Parser/ParserExpr.cpp
  src/Parser/ParserItem.cpp
  src/Parser/ParserPath.cpp
  src/Parser/ParserPattern.cpp
  src/Parser/ParserStmt.cpp
  src/Parser/ParserType.cpp
  src/ASTAnnotation/Scope.cpp
  src/ASTAnnotation/Symbol.cpp
  src/ASTAnnotation/TypeKind.cpp
  src/ASTAnnotation/TypeDef.cpp
  src/ASTAnnotation/ValueInfo.cpp
  src/ASTVisitor/SemanticChecker.cpp
  src/ASTVisitor/SymbolCollector.cpp
  src/ASTVisitor/SymbolAnnotator.cpp
  src/ASTVisitor/ScopeBuiltInInit.cpp
  src/ASTVisitor/ConstEvaluator.cpp
  src/ASTAnnotator/ASTAnnotator.cpp
  src/utils/position.cpp
  src/utils/exception.cpp
)

# 将以上文档编译为.a准备用于静态链接。
add_library(compiler_lib STATIC ${EXECUTABLE_SOURCES})
target_include_directories(compiler_lib PUBLIC ${HEADER_ALL})

# 创建编译目标：可执行文件code 并链接对应的静态库。
add_executable(code src/main.cpp)
target_link_libraries(code PRIVATE compiler_lib)

# 开启测试功能
enable_testing()
# 添加测试目录
add_subdirectory(tests)