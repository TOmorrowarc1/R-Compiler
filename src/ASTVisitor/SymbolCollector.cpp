#include "SymbolCollector.hpp"
#include "ASTNodeTotal.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"

SymbolCollector::SymbolCollector(std::shared_ptr<Scope> initial_scope)
    : current_scope_(std::move(initial_scope)) {}
SymbolCollector::~SymbolCollector() = default;

void SymbolCollector::visit(ASTRootNode &node) {
  for (auto &item : node.items_) {
    item->accept(*this);
  } 
}

void SymbolCollector::visit(ItemConstNode &node) {}
void SymbolCollector::visit(ItemFnNode &node) {}
void SymbolCollector::visit(ItemStructNode &node) {}
void SymbolCollector::visit(ItemEnumNode &node) {}
void SymbolCollector::visit(ItemImplNode &node) {}
void SymbolCollector::visit(ItemTraitNode &node) {}

void SymbolCollector::visit(ExprArrayNode &node) {}
void SymbolCollector::visit(ExprArrayIndexNode &node) {}
void SymbolCollector::visit(ExprBlockNode &node) {}
void SymbolCollector::visit(ExprBlockConstNode &node) {}
void SymbolCollector::visit(ExprCallNode &node) {}
void SymbolCollector::visit(ExprBreakNode &node) {}
void SymbolCollector::visit(ExprReturnNode &node) {}
void SymbolCollector::visit(ExprContinueNode &node) {}
void SymbolCollector::visit(ExprGroupNode &node) {}
void SymbolCollector::visit(ExprIfNode &node) {}
void SymbolCollector::visit(ExprLiteralIntNode &node) {}
void SymbolCollector::visit(ExprLiteralBoolNode &node) {}
void SymbolCollector::visit(ExprLiteralCharNode &node) {}
void SymbolCollector::visit(ExprLiteralStringNode &node) {}
void SymbolCollector::visit(ExprLiteralByteNode &node) {}
void SymbolCollector::visit(ExprLoopNode &node) {}
void SymbolCollector::visit(ExprWhileNode &node) {}
void SymbolCollector::visit(ExprOperBinaryNode &node) {}
void SymbolCollector::visit(ExprOperUnaryNode &node) {}
void SymbolCollector::visit(ExprPathNode &node) {}
void SymbolCollector::visit(ExprFieldNode &node) {}
void SymbolCollector::visit(ExprMethodNode &node) {}
void SymbolCollector::visit(ExprMatchNode &node) {}
void SymbolCollector::visit(ExprStructNode &node) {}
void SymbolCollector::visit(ExprTupleNode &node) {}
void SymbolCollector::visit(ExprTupleIndexNode &node) {}

void SymbolCollector::visit(StmtExprNode &node) {}
void SymbolCollector::visit(StmtEmptyNode &node) {}
void SymbolCollector::visit(StmtItemNode &node) {}
void SymbolCollector::visit(StmtLetNode &node) {}

void SymbolCollector::visit(PatternLiteralNode &node) {}
void SymbolCollector::visit(PatternStructNode &node) {}
void SymbolCollector::visit(PatternTupleNode &node) {}
void SymbolCollector::visit(PatternWildNode &node) {}
void SymbolCollector::visit(PatternPathNode &node) {}
void SymbolCollector::visit(PatternIDNode &node) {}

void SymbolCollector::visit(TypeArrayNode &node) {}
void SymbolCollector::visit(TypeSliceNode &node) {}
void SymbolCollector::visit(TypeInferNode &node) {}
void SymbolCollector::visit(TypePathNode &node) {}

void SymbolCollector::visit(PathNode &node) {}