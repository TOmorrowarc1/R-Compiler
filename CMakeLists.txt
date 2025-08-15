cmake_minimum_required(VERSION 3.10)
project(RCode)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(EXECUTABLE
  main.cpp
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

set(SOURCES_MAIN
  header/Lexer
  header/ASTNode
  header/ASTVisitor
  header/Parser
  header/utils
)

add_executable(code ${EXECUTABLE})

target_include_directories(code PUBLIC ${SOURCES_MAIN})
