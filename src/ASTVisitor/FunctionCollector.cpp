#include "FunctionCollector.hpp"
#include "ASTNodeTotal.hpp"
#include "ConstEvaluator.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include "exception.hpp"

FunctionCollector::FunctionCollector(Scope *initial_scope,
                                     ConstEvaluator *const_evaluator)
    : current_scope_(initial_scope), const_evaluator_(const_evaluator) {
  const_evaluator_->bindScopePointer(current_scope_);
}

FunctionCollector::~FunctionCollector() = default;

// Convert a function node to a new-created symbol function info.
auto FunctionCollector::fnNodeToFunc(const ItemFnNode *node)
    -> std::shared_ptr<SymbolFunctionInfo> {
  std::vector<std::shared_ptr<TypeKind>> parameters;
  auto return_type = const_evaluator_->evaluateType(node->return_type_.get());
  for (const auto &param : node->parameters_) {
    parameters.push_back(const_evaluator_->evaluateType(param.type.get()));
  }
  return std::make_shared<SymbolFunctionInfo>(node->ID_, return_type,
                                              std::move(parameters));
}

void FunctionCollector::visit(ASTRootNode *node) {
  for (auto &item : node->items_) {
    item->accept(*this);
  }
}

void FunctionCollector::visit(ItemConstNode *node) {
  node->type_->accept(*this);
  node->value_->accept(*this);
}

void FunctionCollector::visit(ItemFnNode *node) {
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

// Visit a struct item node and collect its fields.
void FunctionCollector::visit(ItemStructNode *node) {}

void FunctionCollector::visit(ItemEnumNode *node) {}

void FunctionCollector::visit(ItemImplNode *node) {
  if (!is_instance_of<TypePathNode, TypeNode>(node->type_.get())) {
    throw std::runtime_error("Impl type must be a path type");
  }
  const auto type_path = dynamic_cast<const TypePathNode *>(node->type_.get());
  std::string type_name = type_path->path_->getPathIndexName(0);
  auto type_def = current_scope_->getType(type_name)->getType();
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

void FunctionCollector::visit(ItemTraitNode *node) {}

void FunctionCollector::visit(ExprArrayNode *node) {
  for (const auto &item : node->elements_) {
    item->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void FunctionCollector::visit(ExprArrayIndexNode *node) {
  node->array_->accept(*this);
  node->index_->accept(*this);
}

void FunctionCollector::visit(ExprBlockNode *node) {
  current_scope_ = current_scope_->getNextChildScope();
  for (auto &stmt : node->statements_) {
    stmt->accept(*this);
  }
  if (node->return_value_) {
    node->return_value_->accept(*this);
  }
  current_scope_ = current_scope_->getParent();
}

void FunctionCollector::visit(ExprBlockConstNode *node) {
  node->block_expr_->accept(*this);
}

void FunctionCollector::visit(ExprCallNode *node) {
  node->caller_->accept(*this);
  for (auto &arg : node->arguments_) {
    arg->accept(*this);
  }
}

void FunctionCollector::visit(ExprBreakNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void FunctionCollector::visit(ExprReturnNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void FunctionCollector::visit(ExprContinueNode *node) {}

void FunctionCollector::visit(ExprGroupNode *node) {
  node->expr_->accept(*this);
}

void FunctionCollector::visit(ExprIfNode *node) {
  node->condition_->accept(*this);
  if (node->then_block_) {
    node->then_block_->accept(*this);
  }
  if (node->else_block_) {
    node->else_block_->accept(*this);
  }
}

void FunctionCollector::visit(ExprLiteralIntNode *node) {}

void FunctionCollector::visit(ExprLiteralBoolNode *node) {}

void FunctionCollector::visit(ExprLiteralCharNode *node) {}

void FunctionCollector::visit(ExprLiteralStringNode *node) {}

void FunctionCollector::visit(ExprLoopNode *node) {
  node->loop_body_->accept(*this);
}

void FunctionCollector::visit(ExprWhileNode *node) {
  node->condition_->accept(*this);
  node->loop_body_->accept(*this);
}

void FunctionCollector::visit(ExprOperBinaryNode *node) {
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
}

void FunctionCollector::visit(ExprOperUnaryNode *node) {
  node->operand_->accept(*this);
}

void FunctionCollector::visit(ExprPathNode *node) {}

void FunctionCollector::visit(ExprFieldNode *node) {
  node->instance_->accept(*this);
}

void FunctionCollector::visit(ExprMethodNode *node) {
  node->instance_->accept(*this);
  for (auto &param : node->parameters_) {
    param->accept(*this);
  }
}

void FunctionCollector::visit(ExprStructNode *node) {
  node->path_->accept(*this);
  for (const auto &field : node->fields_) {
    field.expr->accept(*this);
  }
}

void FunctionCollector::visit(ExprUnderScoreNode *node) {}

void FunctionCollector::visit(StmtEmptyNode *node) {}

void FunctionCollector::visit(StmtItemNode *node) {
  node->item_->accept(*this);
}

void FunctionCollector::visit(StmtLetNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->init_value_) {
    node->init_value_->accept(*this);
  }
}

void FunctionCollector::visit(StmtExprNode *node) {
  node->expr_->accept(*this);
}

void FunctionCollector::visit(PatternLiteralNode *node) {}

void FunctionCollector::visit(PatternWildNode *node) {}

void FunctionCollector::visit(PatternPathNode *node) {}

void FunctionCollector::visit(PatternIDNode *node) {}

void FunctionCollector::visit(PatternReferNode *node) {}

void FunctionCollector::visit(TypeArrayNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void FunctionCollector::visit(TypePathNode *node) {}

void FunctionCollector::visit(TypeReferNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
}

void FunctionCollector::visit(PathNode *node) {}