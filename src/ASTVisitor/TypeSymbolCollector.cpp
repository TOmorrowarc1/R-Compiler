#include "TypeSymbolCollector.hpp"
#include "ASTNodeTotal.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include <stdexcept>

TypeSymbolCollector::TypeSymbolCollector(Scope *initial_scope)
    : current_scope_(initial_scope) {}

TypeSymbolCollector::~TypeSymbolCollector() = default;

auto TypeSymbolCollector::addType(const std::string type_name) -> bool {
  return current_scope_->addType(
      type_name, std::make_shared<SymbolTypeInfo>(
                     type_name, std::make_shared<TypeDef>(type_name)));
}

void TypeSymbolCollector::visit(ASTRootNode *node) {
  for (auto &item : node->items_) {
    item->accept(*this);
  }
}

void TypeSymbolCollector::visit(ItemConstNode *node) {
  node->type_->accept(*this);
  node->value_->accept(*this);
}
void TypeSymbolCollector::visit(ItemFnNode *node) {
  for (auto &param : node->parameters_) {
    if (param.type) {
      param.type->accept(*this);
    } else {
      param.pattern->accept(*this);
    }
  }
  if (node->return_type_) {
    node->return_type_->accept(*this);
  }
  if (node->body_) {
    node->body_->accept(*this);
  }
}
void TypeSymbolCollector::visit(ItemStructNode *node) {
  addType(node->ID_);
  for (const auto &field : node->fields_) {
    field.type->accept(*this);
  }
}
void TypeSymbolCollector::visit(ItemEnumNode *node) { addType(node->ID_); }
void TypeSymbolCollector::visit(ItemImplNode *node) {
  for (auto &item : node->items_) {
    if (item.constant) {
      item.constant->accept(*this);
    } else {
      item.function->accept(*this);
    }
  }
}
void TypeSymbolCollector::visit(ItemTraitNode *node) {}

void TypeSymbolCollector::visit(StmtEmptyNode *node) {}
void TypeSymbolCollector::visit(StmtItemNode *node) {
  node->item_->accept(*this);
}
void TypeSymbolCollector::visit(StmtLetNode *node) {
  node->pattern_->accept(*this);
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->init_value_) {
    node->init_value_->accept(*this);
  }
}
void TypeSymbolCollector::visit(StmtExprNode *node) {
  node->expr_->accept(*this);
}

void TypeSymbolCollector::visit(ExprArrayNode *node) {
  for (const auto &item : node->elements_) {
    item->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}
void TypeSymbolCollector::visit(ExprArrayIndexNode *node) {
  node->array_->accept(*this);
  node->index_->accept(*this);
}
void TypeSymbolCollector::visit(ExprBlockNode *node) {
  current_scope_ = current_scope_->addNextChildScope();
  for (auto &stmt : node->statements_) {
    stmt->accept(*this);
  }
  if (node->return_value_) {
    node->return_value_->accept(*this);
  }
  current_scope_ = current_scope_->getParent();
}
void TypeSymbolCollector::visit(ExprBlockConstNode *node) {
  node->block_expr_->accept(*this);
}
void TypeSymbolCollector::visit(ExprCallNode *node) {
  node->caller_->accept(*this);
  for (auto &arg : node->arguments_) {
    arg->accept(*this);
  }
}
void TypeSymbolCollector::visit(ExprBreakNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}
void TypeSymbolCollector::visit(ExprReturnNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}
void TypeSymbolCollector::visit(ExprContinueNode *node) {}
void TypeSymbolCollector::visit(ExprGroupNode *node) {
  node->expr_->accept(*this);
}
void TypeSymbolCollector::visit(ExprIfNode *node) {
  node->condition_->accept(*this);
  if (node->then_block_) {
    node->then_block_->accept(*this);
  }
  if (node->else_block_) {
    node->else_block_->accept(*this);
  }
}
void TypeSymbolCollector::visit(ExprLiteralIntNode *node) {}
void TypeSymbolCollector::visit(ExprLiteralBoolNode *node) {}
void TypeSymbolCollector::visit(ExprLiteralCharNode *node) {}
void TypeSymbolCollector::visit(ExprLiteralStringNode *node) {}
void TypeSymbolCollector::visit(ExprLoopNode *node) {
  node->loop_body_->accept(*this);
}
void TypeSymbolCollector::visit(ExprWhileNode *node) {
  node->condition_->accept(*this);
  node->loop_body_->accept(*this);
}
void TypeSymbolCollector::visit(ExprOperBinaryNode *node) {
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
}
void TypeSymbolCollector::visit(ExprOperUnaryNode *node) {
  node->operand_->accept(*this);
}
void TypeSymbolCollector::visit(ExprPathNode *node) {}
void TypeSymbolCollector::visit(ExprFieldNode *node) {
  node->instance_->accept(*this);
}
void TypeSymbolCollector::visit(ExprMethodNode *node) {
  node->instance_->accept(*this);
  for (auto &param : node->parameters_) {
    param->accept(*this);
  }
}
void TypeSymbolCollector::visit(ExprMatchNode *node) {
  node->subject_->accept(*this);
  for (auto &arm : node->arms_) {
    arm.pattern->accept(*this);
    if (arm.guard) {
      arm.guard->accept(*this);
    }
    if (arm.body) {
      arm.body->accept(*this);
    }
  }
}
void TypeSymbolCollector::visit(ExprStructNode *node) {
  node->path_->accept(*this);
  for (const auto &field : node->fields_) {
    field.expr->accept(*this);
  }
}
void TypeSymbolCollector::visit(ExprTupleNode *node) {
  for (auto &element : node->elements_) {
    element->accept(*this);
  }
}
void TypeSymbolCollector::visit(ExprTupleIndexNode *node) {
  node->tuple_->accept(*this);
  node->index_->accept(*this);
}

void TypeSymbolCollector::visit(PatternLiteralNode *node) {}
void TypeSymbolCollector::visit(PatternStructNode *node) {}
void TypeSymbolCollector::visit(PatternTupleNode *node) {}
void TypeSymbolCollector::visit(PatternWildNode *node) {}
void TypeSymbolCollector::visit(PatternPathNode *node) {}
void TypeSymbolCollector::visit(PatternIDNode *node) {}

void TypeSymbolCollector::visit(TypeArrayNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}
void TypeSymbolCollector::visit(TypeSliceNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
}
void TypeSymbolCollector::visit(TypePathNode *node) {}

void TypeSymbolCollector::visit(PathNode *node) {}