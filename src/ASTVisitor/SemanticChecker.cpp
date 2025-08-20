#include "SemanticChecker.hpp"
#include "ASTNodeTotal.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"

SemanticChecker::SemanticChecker(Scope *initial_scope)
    : current_scope_(initial_scope) {}
SemanticChecker::~SemanticChecker() = default;

auto SemanticChecker::typeNodeToType(const TypeNode *type_node)
    -> std::shared_ptr<TypeKind> {
  if (type_node == nullptr) {
    /*Super means "unit", as there is no "null" keyword in Rust.*/
    return std::make_shared<TypeKind>(
        current_scope_->getType("super")->getType());
  }
  if (is_instance_of<TypePathNode, TypeNode>(type_node)) {
    const auto *type_path = dynamic_cast<const TypePathNode *>(type_node);
    std::string type_name = getPathIndexName(type_path->path_.get(), 0);
    return std::make_shared<TypeKind>(
        current_scope_->getType(type_name)->getType());
  }
  if (is_instance_of<TypeArrayNode, TypeNode>(type_node)) {
    const auto *type_array = dynamic_cast<const TypeArrayNode *>(type_node);
    auto type = SemanticChecker::typeNodeToType(type_array->type_.get());
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
    return std::make_shared<TypeKind>(type, length);
  }
  throw std::runtime_error("Unsupported type node for symbol collection");
  return nullptr;
}

auto SemanticChecker::getPathIndexName(const PathNode *path_node,
                                       uint32_t index) -> std::string {
  if (index >= path_node->segments_.size()) {
    throw std::out_of_range("Index out of range for path segments");
  }
  return path_node->segments_[index].name;
}

auto SemanticChecker::bindPatternToType(const PatternNode *pattern_node,
                                        const std::shared_ptr<TypeKind> &type)
    -> bool {
  if (is_instance_of<PatternIDNode, PatternNode>(pattern_node)) {
    const auto *id_pattern = dynamic_cast<const PatternIDNode *>(pattern_node);
    return current_scope_->addSymbol(
        id_pattern->identifier_,
        std::make_shared<SymbolVariableInfo>(id_pattern->identifier_, type));
  }
  if (is_instance_of<PatternWildNode, PatternNode>(pattern_node)) {
    return true;
  }
  throw std::runtime_error("Patterns in function paras must be irrefutable.");
  return false;
}

auto SemanticChecker::judgeU32(const ExprNode *node) -> bool {
  if (!is_instance_of<ExprLiteralIntNode, ExprNode>(node)) {
    auto index_type = node->value_info_->getType();
    if (!index_type->isTypePath(
            current_scope_->getType("u32")->getType().get())) {
      return false;
    }
  }
  return true;
}

auto SemanticChecker::judgeNum(const ExprNode *node) -> bool {
  auto type = node->value_info_->getType();
  return type->isTypePath(current_scope_->getType("i32")->getType().get()) ||
         type->isTypePath(current_scope_->getType("u32")->getType().get());
}

void SemanticChecker::visit(ASTRootNode *node) {
  for (auto &item : node->items_) {
    item->accept(*this);
  }
}

void SemanticChecker::visit(ItemConstNode *node) {
  node->type_->accept(*this);
  node->value_->accept(*this);
}

void SemanticChecker::visit(ItemFnNode *node) {
  for (const auto &param : node->parameters_) {
    param.type->accept(*this);
  }
  for (const auto &param : node->parameters_) {
    if (param.type) {
      param.type->accept(*this);
    }
  }
  if (node->return_type_) {
    node->return_type_->accept(*this);
  }
  if (node->body_) {
    auto function_scope = std::make_unique<Scope>(current_scope_);
    current_scope_ = function_scope.get();
    for (auto &param : node->parameters_) {
      if (param.pattern) {
        bindPatternToType(param.pattern.get(),
                          typeNodeToType(param.type.get()));
      } else {
        throw std::runtime_error(
            "Parameters must have a pattern in function definitions");
      }
    }
    node->body_->accept(*this);
    if (!node->body_->value_info_->getType()->isEqual(
            typeNodeToType(node->return_type_.get()).get())) {
      throw std::runtime_error("Function return type mismatch");
    }
    current_scope_ = current_scope_->getParent();
  }
}

