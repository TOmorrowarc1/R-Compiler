#include "SymbolAnnotator.hpp"
#include "ASTNodeTotal.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include "exception.hpp"

SymbolAnnotator::SymbolAnnotator(Scope *initial_scope)
    : current_scope_(initial_scope) {}

SymbolAnnotator::~SymbolAnnotator() = default;

auto SymbolAnnotator::typeNodeToType(const TypeNode *type_node)
    -> std::shared_ptr<TypeKind> {
  if (type_node == nullptr) {
    return std::make_shared<TypeKindPath>(
        current_scope_->getType("unit")->getType());
  }
  if (is_instance_of<TypePathNode, TypeNode>(type_node)) {
    const auto *type_path = dynamic_cast<const TypePathNode *>(type_node);
    std::string type_name = getPathIndexName(type_path->path_.get(), 0);
    return std::make_shared<TypeKindPath>(
        current_scope_->getType(type_name)->getType());
  }
  if (is_instance_of<TypeArrayNode, TypeNode>(type_node)) {
    const auto *type_array = dynamic_cast<const TypeArrayNode *>(type_node);
    auto type = SymbolAnnotator::typeNodeToType(type_array->type_.get());
    int32_t length;
    if (type_array->length_ != nullptr &&
        is_instance_of<ExprLiteralIntNode, ExprNode>(type_array->length_)) {
      // TODO: 编译期求值。
      length =
          dynamic_cast<const ExprLiteralIntNode *>(type_array->length_.get())
              ->value_;
    } else {
      throw std::runtime_error("Array length must be a literal integer");
    }
    return std::make_shared<TypeKindArray>(type, length);
  }
  throw std::runtime_error("Unsupported type node for symbol collection");
  return nullptr;
}

// Convert a function node to a new-created symbol function info.
auto SymbolAnnotator::fnNodeToFunc(const ItemFnNode *node)
    -> std::shared_ptr<SymbolFunctionInfo> {
  std::vector<std::shared_ptr<TypeKind>> parameters;
  auto return_type = typeNodeToType(node->return_type_.get());
  for (const auto &param : node->parameters_) {
    parameters.push_back(typeNodeToType(param.type.get()));
  }
  return std::make_shared<SymbolFunctionInfo>(node->ID_, return_type,
                                              std::move(parameters));
}

auto SymbolAnnotator::getPathIndexName(const PathNode *path_node,
                                       uint32_t index) -> std::string {
  if (index >= path_node->segments_.size()) {
    throw std::out_of_range("Index out of range for path segments");
  }
  return path_node->segments_[index].name;
}

void SymbolAnnotator::visit(ASTRootNode *node) {
  for (auto &item : node->items_) {
    item->accept(*this);
  }
}

void SymbolAnnotator::visit(ItemConstNode *node) {
  node->type_->accept(*this);
  node->value_->accept(*this);
  auto type = typeNodeToType(node->type_.get());
  auto const_info = std::make_shared<SymbolConstInfo>(node->ID_, type);
  current_scope_->addConst(node->ID_, std::move(const_info));
}

void SymbolAnnotator::visit(ItemFnNode *node) {
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
void SymbolAnnotator::visit(ItemStructNode *node) {

}

void SymbolAnnotator::visit(ItemEnumNode *node) {
 
}

void SymbolAnnotator::visit(ItemImplNode *node) {
  if (!is_instance_of<TypePathNode, TypeNode>(node->type_.get())) {
    throw std::runtime_error("Impl type must be a path type");
  }
  const auto type_path = dynamic_cast<const TypePathNode *>(node->type_.get());
  std::string type_name = getPathIndexName(type_path->path_.get(), 0);
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
      auto const_type = typeNodeToType(item.constant->type_.get());
      auto const_info =
          std::make_shared<SymbolConstInfo>(item.constant->ID_, const_type);
      if (!type_def->addConst(item.constant->ID_, const_info)) {
        throw CompilerException(
            "Duplicate constant name: " + item.constant->ID_, node->position_);
      }
    }
  }
}

