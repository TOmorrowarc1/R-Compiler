#pragma once
#include "ASTNodeTotal.hpp"
#include "Token.hpp"

/*
Private interfaces of modules of the Parser.
*/

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