void SemanticChecker::visit(ItemStructNode *node) {
  for (const auto &fields : node->fields_) {
    fields.type->accept(*this);
  }
}

void SemanticChecker::visit(ItemEnumNode *node) {}

void SemanticChecker::visit(ItemImplNode *node) {
  for (const auto &item : node->items_) {
    if (item.function) {
      item.function->accept(*this);
    } else {
      throw std::runtime_error(
          "ItemImplNode currently does not support constants");
    }
  }
}

void SemanticChecker::visit(ItemTraitNode *node) {}

void SemanticChecker::visit(StmtEmptyNode *node) {}

void SemanticChecker::visit(StmtItemNode *node) { node->item_->accept(*this); }

void SemanticChecker::visit(StmtLetNode *node) {
  node->type_->accept(*this);
  auto type = typeNodeToType(node->type_.get());
  bindPatternToType(node->pattern_.get(), type);
  if (node->init_value_) {
    node->init_value_->accept(*this);
  }
  if (!node->init_value_->value_info_->getType()->isEqual(type.get())) {
    throw std::runtime_error("Type mismatch in let statement initialization");
  }
}

void SemanticChecker::visit(StmtExprNode *node) { node->expr_->accept(*this); }

void SemanticChecker::visit(ExprArrayNode *node) {
  if (node->length_) {
    node->length_->accept(*this);
    auto length_type = node->length_->value_info_->getType();
    if (!judgeU32(node->length_.get())) {
      throw std::runtime_error("Array length must be U32.");
    }
  }
  node->elements_[0]->accept(*this);
  auto element_type = node->elements_[0]->value_info_->getType();
  for (const auto &element : node->elements_) {
    element->accept(*this);
    if (!element->value_info_->getType()->isEqual(element_type.get())) {
      throw std::runtime_error(
          "All elements in an array must have the same type");
    }
  }
  auto array_type = std::make_shared<TypeKind>(element_type, node->length_);
  node->value_info_ =
      std::make_unique<ValueInfo>(array_type, false, false, false);
}

void SemanticChecker::visit(ExprArrayIndexNode *node) {
  node->array_->accept(*this);
  node->index_->accept(*this);
  auto array_type = node->array_->value_info_->getType();
  if (!is_instance_of<TypeKindArray, TypeKind>(array_type.get())) {
    throw std::runtime_error("Array index operation requires an array type");
  }
  auto index_type = node->index_->value_info_->getType();
  if (!judgeU32(node->index_.get())) {
    throw std::runtime_error("Array index must be U32.");
  }
  node->value_info_ = std::make_unique<ValueInfo>(
      dynamic_cast<TypeKindArray *>(array_type.get())->getType(), false, false,
      false);
}

void SemanticChecker::visit(ExprBlockNode *node) {
  current_scope_ = current_scope_->addNextChildScope();
  for (auto &stmt : node->statements_) {
    stmt->accept(*this);
  }
  if (node->return_value_) {
    node->return_value_->accept(*this);
    auto return_type = node->return_value_->value_info_->getType();
    node->value_info_ =
        std::make_unique<ValueInfo>(return_type, false, false, false);
  } else {
    auto unit_type =
        std::make_shared<TypeKind>(current_scope_->getType("super")->getType());
    node->value_info_ =
        std::make_unique<ValueInfo>(unit_type, false, false, false);
  }
  current_scope_ = current_scope_->getParent();
}
void SemanticChecker::visit(ExprBlockConstNode *node) {
  node->block_expr_->accept(*this);
  auto block_type = node->block_expr_->value_info_->getType();
  node->value_info_ =
      std::make_unique<ValueInfo>(block_type, false, false, true);
}

