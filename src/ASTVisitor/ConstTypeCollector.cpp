#include "ConstTypeCollector.hpp"
#include "ASTNodeTotal.hpp"
#include "ConstEvaluator.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include "exception.hpp"

ConstTypeCollector::ConstTypeCollector(Scope *initial_scope,
                                       ConstEvaluator *const_evaluator)
    : current_scope_(initial_scope), const_evaluator_(const_evaluator) {
  const_evaluator_->bindScopePointer(current_scope_);
}

ConstTypeCollector::~ConstTypeCollector() = default;

auto ConstTypeCollector::addStructType(const std::string &type_name) -> bool {
  auto new_struct_type = std::make_shared<StructDef>(type_name);
  auto new_symbol_type =
      std::make_shared<SymbolTypeInfo>(type_name, new_struct_type);
  return current_scope_->addType(type_name, new_symbol_type);
}

auto ConstTypeCollector::addEnumType(const std::string &type_name) -> bool {
  auto new_enum_type = std::make_shared<EnumDef>(type_name);
  auto new_symbol_type =
      std::make_shared<SymbolTypeInfo>(type_name, new_enum_type);
  return current_scope_->addType(type_name, new_symbol_type);
}

auto ConstTypeCollector::addConstSymbol(const std::string &name) -> bool {
  auto const_info = std::make_shared<SymbolConstInfo>(name);
  if (impl_type_name_.empty()) {
    return current_scope_->addConst(name, const_info);
  }
  auto type_def = current_scope_->getType(impl_type_name_)->getType();
  return type_def->addConst(name, const_info);
}

void ConstTypeCollector::visit(ASTRootNode *node) {
  for (auto &item : node->items_) {
    item->accept(*this);
  }
}

void ConstTypeCollector::visit(ItemConstNode *node) {
  node->type_->accept(*this);
  node->value_->accept(*this);
  if (!addConstSymbol(node->ID_)) {
    throw CompilerException("Duplicate constant name: " + node->ID_,
                            node->position_);
  }
  if (impl_type_name_.empty()) {
    const_evaluator_->attachNodeToConst(node, node->ID_);
  } else {
    const_evaluator_->attachNodeToStructConst(node, impl_type_name_, node->ID_);
  }
}

void ConstTypeCollector::visit(ItemFnNode *node) {
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
    current_scope_ = current_scope_->addNextChildScope();
    node->body_->accept(*this);
    current_scope_ = current_scope_->getParent();
  }
}

void ConstTypeCollector::visit(ItemStructNode *node) {
  addStructType(node->ID_);
  const_evaluator_->attachNodeToTypeDef(node, node->ID_);
  for (const auto &field : node->fields_) {
    field.type->accept(*this);
  }
}

void ConstTypeCollector::visit(ItemEnumNode *node) {
  addEnumType(node->ID_);
  const_evaluator_->attachNodeToTypeDef(node, node->ID_);
}

void ConstTypeCollector::visit(ItemImplNode *node) {
  auto type_path = dynamic_cast<TypePathNode *>(node->type_.get());
  if (type_path == nullptr) {
    throw CompilerException("Impl type must be a path type", node->position_);
  }
  auto type_name = type_path->path_->getPathIndexName(0);
  impl_type_name_ = type_name;
  for (auto &item : node->items_) {
    if (item.constant) {
      item.constant->accept(*this);
    } else {
      item.function->accept(*this);
    }
  }
  impl_type_name_.clear();
}

void ConstTypeCollector::visit(ItemTraitNode *node) {}

void ConstTypeCollector::visit(StmtEmptyNode *node) {}

void ConstTypeCollector::visit(StmtItemNode *node) {
  node->item_->accept(*this);
}

void ConstTypeCollector::visit(StmtLetNode *node) {
  node->pattern_->accept(*this);
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->init_value_) {
    node->init_value_->accept(*this);
  }
}

void ConstTypeCollector::visit(StmtExprNode *node) {
  node->expr_->accept(*this);
}

void ConstTypeCollector::visit(ExprArrayNode *node) {
  for (const auto &item : node->elements_) {
    item->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void ConstTypeCollector::visit(ExprArrayIndexNode *node) {
  node->array_->accept(*this);
  node->index_->accept(*this);
}

void ConstTypeCollector::visit(ExprBlockNode *node) {
  current_scope_ = current_scope_->addNextChildScope();
  for (auto &stmt : node->statements_) {
    stmt->accept(*this);
  }
  if (node->return_value_) {
    node->return_value_->accept(*this);
  }
  current_scope_ = current_scope_->getParent();
}

void ConstTypeCollector::visit(ExprBlockConstNode *node) {
  node->block_expr_->accept(*this);
}

void ConstTypeCollector::visit(ExprCallNode *node) {
  node->caller_->accept(*this);
  for (auto &arg : node->arguments_) {
    arg->accept(*this);
  }
}

void ConstTypeCollector::visit(ExprBreakNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void ConstTypeCollector::visit(ExprReturnNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void ConstTypeCollector::visit(ExprContinueNode *node) {}

void ConstTypeCollector::visit(ExprGroupNode *node) {
  node->expr_->accept(*this);
}

void ConstTypeCollector::visit(ExprIfNode *node) {
  node->condition_->accept(*this);
  if (node->then_block_) {
    node->then_block_->accept(*this);
  }
  if (node->else_block_) {
    node->else_block_->accept(*this);
  }
}

void ConstTypeCollector::visit(ExprLiteralIntNode *node) {}

void ConstTypeCollector::visit(ExprLiteralBoolNode *node) {}

void ConstTypeCollector::visit(ExprLiteralCharNode *node) {}

void ConstTypeCollector::visit(ExprLiteralStringNode *node) {}

void ConstTypeCollector::visit(ExprLoopNode *node) {
  node->loop_body_->accept(*this);
}

void ConstTypeCollector::visit(ExprWhileNode *node) {
  node->condition_->accept(*this);
  node->loop_body_->accept(*this);
}

void ConstTypeCollector::visit(ExprOperBinaryNode *node) {
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
}

void ConstTypeCollector::visit(ExprOperUnaryNode *node) {
  node->operand_->accept(*this);
}

void ConstTypeCollector::visit(ExprPathNode *node) {}

void ConstTypeCollector::visit(ExprFieldNode *node) {
  node->instance_->accept(*this);
}

void ConstTypeCollector::visit(ExprMethodNode *node) {
  node->instance_->accept(*this);
  for (auto &param : node->parameters_) {
    param->accept(*this);
  }
}

void ConstTypeCollector::visit(ExprStructNode *node) {
  node->path_->accept(*this);
  for (const auto &field : node->fields_) {
    field.expr->accept(*this);
  }
}

void ConstTypeCollector::visit(ExprUnderScoreNode *node) {}

void ConstTypeCollector::visit(PatternLiteralNode *node) {}

void ConstTypeCollector::visit(PatternWildNode *node) {}

void ConstTypeCollector::visit(PatternPathNode *node) {}

void ConstTypeCollector::visit(PatternIDNode *node) {}

void ConstTypeCollector::visit(PatternReferNode *node) {}

void ConstTypeCollector::visit(TypeArrayNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void ConstTypeCollector::visit(TypePathNode *node) {}

void ConstTypeCollector::visit(TypeReferNode *node) {
  node->type_->accept(*this);
}

void ConstTypeCollector::visit(PathNode *node) {}