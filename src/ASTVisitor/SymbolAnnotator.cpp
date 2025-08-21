#include "SymbolAnnotator.hpp"
#include "ASTNodeTotal.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"

SymbolAnnotator::SymbolAnnotator(Scope *initial_scope)
    : current_scope_(initial_scope) {}

SymbolAnnotator::~SymbolAnnotator() = default;

auto SymbolAnnotator::typeNodeToType(const TypeNode *type_node)
    -> std::shared_ptr<TypeKind> {
  if (type_node == nullptr) {
    return std::make_shared<TypeKindPath>(
        current_scope_->getType("super")->getType());
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
    return std::make_shared<TypeKindArray>(std::move(type), length);
  }
  throw std::runtime_error("Unsupported type node for symbol collection");
  return nullptr;
}

auto SymbolAnnotator::fnNodeToFunc(const ItemFnNode *node)
    -> std::shared_ptr<SymbolFunctionInfo> {
  auto return_type = typeNodeToType(node->return_type_.get());
  std::vector<std::shared_ptr<TypeKind>> parameters;
  for (const auto &param : node->parameters_) {
    parameters.push_back(typeNodeToType(param.type.get()));
  }
  return std::make_shared<SymbolFunctionInfo>(node->ID_, std::move(return_type),
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
  current_scope_->addSymbol(node->ID_, std::make_shared<SymbolVariableInfo>(
                                           node->ID_, std::move(type)));
}

void SymbolAnnotator::visit(ItemFnNode *node) {
  for (const auto &param : node->parameters_) {
    param.type->accept(*this);
  }
  if (node->return_type_) {
    node->return_type_->accept(*this);
  }
  if (node->body_) {
    node->body_->accept(*this);
  }
  current_scope_->addSymbol(node->ID_, fnNodeToFunc(node));
}

void SymbolAnnotator::visit(ItemStructNode *node) {
  auto type_def = current_scope_->getType(node->ID_)->getType();
  if (type_def == nullptr ||
      !is_instance_of<StructDef, TypeDef>(type_def.get())) {
    throw std::runtime_error("TypeDef for struct " + node->ID_ + " not found");
  }
  auto struct_def = std::dynamic_pointer_cast<StructDef>(type_def);
  for (const auto &field : node->fields_) {
    field.type->accept(*this);
    auto field_type = typeNodeToType(field.type.get());
    if (!struct_def->addMember(field.identifier, std::move(field_type))) {
      throw std::runtime_error("Duplicate member name: " + field.identifier);
    }
  }
}

void SymbolAnnotator::visit(ItemEnumNode *node) {
  auto type_def = current_scope_->getType(node->ID_)->getType();
  if (type_def == nullptr ||
      !is_instance_of<EnumDef, TypeDef>(type_def.get())) {
    throw std::runtime_error("TypeDef for enum " + node->ID_ + " not found");
  }
  auto enum_def = std::dynamic_pointer_cast<EnumDef>(type_def);
  for (const auto &variant : node->variants_) {
    if (!enum_def->addVariant(variant)) {
      throw std::runtime_error("Duplicate variant name: " + variant);
    }
  }
}

void SymbolAnnotator::visit(ItemImplNode *node) {
  if (!is_instance_of<TypePathNode, TypeNode>(node->type_.get())) {
    throw std::runtime_error("Impl type must be a path type");
  }
  const auto *type_path = dynamic_cast<const TypePathNode *>(node->type_.get());
  std::string type_name = getPathIndexName(type_path->path_.get(), 0);
  auto type_def = current_scope_->getType(type_name)->getType();
  for (const auto &item : node->items_) {
    if (item.function) {
      item.function->accept(*this);
      auto func = fnNodeToFunc(item.function.get());
      if (!type_def->addMethod(func->getName(), std::move(func))) {
        throw std::runtime_error("Duplicate method name: " + func->getName());
      }
    } else {
      throw std::runtime_error(
          "ItemImplNode currently does not support constants");
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

void SymbolAnnotator::visit(ExprMatchNode *node) {
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

void SymbolAnnotator::visit(TypeArrayNode *node) {
  if (node->type_) {
    node->type_->accept(*this);
  }
  if (node->length_) {
    node->length_->accept(*this);
  }
}

void SymbolAnnotator::visit(TypePathNode *node) {}

void SymbolAnnotator::visit(PathNode *node) {}