#include "ConstTypeEvaluator.hpp"
#include "ASTNodeTotal.hpp"
#include "ConstEvaluator.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include "exception.hpp"

ConstTypeEvaluator::ConstTypeEvaluator(Scope *initial_scope,
                                       ConstEvaluator *const_evaluator)
    : current_scope_(initial_scope), const_evaluator_(const_evaluator) {
  const_evaluator_->bindScopePointer(current_scope_);
}

ConstTypeEvaluator::~ConstTypeEvaluator() = default;

void ConstTypeEvaluator::visit(ASTRootNode *node) {
  for (auto &item : node->items_) {
    item->accept(*this);
  }
}

void ConstTypeEvaluator::visit(ItemConstNode *node) {
  node->type_->accept(*this);
  node->value_->accept(*this);
  const_evaluator_->evaluateConstSymbol(node->ID_);
}

void ConstTypeEvaluator::visit(ItemFnNode *node) {
  for (const auto &param : node->parameters_) {
    param.type->accept(*this);
  }
  if (node->return_type_) {
    node->return_type_->accept(*this);
  }
  if (node->body_) {
    current_scope_ = current_scope_->getNextChildScope();
    node->body_->accept(*this);
    current_scope_ = current_scope_->getParent();
  }
}

// Visit a struct item node and collect its fields.
void ConstTypeEvaluator::visit(ItemStructNode *node) {
  const_evaluator_->evaluateTypeSymbol(node->ID_);
}

void ConstTypeEvaluator::visit(ItemEnumNode *node) {
  const_evaluator_->evaluateTypeSymbol(node->ID_);
}

void ConstTypeEvaluator::visit(ItemImplNode *node) {
  if (!is_instance_of<TypePathNode, TypeNode>(node->type_.get())) {
    throw std::runtime_error("Impl type must be a path type");
  }
  const auto type_path = dynamic_cast<const TypePathNode *>(node->type_.get());
  std::string type_name = type_path->path_->getPathIndexName(0);
  auto type_def = current_scope_->getType(type_name)->getType();
  for (const auto &item : node->items_) {
    if (item.function) {
      item.function->accept(*this);
    } else {
      item.constant->accept(*this);
      const_evaluator_->evaluateStructConst(type_name, item.constant->ID_);
    }
  }
}

void ConstTypeEvaluator::visit(ItemTraitNode *node) {}

void ConstTypeEvaluator::visit(ExprArrayNode *node) {
  for (const auto &item : node->elements_) {
    item->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void ConstTypeEvaluator::visit(ExprArrayIndexNode *node) {
  node->array_->accept(*this);
  node->index_->accept(*this);
}

void ConstTypeEvaluator::visit(ExprBlockNode *node) {
  current_scope_ = current_scope_->getNextChildScope();
  for (auto &stmt : node->statements_) {
    stmt->accept(*this);
  }
  if (node->return_value_) {
    node->return_value_->accept(*this);
  }
  current_scope_ = current_scope_->getParent();
}

void ConstTypeEvaluator::visit(ExprBlockConstNode *node) {
  node->block_expr_->accept(*this);
}

void ConstTypeEvaluator::visit(ExprCallNode *node) {
  node->caller_->accept(*this);
  for (auto &arg : node->arguments_) {
    arg->accept(*this);
  }
}

void ConstTypeEvaluator::visit(ExprBreakNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void ConstTypeEvaluator::visit(ExprReturnNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void ConstTypeEvaluator::visit(ExprContinueNode *node) {}

void ConstTypeEvaluator::visit(ExprGroupNode *node) {
  node->expr_->accept(*this);
}

void ConstTypeEvaluator::visit(ExprIfNode *node) {
  node->condition_->accept(*this);
  if (node->then_block_) {
    node->then_block_->accept(*this);
  }
  if (node->else_block_) {
    node->else_block_->accept(*this);
  }
}

void ConstTypeEvaluator::visit(ExprLiteralIntNode *node) {}

void ConstTypeEvaluator::visit(ExprLiteralBoolNode *node) {}

void ConstTypeEvaluator::visit(ExprLiteralCharNode *node) {}

void ConstTypeEvaluator::visit(ExprLiteralStringNode *node) {}

void ConstTypeEvaluator::visit(ExprLoopNode *node) {
  node->loop_body_->accept(*this);
}

void ConstTypeEvaluator::visit(ExprWhileNode *node) {
  node->condition_->accept(*this);
  node->loop_body_->accept(*this);
}

void ConstTypeEvaluator::visit(ExprOperBinaryNode *node) {
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
}

void ConstTypeEvaluator::visit(ExprOperUnaryNode *node) {
  node->operand_->accept(*this);
}

void ConstTypeEvaluator::visit(ExprPathNode *node) {}

void ConstTypeEvaluator::visit(ExprFieldNode *node) {
  node->instance_->accept(*this);
}

void ConstTypeEvaluator::visit(ExprMethodNode *node) {
  node->instance_->accept(*this);
  for (auto &param : node->parameters_) {
    param->accept(*this);
  }
}

void ConstTypeEvaluator::visit(ExprStructNode *node) {
  node->path_->accept(*this);
  for (const auto &field : node->fields_) {
    field.expr->accept(*this);
  }
}

void ConstTypeEvaluator::visit(ExprUnderScoreNode *node) {}

void ConstTypeEvaluator::visit(StmtEmptyNode *node) {}

void ConstTypeEvaluator::visit(StmtItemNode *node) {
  node->item_->accept(*this);
}

void ConstTypeEvaluator::visit(StmtLetNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->init_value_) {
    node->init_value_->accept(*this);
  }
}

void ConstTypeEvaluator::visit(StmtExprNode *node) {
  node->expr_->accept(*this);
}

void ConstTypeEvaluator::visit(PatternLiteralNode *node) {}

void ConstTypeEvaluator::visit(PatternWildNode *node) {}

void ConstTypeEvaluator::visit(PatternPathNode *node) {}

void ConstTypeEvaluator::visit(PatternIDNode *node) {}

void ConstTypeEvaluator::visit(PatternReferNode *node) {}

void ConstTypeEvaluator::visit(TypeArrayNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void ConstTypeEvaluator::visit(TypePathNode *node) {}

void ConstTypeEvaluator::visit(TypeReferNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
}

void ConstTypeEvaluator::visit(PathNode *node) {}