void SemanticChecker::visit(ExprCallNode *node) {
  node->caller_->accept(*this);
  if (!is_instance_of<ExprPathNode, ExprNode>(node->caller_.get())) {
    throw std::runtime_error("Function call requires a path as caller");
  }
  // Located the function in the current scope.
  auto path = dynamic_cast<ExprPathNode *>(node->caller_.get())->path_.get();
  std::shared_ptr<SymbolFunctionInfo> function_info;
  if (path->segments_.size() == 1) {
    auto function_name = getPathIndexName(path, 0);
    auto function = current_scope_->getSymbol(function_name);
    if (!function ||
        !is_instance_of<SymbolFunctionInfo, SymbolInfo>(function.get())) {
      throw std::runtime_error("Function " + function_name + " not found");
    }
    function_info = std::dynamic_pointer_cast<SymbolFunctionInfo>(function);
  } else {
    auto type_name = getPathIndexName(path, 0);
    auto type = current_scope_->getType(type_name)->getType();
    auto function = type->getAssociatedFunction(path->segments_[1].name);
    function_info = function;
  }
  // Check the number and types of parameters.
  if (function_info->getParametersType().size() != node->arguments_.size()) {
    throw std::runtime_error("Function call parameters count mismatch");
  }
  for (int i = 0; i < node->arguments_.size(); ++i) {
    node->arguments_[i]->accept(*this);
    auto arg_type = node->arguments_[i]->value_info_->getType();
    if (!arg_type->isEqual(function_info->getParametersType()[i].get())) {
      throw std::runtime_error("Function call parameters type mismatch");
    }
  }
  node->value_info_ = std::make_unique<ValueInfo>(
      function_info->getReturnType(), false, false, false);
}

void SemanticChecker::visit(ExprBreakNode *node) {
  node->value_->accept(*this);
  auto value_type = node->value_->value_info_->getType();
  node->value_info_ =
      std::make_unique<ValueInfo>(value_type, false, false, false);
}

void SemanticChecker::visit(ExprReturnNode *node) {
  node->value_->accept(*this);
  auto value_type = node->value_->value_info_->getType();
  node->value_info_ =
      std::make_unique<ValueInfo>(value_type, false, false, false);
}

void SemanticChecker::visit(ExprContinueNode *node) {
  node->value_info_ = std::make_unique<ValueInfo>(
      std::make_shared<TypeKind>(current_scope_->getType("super")->getType()),
      false, false, false);
}

void SemanticChecker::visit(ExprGroupNode *node) {
  node->expr_->accept(*this);
  auto expr_type = node->expr_->value_info_->getType();
  node->value_info_ =
      std::make_unique<ValueInfo>(expr_type, false, false, false);
}

void SemanticChecker::visit(ExprIfNode *node) {
  node->condition_->accept(*this);
  if (!is_instance_of<ExprLiteralBoolNode, ExprNode>(node->condition_.get())) {
    throw std::runtime_error("Condition statement must be a boolean");
  }
  node->then_block_->accept(*this);
  auto then_type = node->then_block_->value_info_->getType();
  if (node->else_block_) {
    node->else_block_->accept(*this);
    auto else_type = node->else_block_->value_info_->getType();
    if (!then_type->isEqual(else_type.get())) {
      throw std::runtime_error("Then and else blocks must have the same type");
    }
  } else {
    if (!then_type->isTypePath(
            current_scope_->getType("super")->getType().get())) {
      throw std::runtime_error("Then and else blocks must have the same type");
    }
  }
  node->value_info_ =
      std::make_unique<ValueInfo>(then_type, false, false, false);
}

void SemanticChecker::visit(ExprLiteralIntNode *node) {
  auto int_type =
      std::make_shared<TypeKind>(current_scope_->getType("i32")->getType());
  node->value_info_ =
      std::make_unique<ValueInfo>(int_type, false, false, false);
}

