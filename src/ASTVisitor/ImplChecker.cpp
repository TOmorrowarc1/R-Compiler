#include "ImplChecker.hpp"
#include "ASTNodeTotal.hpp"
#include "ConstEvaluator.hpp"
#include "Symbol.hpp"
#include "TraitDef.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include "exception.hpp"


ImplChecker::ImplChecker(Scope *initial_scope, ConstEvaluator *const_evaluator)
    : current_scope_(initial_scope), const_evaluator_(const_evaluator) {
  const_evaluator_->bindScopePointer(&current_scope_);
}

ImplChecker::~ImplChecker() = default;

void ImplChecker::visit(ASTRootNode *node) {
  for (auto &item : node->items_) {
    item->accept(*this);
  }
}

void ImplChecker::visit(ItemConstNode *node) {
  node->type_->accept(*this);
  node->value_->accept(*this);
}

void ImplChecker::visit(ItemFnNode *node) {
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
  current_scope_->addFunction(node->ID_, fnNodeToFunc(node));
}

void ImplChecker::visit(ItemStructNode *node) {
  for (auto &item : node->fields_) {
    item.type->accept(*this);
  }
}

void ImplChecker::visit(ItemEnumNode *node) {}

void ImplChecker::visit(ItemImplNode *node) {
  if (!is_instance_of<TypePathNode, TypeNode>(node->type_.get())) {
    throw std::runtime_error("Impl type must be a path type");
  }
  const auto type_path = dynamic_cast<const TypePathNode *>(node->type_.get());
  std::string type_name = type_path->path_->getPathIndexName(0);
  // Only record the trait type.
  if (node->trait_name_.empty()) {
    for (auto &iter : node->items_) {
      if (iter.function) {
        iter.function->accept(*this);
      }
      if (iter.constant) {
        iter.constant->accept(*this);
      }
    }
    return;
  }
  auto trait_def = current_scope_->getType(type_name)->getType();
  for (const auto &item : node->items_) {
    if (item.function) {
      item.function->accept(*this);
      auto func = fnNodeToFunc(item.function.get());
      if (item.function->fn_type_ == FnType::Fn) {
        if (type_def->addAssociatedFunction(func->getName(), func)) {
          throw CompilerException("Duplicate associated function name: ",
                                  node->position_);
        }
      } else {
        if (item.function->fn_type_ == FnType::MutMethod) {
          func->setFnType(SymbolFunctionInfo::FnType::MutMethod);
        } else {
          func->setFnType(SymbolFunctionInfo::FnType::Method);
        }
        if (!type_def->addMethod(func->getName(), std::move(func))) {
          throw CompilerException("Duplicate method name: " + func->getName(),
                                  node->position_);
        }
      }
    } else {
      item.constant->accept(*this);
    }
  }
}

void ImplChecker::visit(ItemTraitNode *node) {}

void ImplChecker::visit(ExprArrayNode *node) {
  for (const auto &item : node->elements_) {
    item->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void ImplChecker::visit(ExprArrayIndexNode *node) {
  node->array_->accept(*this);
  node->index_->accept(*this);
}

void ImplChecker::visit(ExprBlockNode *node) {
  current_scope_ = current_scope_->getNextChildScope();
  for (auto &stmt : node->statements_) {
    stmt->accept(*this);
  }
  if (node->return_value_) {
    node->return_value_->accept(*this);
  }
  current_scope_ = current_scope_->getParent();
}

void ImplChecker::visit(ExprBlockConstNode *node) {
  node->block_expr_->accept(*this);
}

void ImplChecker::visit(ExprCallNode *node) {
  node->caller_->accept(*this);
  for (auto &arg : node->arguments_) {
    arg->accept(*this);
  }
}

void ImplChecker::visit(ExprBreakNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void ImplChecker::visit(ExprReturnNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void ImplChecker::visit(ExprContinueNode *node) {}

void ImplChecker::visit(ExprGroupNode *node) { node->expr_->accept(*this); }

void ImplChecker::visit(ExprIfNode *node) {
  node->condition_->accept(*this);
  if (node->then_block_) {
    node->then_block_->accept(*this);
  }
  if (node->else_block_) {
    node->else_block_->accept(*this);
  }
}

void ImplChecker::visit(ExprLiteralIntNode *node) {}

void ImplChecker::visit(ExprLiteralBoolNode *node) {}

void ImplChecker::visit(ExprLiteralCharNode *node) {}

void ImplChecker::visit(ExprLiteralStringNode *node) {}

void ImplChecker::visit(ExprLoopNode *node) { node->loop_body_->accept(*this); }

void ImplChecker::visit(ExprWhileNode *node) {
  node->condition_->accept(*this);
  node->loop_body_->accept(*this);
}

void ImplChecker::visit(ExprOperBinaryNode *node) {
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
}

void ImplChecker::visit(ExprOperUnaryNode *node) {
  node->operand_->accept(*this);
}

void ImplChecker::visit(ExprPathNode *node) {}

void ImplChecker::visit(ExprFieldNode *node) { node->instance_->accept(*this); }

void ImplChecker::visit(ExprMethodNode *node) {
  node->instance_->accept(*this);
  for (auto &param : node->parameters_) {
    param->accept(*this);
  }
}

void ImplChecker::visit(ExprStructNode *node) {
  node->path_->accept(*this);
  for (const auto &field : node->fields_) {
    field.expr->accept(*this);
  }
}

void ImplChecker::visit(ExprUnderScoreNode *node) {}

void ImplChecker::visit(StmtEmptyNode *node) {}

void ImplChecker::visit(StmtItemNode *node) { node->item_->accept(*this); }

void ImplChecker::visit(StmtLetNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->init_value_) {
    node->init_value_->accept(*this);
  }
}

void ImplChecker::visit(StmtExprNode *node) { node->expr_->accept(*this); }

void ImplChecker::visit(PatternLiteralNode *node) {}

void ImplChecker::visit(PatternWildNode *node) {}

void ImplChecker::visit(PatternPathNode *node) {}

void ImplChecker::visit(PatternIDNode *node) {}

void ImplChecker::visit(PatternReferNode *node) {}

void ImplChecker::visit(TypeArrayNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void ImplChecker::visit(TypePathNode *node) {}

void ImplChecker::visit(TypeReferNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
}

void ImplChecker::visit(PathNode *node) {}