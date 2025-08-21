#include "SymbolCollector.hpp"
#include "ASTNodeTotal.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include <stdexcept>

SymbolCollector::SymbolCollector(Scope *initial_scope)
    : current_scope_(initial_scope) {}

SymbolCollector::~SymbolCollector() = default;

auto SymbolCollector::addStructType(const std::string &type_name) -> bool {
  return current_scope_->addType(
      type_name, std::make_shared<SymbolTypeInfo>(
                     type_name, std::make_shared<StructDef>(type_name)));
}

auto SymbolCollector::addEnumType(const std::string &type_name) -> bool {
  return current_scope_->addType(
      type_name, std::make_shared<SymbolTypeInfo>(
                     type_name, std::make_shared<EnumDef>(type_name)));
}

void SymbolCollector::visit(ASTRootNode *node) {
  for (auto &item : node->items_) {
    item->accept(*this);
  }
}

void SymbolCollector::visit(ItemConstNode *node) {
  node->type_->accept(*this);
  node->value_->accept(*this);
}

void SymbolCollector::visit(ItemFnNode *node) {
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

void SymbolCollector::visit(ItemStructNode *node) {
  addStructType(node->ID_);
  for (const auto &field : node->fields_) {
    field.type->accept(*this);
  }
}

void SymbolCollector::visit(ItemEnumNode *node) { addEnumType(node->ID_); }

void SymbolCollector::visit(ItemImplNode *node) {
  for (auto &item : node->items_) {
    if (item.constant) {
      item.constant->accept(*this);
    } else {
      item.function->accept(*this);
    }
  }
}

void SymbolCollector::visit(ItemTraitNode *node) {}

void SymbolCollector::visit(StmtEmptyNode *node) {}

void SymbolCollector::visit(StmtItemNode *node) { node->item_->accept(*this); }

void SymbolCollector::visit(StmtLetNode *node) {
  node->pattern_->accept(*this);
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->init_value_) {
    node->init_value_->accept(*this);
  }
}

void SymbolCollector::visit(StmtExprNode *node) { node->expr_->accept(*this); }

void SymbolCollector::visit(ExprArrayNode *node) {
  for (const auto &item : node->elements_) {
    item->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void SymbolCollector::visit(ExprArrayIndexNode *node) {
  node->array_->accept(*this);
  node->index_->accept(*this);
}

void SymbolCollector::visit(ExprBlockNode *node) {
  current_scope_ = current_scope_->addNextChildScope();
  for (auto &stmt : node->statements_) {
    stmt->accept(*this);
  }
  if (node->return_value_) {
    node->return_value_->accept(*this);
  }
  current_scope_ = current_scope_->getParent();
}

void SymbolCollector::visit(ExprBlockConstNode *node) {
  node->block_expr_->accept(*this);
}

void SymbolCollector::visit(ExprCallNode *node) {
  node->caller_->accept(*this);
  for (auto &arg : node->arguments_) {
    arg->accept(*this);
  }
}

void SymbolCollector::visit(ExprBreakNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void SymbolCollector::visit(ExprReturnNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void SymbolCollector::visit(ExprContinueNode *node) {}

void SymbolCollector::visit(ExprGroupNode *node) { node->expr_->accept(*this); }

void SymbolCollector::visit(ExprIfNode *node) {
  node->condition_->accept(*this);
  if (node->then_block_) {
    node->then_block_->accept(*this);
  }
  if (node->else_block_) {
    node->else_block_->accept(*this);
  }
}

void SymbolCollector::visit(ExprLiteralIntNode *node) {}

void SymbolCollector::visit(ExprLiteralBoolNode *node) {}

void SymbolCollector::visit(ExprLiteralCharNode *node) {}

void SymbolCollector::visit(ExprLiteralStringNode *node) {}

void SymbolCollector::visit(ExprLoopNode *node) {
  node->loop_body_->accept(*this);
}

void SymbolCollector::visit(ExprWhileNode *node) {
  node->condition_->accept(*this);
  node->loop_body_->accept(*this);
}

void SymbolCollector::visit(ExprOperBinaryNode *node) {
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
}

void SymbolCollector::visit(ExprOperUnaryNode *node) {
  node->operand_->accept(*this);
}

void SymbolCollector::visit(ExprPathNode *node) {}

void SymbolCollector::visit(ExprFieldNode *node) {
  node->instance_->accept(*this);
}

void SymbolCollector::visit(ExprMethodNode *node) {
  node->instance_->accept(*this);
  for (auto &param : node->parameters_) {
    param->accept(*this);
  }
}

void SymbolCollector::visit(ExprMatchNode *node) {
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

void SymbolCollector::visit(ExprStructNode *node) {
  node->path_->accept(*this);
  for (const auto &field : node->fields_) {
    field.expr->accept(*this);
  }
}

void SymbolCollector::visit(ExprUnderScoreNode *node) {}

void SymbolCollector::visit(PatternLiteralNode *node) {}

void SymbolCollector::visit(PatternWildNode *node) {}

void SymbolCollector::visit(PatternPathNode *node) {}

void SymbolCollector::visit(PatternIDNode *node) {}

void SymbolCollector::visit(TypeArrayNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void SymbolCollector::visit(TypePathNode *node) {}

void SymbolCollector::visit(PathNode *node) {}