void SemanticChecker::visit(ExprLiteralBoolNode *node) {
  auto bool_type =
      std::make_shared<TypeKind>(current_scope_->getType("bool")->getType());
  node->value_info_ =
      std::make_unique<ValueInfo>(bool_type, false, false, false);
}

void SemanticChecker::visit(ExprLiteralCharNode *node) {
  auto char_type =
      std::make_shared<TypeKind>(current_scope_->getType("char")->getType());
  node->value_info_ =
      std::make_unique<ValueInfo>(char_type, false, false, false);
}

void SemanticChecker::visit(ExprLiteralStringNode *node) {
  auto string_type =
      std::make_shared<TypeKind>(current_scope_->getType("str")->getType());
  node->value_info_ =
      std::make_unique<ValueInfo>(string_type, false, false, false);
}

void SemanticChecker::visit(ExprLoopNode *node) {
  node->loop_body_->accept(*this);
  auto body_type = node->loop_body_->value_info_->getType();
  node->value_info_ =
      std::make_unique<ValueInfo>(body_type, false, false, false);
}

void SemanticChecker::visit(ExprWhileNode *node) {
  node->condition_->accept(*this);
  if (node->condition_->value_info_->getType()->isTypePath(
          current_scope_->getType("bool")->getType().get())) {
    throw std::runtime_error("While condition must be a boolean");
  }
  node->loop_body_->accept(*this);
  auto body_type = node->loop_body_->value_info_->getType();
  node->value_info_ =
      std::make_unique<ValueInfo>(body_type, false, false, false);
}

void SemanticChecker::visit(ExprOperBinaryNode *node) {
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
  auto lhs_type = node->lhs_->value_info_->getType();
  auto rhs_type = node->rhs_->value_info_->getType();
  switch (node->op_) {
  case BinaryOperator::AS_CAST:
    if (!judgeNum(node->lhs_.get())) {
      throw std::runtime_error("The lhs type is not number");
    }
    if (!judgeNum(node->rhs_.get())) {
      throw std::runtime_error("The rhs type is not number");
    }
    break;
  case BinaryOperator::ASSIGN:
    if (lhs_type->isEqual(rhs_type.get()) &&
        node->lhs_->value_info_->isLeftValue()) {
      node->value_info_ =
          std::make_unique<ValueInfo>(lhs_type, true, false, false);
    }
    break;
  case BinaryOperator::EQUAL:
  case BinaryOperator::NOT_EQUAL:
  case BinaryOperator::LESS_THAN:
  case BinaryOperator::LESS_EQUAL:
  case BinaryOperator::GREATER_THAN:
  case BinaryOperator::GREATER_EQUAL:
    if (!lhs_type->isEqual(rhs_type.get())) {
      throw std::runtime_error("Equality operator requires boolean operands");
    }
    node->value_info_ = std::make_unique<ValueInfo>(
        std::make_shared<TypeKind>(current_scope_->getType("bool")->getType()),
        false, false, false);
    break;
  case BinaryOperator::LOGIC_AND:
  case BinaryOperator::LOGIC_OR:
    auto bool_type = current_scope_->getType("bool")->getType().get();
    if (!node->lhs_->value_info_->getType()->isTypePath(bool_type) ||
        !node->rhs_->value_info_->getType()->isTypePath(bool_type)) {
      throw std::runtime_error("Logical and comparison operators require "
                               "boolean operands");
    }
    node->value_info_ = std::make_unique<ValueInfo>(
        std::make_shared<TypeKind>(bool_type), false, false, false);
    break;
  default:
    if (!judgeNum(node->lhs_.get()) || !judgeNum(node->rhs_.get())) {
      throw std::runtime_error("Binary operation operands must be numeric");
    }
    node->value_info_ =
        std::make_unique<ValueInfo>(lhs_type, false, false, false);
  }
}

