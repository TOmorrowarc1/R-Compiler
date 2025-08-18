#pragma once
#include "ASTRootNode.hpp"
#include "ExprArrayIndexNode.hpp"
#include "ExprArrayNode.hpp"
#include "ExprBlockNode.hpp"
#include "ExprCallNode.hpp"
#include "ExprFieldNode.hpp"
#include "ExprFlowNode.hpp"
#include "ExprGroupNode.hpp"
#include "ExprIfNode.hpp"
#include "ExprLiteralNode.hpp"
#include "ExprLoopNode.hpp"
#include "ExprMatchNode.hpp"
#include "ExprMethodNode.hpp"
#include "ExprOperBinaryNode.hpp"
#include "ExprOperUnaryNode.hpp"
#include "ExprPathNode.hpp"
#include "ExprStructNode.hpp"
#include "ExprTupleIndexNode.hpp"
#include "ExprTupleNode.hpp"
#include "ExprUnderScoreNode.hpp"
#include "ItemConstNode.hpp"
#include "ItemEnumNode.hpp"
#include "ItemFnNode.hpp"
#include "ItemImplNode.hpp"
#include "ItemStructNode.hpp"
#include "Parser.hpp"
#include "PathNode.hpp"
#include "PatternIDNode.hpp"
#include "PatternLiteralNode.hpp"
#include "PatternPathNode.hpp"
#include "PatternTupleNode.hpp"
#include "PatternWildNode.hpp"
#include "StmtEmptyNode.hpp"
#include "StmtExprNode.hpp"
#include "StmtItemNode.hpp"
#include "StmtLetNode.hpp"
#include "TypeArrayNode.hpp"
#include "TypeInferNode.hpp"
#include "TypePathNode.hpp"
#include "TypeSliceNode.hpp"
#include "token.hpp"
#include <stdexcept>

auto parse(const std::vector<Token> &tokens) -> std::unique_ptr<ASTRootNode>;
auto parseRootNode(TokenStream &stream) -> std::unique_ptr<ASTRootNode>;

auto parseItemNode(TokenStream &stream) -> std::unique_ptr<ItemNode>;

auto parseStmtNode(TokenStream &stream) -> std::unique_ptr<StmtNode>;

auto parseExprNode(TokenStream &stream) -> std::unique_ptr<ExprNode>;
auto parseExprBlockNode(TokenStream &stream) -> std::unique_ptr<ExprBlockNode>;
auto parseExprLiteralNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralNode>;
auto parseExprPathNode(TokenStream &stream) -> std::unique_ptr<ExprPathNode>;

auto parsePatternNode(TokenStream &stream) -> std::unique_ptr<PatternNode>;

auto parseTypeNode(TokenStream &stream) -> std::unique_ptr<TypeNode>;

auto parsePathNode(TokenStream &stream) -> std::unique_ptr<PathNode>;