void SymbolAnnotator::visit(ItemTraitNode *node) {}

void SymbolAnnotator::visit(ExprArrayNode *node) {
  for (const auto &item : node->elements_) {
    item->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void SymbolAnnotator::visit(ExprArrayIndexNode *node) {
  node->array_->accept(*this);
  node->index_->accept(*this);
}

void SymbolAnnotator::visit(ExprBlockNode *node) {
  current_scope_ = current_scope_->getNextChildScope();
  for (auto &stmt : node->statements_) {
    stmt->accept(*this);
  }
  if (node->return_value_) {
    node->return_value_->accept(*this);
  }
  current_scope_ = current_scope_->getParent();
}

void SymbolAnnotator::visit(ExprBlockConstNode *node) {
  node->block_expr_->accept(*this);
}

void SymbolAnnotator::visit(ExprCallNode *node) {
  node->caller_->accept(*this);
  for (auto &arg : node->arguments_) {
    arg->accept(*this);
  }
}

void SymbolAnnotator::visit(ExprBreakNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void SymbolAnnotator::visit(ExprReturnNode *node) {
  if (node->value_) {
    node->value_->accept(*this);
  }
}

void SymbolAnnotator::visit(ExprContinueNode *node) {}

void SymbolAnnotator::visit(ExprGroupNode *node) { node->expr_->accept(*this); }

void SymbolAnnotator::visit(ExprIfNode *node) {
  node->condition_->accept(*this);
  if (node->then_block_) {
    node->then_block_->accept(*this);
  }
  if (node->else_block_) {
    node->else_block_->accept(*this);
  }
}

void SymbolAnnotator::visit(ExprLiteralIntNode *node) {}

void SymbolAnnotator::visit(ExprLiteralBoolNode *node) {}

void SymbolAnnotator::visit(ExprLiteralCharNode *node) {}

void SymbolAnnotator::visit(ExprLiteralStringNode *node) {}

void SymbolAnnotator::visit(ExprLoopNode *node) {
  node->loop_body_->accept(*this);
}

void SymbolAnnotator::visit(ExprWhileNode *node) {
  node->condition_->accept(*this);
  node->loop_body_->accept(*this);
}

void SymbolAnnotator::visit(ExprOperBinaryNode *node) {
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
}

void SymbolAnnotator::visit(ExprOperUnaryNode *node) {
  node->operand_->accept(*this);
}

void SymbolAnnotator::visit(ExprPathNode *node) {}

void SymbolAnnotator::visit(ExprFieldNode *node) {
  node->instance_->accept(*this);
}

void SymbolAnnotator::visit(ExprMethodNode *node) {
  node->instance_->accept(*this);
  for (auto &param : node->parameters_) {
    param->accept(*this);
  }
}

void SymbolAnnotator::visit(ExprStructNode *node) {
  node->path_->accept(*this);
  for (const auto &field : node->fields_) {
    field.expr->accept(*this);
  }
}

void SymbolAnnotator::visit(ExprUnderScoreNode *node) {}

void SymbolAnnotator::visit(StmtEmptyNode *node) {}

void SymbolAnnotator::visit(StmtItemNode *node) { node->item_->accept(*this); }

void SymbolAnnotator::visit(StmtLetNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->init_value_) {
    node->init_value_->accept(*this);
  }
}

void SymbolAnnotator::visit(StmtExprNode *node) { node->expr_->accept(*this); }

void SymbolAnnotator::visit(PatternLiteralNode *node) {}

void SymbolAnnotator::visit(PatternWildNode *node) {}

void SymbolAnnotator::visit(PatternPathNode *node) {}

void SymbolAnnotator::visit(PatternIDNode *node) {}

void SymbolAnnotator::visit(PatternReferNode *node) {}

void SymbolAnnotator::visit(TypeArrayNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void SymbolAnnotator::visit(TypePathNode *node) {}

void SymbolAnnotator::visit(TypeReferNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
}

void SymbolAnnotator::visit(PathNode *node) {}