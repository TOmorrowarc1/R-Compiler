#include "FuncTraitCollector.hpp"
#include "ASTNodeTotal.hpp"
#include "ConstEvaluator.hpp"
#include "Symbol.hpp"
#include "TraitDef.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include "exception.hpp"

FuncTraitCollector::FuncTraitCollector(Scope *initial_scope,
                                       ConstEvaluator *const_evaluator)
    : current_scope_(initial_scope), const_evaluator_(const_evaluator) {
  const_evaluator_->bindScopePointer(&current_scope_);
}

FuncTraitCollector::~FuncTraitCollector() = default;

// Convert a function node to a new-created symbol function info.
auto FuncTraitCollector::fnNodeToFunc(const ItemFnNode *node)
    -> std::shared_ptr<SymbolFunctionInfo> {
  std::vector<std::shared_ptr<TypeKind>> parameters;
  auto return_type = const_evaluator_->evaluateType(node->return_type_.get());
  for (const auto &param : node->parameters_) {
    parameters.push_back(const_evaluator_->evaluateType(param.type.get()));
  }
  auto result = std::make_shared<SymbolFunctionInfo>(node->ID_, return_type,
                                                     std::move(parameters));
  if (node->fn_type_ == FnType::MutMethod) {
    result->setFnType(SymbolFunctionInfo::FnType::MutMethod);
  } else {
    result->setFnType(SymbolFunctionInfo::FnType::Method);
  }
  return result;
}

void FuncTraitCollector::visit(ASTRootNode *node) {
  for (auto &item : node->items_) {
    item->accept(*this);
  }
}

void FuncTraitCollector::visit(ItemConstNode *node) {
  node->type_->accept(*this);
  node->value_->accept(*this);
}

void FuncTraitCollector::visit(ItemFnNode *node) {
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
  auto func_info = fnNodeToFunc(node);
  if (ctx_name_.empty()) {
    current_scope_->addFunction(node->ID_, fnNodeToFunc(node));
  } else {
    std::string name = ctx_name_.top();
    if (context_.top() == ContextType::IN_TYPE_DEF) {
      auto type_def = current_scope_->getType(name)->getType();
      if (node->fn_type_ == FnType::Fn) {
        if (!type_def->addAssociatedFunction(node->ID_, func_info)) {
          throw CompilerException("Duplicate associated function name: ",
                                  node->position_);
        }
      } else {
        if (!type_def->addMethod(node->ID_, func_info)) {
          throw CompilerException("Duplicate method name: " + node->ID_,
                                  node->position_);
        }
      }
    } else {
      auto trait_def = current_scope_->getTrait(name)->getTrait();
      if (node->fn_type_ == FnType::Fn) {
        if (!trait_def->addFunction(node->ID_, func_info)) {
          throw CompilerException(
              "Duplicate function name in trait:" + node->ID_, node->position_);
        }
      } else {
        if (!trait_def->addMethod(node->ID_, func_info)) {
          throw CompilerException(
              "Duplicate method name in trait: " + node->ID_, node->position_);
        }
      }
    }
  }
}

void FuncTraitCollector::visit(ItemStructNode *node) {
  for (auto &item : node->fields_) {
    item.type->accept(*this);
  }
}

void FuncTraitCollector::visit(ItemEnumNode *node) {}

void FuncTraitCollector::visit(ItemImplNode *node) {
  if (!is_instance_of<TypePathNode, TypeNode>(node->type_.get())) {
    throw std::runtime_error("Impl type must be a path type");
  }
  const auto type_path = dynamic_cast<const TypePathNode *>(node->type_.get());
  ctx_name_.push(type_path->path_->getPathIndexName(0));
  context_.push(ContextType::IN_TYPE_DEF);
  for (const auto &item : node->items_) {
    if (item.function) {
      item.function->accept(*this);
    } else {
      item.constant->accept(*this);
    }
  }
  context_.pop();
  ctx_name_.pop();
}

