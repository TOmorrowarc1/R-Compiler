cmake_minimum_required(VERSION 3.10)
project(RCode)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(third_party/googletest)

# 对应命令行中的 -I，不过 target_include_directories更强调针对目标对象的引入。
set(HEADER_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/header
  ${CMAKE_CURRENT_SOURCE_DIR}/header/Data/ASTAnnotation
  ${CMAKE_CURRENT_SOURCE_DIR}/header/Data/ASTNode
  ${CMAKE_CURRENT_SOURCE_DIR}/header/Data/Token
  ${CMAKE_CURRENT_SOURCE_DIR}/header/Lexer
  ${CMAKE_CURRENT_SOURCE_DIR}/header/Parser
  ${CMAKE_CURRENT_SOURCE_DIR}/header/Semantic
  ${CMAKE_CURRENT_SOURCE_DIR}/header/Semantic/ASTVisitor
  ${CMAKE_CURRENT_SOURCE_DIR}/header/Semantic/BuildInInit
  ${CMAKE_CURRENT_SOURCE_DIR}/header/Semantic/ConstTypeManager
  ${CMAKE_CURRENT_SOURCE_DIR}/header/utils
)

set(EXECUTABLE_SOURCES
  src/Data/ASTNode/ASTRootNode.cpp
  src/Data/ASTNode/ExprArrayIndexNode.cpp
  src/Data/ASTNode/ExprArrayNode.cpp
  src/Data/ASTNode/ExprBlockNode.cpp
  src/Data/ASTNode/ExprCallNode.cpp
  src/Data/ASTNode/ExprFieldNode.cpp
  src/Data/ASTNode/ExprFlowNode.cpp
  src/Data/ASTNode/ExprGroupNode.cpp
  src/Data/ASTNode/ExprIfNode.cpp
  src/Data/ASTNode/ExprLiteralNode.cpp
  src/Data/ASTNode/ExprLoopNode.cpp
  src/Data/ASTNode/ExprMethodNode.cpp
  src/Data/ASTNode/ExprOperBinaryNode.cpp
  src/Data/ASTNode/ExprOperUnaryNode.cpp
  src/Data/ASTNode/ExprPathNode.cpp
  src/Data/ASTNode/ExprStructNode.cpp
  src/Data/ASTNode/ExprUnderScoreNode.cpp
  src/Data/ASTNode/ItemConstNode.cpp
  src/Data/ASTNode/ItemEnumNode.cpp
  src/Data/ASTNode/ItemFnNode.cpp
  src/Data/ASTNode/ItemImplNode.cpp
  src/Data/ASTNode/ItemStructNode.cpp
  src/Data/ASTNode/ItemTraitNode.cpp
  src/Data/ASTNode/PathNode.cpp
  src/Data/ASTNode/PatternIDNode.cpp
  src/Data/ASTNode/PatternLiteralNode.cpp
  src/Data/ASTNode/PatternPathNode.cpp
  src/Data/ASTNode/PatternWildNode.cpp
  src/Data/ASTNode/PatternReferNode.cpp
  src/Data/ASTNode/StmtEmptyNode.cpp
  src/Data/ASTNode/StmtExprNode.cpp
  src/Data/ASTNode/StmtItemNode.cpp
  src/Data/ASTNode/StmtLetNode.cpp
  src/Data/ASTNode/TypeArrayNode.cpp
  src/Data/ASTNode/TypePathNode.cpp
  src/Data/ASTNode/TypeReferNode.cpp
  src/Data/ASTNode/TypeUnitNode.cpp
  src/Data/Token/Token.cpp
  src/Lexer/Lexer.cpp
  src/Parser/Parser.cpp
  src/Parser/ParserExpr.cpp
  src/Parser/ParserItem.cpp
  src/Parser/ParserPath.cpp
  src/Parser/ParserPattern.cpp
  src/Parser/ParserStmt.cpp
  src/Parser/ParserType.cpp
  src/Data/ASTAnnotation/ConstInfo.cpp
  src/Data/ASTAnnotation/ConstValue.cpp
  src/Data/ASTAnnotation/Scope.cpp
  src/Data/ASTAnnotation/Symbol.cpp
  src/Data/ASTAnnotation/TraitDef.cpp
  src/Data/ASTAnnotation/TypeDef.cpp
  src/Data/ASTAnnotation/TypeKind.cpp
  src/Data/ASTAnnotation/ValueInfo.cpp
  src/Semantic/ASTVisitor/ConstTypeCollector.cpp
  src/Semantic/ASTVisitor/ConstTypeEvaluator.cpp
  src/Semantic/ASTVisitor/FuncTraitCollector.cpp
  src/Semantic/ASTVisitor/SemanticChecker.cpp
  src/Semantic/ASTAnnotator.cpp
  src/Semantic/BuildInInit/BuiltInInit.cpp
  src/Semantic/ConstTypeManager/ConstEvaluator.cpp
  src/utils/position.cpp
  src/utils/exception.cpp
  src/utils/logger.cpp
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