void SemanticChecker::visit(ExprOperUnaryNode *node) {
  node->operand_->accept(*this);
  auto operand_type = node->operand_->value_info_->getType();
  switch (node->op_) {
  case UnaryOperator::NEGATE:
    if (!judgeNum(node->operand_.get())) {
      throw std::runtime_error("Negation operator requires a numeric operand");
    }
    node->value_info_ =
        std::make_unique<ValueInfo>(operand_type, false, false, false);
    break;
  case UnaryOperator::NOT:
    if (!operand_type->isTypePath(
            current_scope_->getType("bool")->getType().get()) &&
        !judgeNum(node->operand_.get())) {
      throw std::runtime_error("Not operator requires a boolean operand");
    }
  }
}

void SemanticChecker::visit(ExprPathNode *node) {}

void SemanticChecker::visit(ExprFieldNode *node) {
  node->instance_->accept(*this);
  auto instance_type = node->instance_->value_info_->getType();
  if (!is_instance_of<TypeKindPath, TypeKind>(instance_type.get())) {
    throw std::runtime_error("Field access requires a path type");
  }
  auto path_type = dynamic_cast<TypeKindPath *>(instance_type.get());
  auto member_type = path_type->getTypeDef()->getMember(node->ID_);
  if (!member_type) {
    throw std::runtime_error("Field " + node->ID_ + " not found in type");
  }
  node->value_info_ =
      std::make_unique<ValueInfo>(member_type, false, false, false);
}

void SemanticChecker::visit(ExprMethodNode *node) {
  node->instance_->accept(*this);
  if (!is_instance_of<ExprPathNode, ExprNode>(node->instance_.get())) {
    throw std::runtime_error("Method call requires a path as instance");
  }
  auto path = dynamic_cast<ExprPathNode *>(node->instance_.get())->path_.get();
  std::shared_ptr<SymbolFunctionInfo> method_info;
  if (path->segments_.size() != 1) {
    throw std::runtime_error(
        "Method call requires a path with only one segment");
  }
  auto method_name = getPathIndexName(path, 0);
  auto method = current_scope_->getSymbol(method_name);
  method_info = std::dynamic_pointer_cast<SymbolFunctionInfo>(method);
  if (method_info->getParametersType().size() != node->parameters_.size()) {
    throw std::runtime_error("Method call parameters count mismatch");
  }
  for (int i = 0; i < node->parameters_.size(); ++i) {
    node->parameters_[i]->accept(*this);
    auto para_type = node->parameters_[i]->value_info_->getType();
    if (!para_type->isEqual(method_info->getParametersType()[i].get())) {
      throw std::runtime_error("Method call parameters type mismatch");
    }
  }
  node->value_info_ = std::make_unique<ValueInfo>(method_info->getReturnType(),
                                                  false, false, false);
}

void SemanticChecker::visit(ExprMatchNode *node) {}

void SemanticChecker::visit(ExprStructNode *node) {
  std::string name = getPathIndexName(node->path_->path_.get(), 0);
  auto type_def = current_scope_->getType(name);
  if (!type_def) {
    throw std::runtime_error("Struct " + name + " not found");
  }
  auto struct_type = type_def->getType();
  for (auto &field : node->fields_) {
    field.expr->accept(*this);
    auto field_type = field.expr->value_info_->getType();
    auto member_type = struct_type->getMember(field.ID);
    if (!member_type || !field_type->isEqual(member_type.get())) {
      throw std::runtime_error("Field " + field.ID +
                               " type mismatch in struct.");
    }
  }
}

void SemanticChecker::visit(PatternLiteralNode *node) {}
void SemanticChecker::visit(PatternWildNode *node) {}
void SemanticChecker::visit(PatternPathNode *node) {}
void SemanticChecker::visit(PatternIDNode *node) {}

void SemanticChecker::visit(TypeArrayNode *node) {}
void SemanticChecker::visit(TypeSliceNode *node) {}
void SemanticChecker::visit(TypePathNode *node) {}

void SemanticChecker::visit(PathNode *node) {}