void FuncTraitCollector::visit(ItemTraitNode *node) {
  ctx_name_.push(node->trait_name_);
  context_.push(ContextType::IN_TRAIT_DEF);
  for (const auto &item : node->items_) {
    if (item.function) {
      item.function->accept(*this);
    } else {
      item.constant->accept(*this);
    }
  }
  context_.pop();
  ctx_name_.pop();
}

void FuncTraitCollector::visit(ExprArrayNode *node) {
  for (const auto &item : node->elements_) {
    item->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void FuncTraitCollector::visit(ExprArrayIndexNode *node) {
  node->array_->accept(*this);
  node->index_->accept(*this);
}

void FuncTraitCollector::visit(ExprBlockNode *node) {
  current_scope_ = current_scope_->getNextChildScope();
  for (auto &stmt : node->statements_) {
    stmt->accept(*this);
  }
  if (node->return_value_) {
    node->return_value_->accept(*this);
  }
  current_scope_ = current_scope_->getParent();
}

void FuncTraitCollector::visit(ExprBlockConstNode *node) {
  node->block_expr_->accept(*this);
}

void FuncTraitCollector::visit(ExprCallNode *node) {
  node->caller_->accept(*this);
  for (auto &arg : node->arguments_) {
    arg->accept(*this);
  }
}

void FuncTraitCollector::visit(ExprBreakNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void FuncTraitCollector::visit(ExprReturnNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void FuncTraitCollector::visit(ExprContinueNode *node) {}

void FuncTraitCollector::visit(ExprGroupNode *node) {
  node->expr_->accept(*this);
}

void FuncTraitCollector::visit(ExprIfNode *node) {
  node->condition_->accept(*this);
  if (node->then_block_) {
    node->then_block_->accept(*this);
  }
  if (node->else_block_) {
    node->else_block_->accept(*this);
  }
}

void FuncTraitCollector::visit(ExprLiteralIntNode *node) {}

void FuncTraitCollector::visit(ExprLiteralBoolNode *node) {}

void FuncTraitCollector::visit(ExprLiteralCharNode *node) {}

void FuncTraitCollector::visit(ExprLiteralStringNode *node) {}

void FuncTraitCollector::visit(ExprLoopNode *node) {
  node->loop_body_->accept(*this);
}

void FuncTraitCollector::visit(ExprWhileNode *node) {
  node->condition_->accept(*this);
  node->loop_body_->accept(*this);
}

void FuncTraitCollector::visit(ExprOperBinaryNode *node) {
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
}

void FuncTraitCollector::visit(ExprOperUnaryNode *node) {
  node->operand_->accept(*this);
}

void FuncTraitCollector::visit(ExprPathNode *node) {}

void FuncTraitCollector::visit(ExprFieldNode *node) {
  node->instance_->accept(*this);
}

void FuncTraitCollector::visit(ExprMethodNode *node) {
  node->instance_->accept(*this);
  for (auto &param : node->parameters_) {
    param->accept(*this);
  }
}

void FuncTraitCollector::visit(ExprStructNode *node) {
  node->path_->accept(*this);
  for (const auto &field : node->fields_) {
    field.expr->accept(*this);
  }
}

void FuncTraitCollector::visit(ExprUnderScoreNode *node) {}

void FuncTraitCollector::visit(StmtEmptyNode *node) {}

void FuncTraitCollector::visit(StmtItemNode *node) {
  node->item_->accept(*this);
}

void FuncTraitCollector::visit(StmtLetNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->init_value_) {
    node->init_value_->accept(*this);
  }
}

void FuncTraitCollector::visit(StmtExprNode *node) {
  node->expr_->accept(*this);
}

void FuncTraitCollector::visit(PatternLiteralNode *node) {}

void FuncTraitCollector::visit(PatternWildNode *node) {}

void FuncTraitCollector::visit(PatternPathNode *node) {}

void FuncTraitCollector::visit(PatternIDNode *node) {}

void FuncTraitCollector::visit(PatternReferNode *node) {}

void FuncTraitCollector::visit(TypeArrayNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void FuncTraitCollector::visit(TypePathNode *node) {}

void FuncTraitCollector::visit(TypeReferNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
}

void FuncTraitCollector::visit(PathNode *node) {}