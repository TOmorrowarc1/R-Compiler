#pragma once
#include "ASTRootNode.hpp"
#include "ExprBlockNode.hpp"
#include "ExprCallNode.hpp"
#include "ExprFlowNode.hpp"
#include "ExprGroupNode.hpp"
#include "ExprIfNode.hpp"
#include "ExprLiteralNode.hpp"
#include "ExprLoopNode.hpp"
#include "ExprOperBinaryNode.hpp"
#include "ExprOperUnaryNode.hpp"
#include "ExprPathNode.hpp"
#include "ItemConstNode.hpp"
#include "ItemEnumNode.hpp"
#include "ItemFnNode.hpp"
#include "ItemImplNode.hpp"
#include "ItemModuleNode.hpp"
#include "ItemStructNode.hpp"
#include "Parser.hpp"
#include "PathQualifiedNode.hpp"
#include "PathSimpleNode.hpp"
#include "PatternGroupNode.hpp"
#include "PatternIDNode.hpp"
#include "PatternLiteralNode.hpp"
#include "PatternPathNode.hpp"
#include "PatternSliceNode.hpp"
#include "PatternStructNode.hpp"
#include "PatternTupleNode.hpp"
#include "PatternWildNode.hpp"
#include "StmtEmptyNode.hpp"
#include "StmtExprNode.hpp"
#include "StmtItemNode.hpp"
#include "StmtLetNode.hpp"
#include "TypeArrayNode.hpp"
#include "TypeInferNode.hpp"
#include "TypeNeverNode.hpp"
#include "TypeParentNode.hpp"
#include "TypePathNode.hpp"
#include "TypeSliceNode.hpp"
#include "TypeTupleNode.hpp"
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
auto parsePatternOneNode(TokenStream &stream)
    -> std::unique_ptr<PatternOneNode>;

auto parseTypeNode(TokenStream &stream) -> std::unique_ptr<TypeNode>;

auto parsePathNode(TokenStream &stream) -> std::unique_ptr<PathNode>;
auto parsePathSimpleNode(TokenStream &stream)
    -> std::unique_ptr<PathSimpleNode>;
auto parsePathQualifiedNode(TokenStream &stream)
    -> std::unique_ptr<PathQualifiedNode>;