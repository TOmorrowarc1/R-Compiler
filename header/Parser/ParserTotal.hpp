#pragma once
#include "Parser.hpp"
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
#include "ItemConstantNode.hpp"
#include "ItemFnNode.hpp"
#include "StmtEmptyNode.hpp"
#include "StmtExprNode.hpp"
#include "StmtItemNode.hpp"
#include "StmtLetNode.hpp"
#include "TypeInferNode.hpp"
#include "TypeNeverNode.hpp"
#include "TypeParentNode.hpp"
#include "TypePathNode.hpp"
#include "token.hpp"
#include <stdexcept>

auto parse(const std::vector<Token> &tokens) -> std::unique_ptr<ASTRootNode>;
auto parseRootNode(TokenStream &stream) -> std::unique_ptr<ASTRootNode>;

auto parseItemNode(TokenStream &stream) -> std::unique_ptr<ItemNode>;
auto parseItemFnNode(TokenStream &stream) -> std::unique_ptr<ItemNode>;
auto parseFnParameters(TokenStream &stream) -> std::vector<FnParameter>;
auto parseFnParameter(TokenStream &stream) -> FnParameter;
auto parseItemConstNode(TokenStream &stream) -> std::unique_ptr<ItemNode>;

auto parseStmtNode(TokenStream &stream) -> std::unique_ptr<StmtNode>;
auto parseStmtItemNode(TokenStream &stream) -> std::unique_ptr<StmtItemNode>;
auto parseStmtLetNode(TokenStream &stream) -> std::unique_ptr<StmtLetNode>;
auto parseStmtExprNode(TokenStream &stream) -> std::unique_ptr<StmtExprNode>;
auto parseStmtEmptyNode(TokenStream &stream) -> std::unique_ptr<StmtEmptyNode>;

auto parseExprNode(TokenStream &stream) -> std::unique_ptr<ExprNode>;
auto parseExprBlockInNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockInNode>;
auto parseExprBlockOutNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockOutNode>;
auto parseExprBlockNode(TokenStream &stream) -> std::unique_ptr<ExprBlockNode>;
auto parseExprBlockConstNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockConstNode>;
auto parseExprLoopNode(TokenStream &stream) -> std::unique_ptr<ExprLoopNode>;
auto parseExprWhileNode(TokenStream &stream) -> std::unique_ptr<ExprWhileNode>;
auto parseExprIfNode(TokenStream &stream) -> std::unique_ptr<ExprIfNode>;
auto parseExprLiteralNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralNode>;
auto parseExprLiteralIntNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralIntNode>;
auto parseExprLiteralBoolNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralBoolNode>;
auto parseExprLiteralCharNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralCharNode>;
auto parseExprLiteralStringNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralStringNode>;
auto parseExprLiteralByteNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralByteNode>;
auto parseExprOperBinaryNode(TokenStream &stream)
    -> std::unique_ptr<ExprOperBinaryNode>;
auto parseExprOperUnaryNode(TokenStream &stream)
    -> std::unique_ptr<ExprOperUnaryNode>;
auto parseExprGroupNode(TokenStream &stream) -> std::unique_ptr<ExprGroupNode>;
auto parseExprCallNode(TokenStream &stream) -> std::unique_ptr<ExprCallNode>;
auto parseExprBreakNode(TokenStream &stream) -> std::unique_ptr<ExprBreakNode>;
auto parseExprReturnNode(TokenStream &stream)
    -> std::unique_ptr<ExprReturnNode>;
auto parseExprContinueNode(TokenStream &stream)
    -> std::unique_ptr<ExprContinueNode>;

auto parseTypeNode(TokenStream &stream) -> std::unique_ptr<TypeNode>;
auto parseTypeInferNode(TokenStream &stream) -> std::unique_ptr<TypeInferNode>;
auto parseTypeNeverNode(TokenStream &stream) -> std::unique_ptr<TypeNeverNode>;
auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode>;
auto parseTypeParentNode(TokenStream &stream)
    -> std::unique_ptr<TypeParentNode>;