#include "ConstEvaluator.hpp"
#include "ASTNodeTotal.hpp"
#include "ConstInfo.hpp"
#include "ConstValue.hpp"
#include "Scope.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include <stdexcept>

StatusRecorder::StatusRecorder() { status_ = UNTOUCH; }
StatusRecorder::~StatusRecorder() = default;
auto StatusRecorder::touch() -> bool {
  if (status_ == UNTOUCH) {
    status_ = CALC;
    return true;
  }
  return false;
}
auto StatusRecorder::isValid() const -> bool { return status_ == VAILD; }
void StatusRecorder::setVaild() { status_ = VAILD; }

SymbolStatus::SymbolStatus() : node(nullptr), status() {}
SymbolStatus::SymbolStatus(ASTNode *target) : node(target), status() {}

ConstEvaluator::ConstEvaluator() { current_scope_ = nullptr; }

ConstEvaluator::~ConstEvaluator() = default;

void ConstEvaluator::bindScopePointer(Scope **current_scope) {
  current_scope_ = current_scope;
}

void ConstEvaluator::addBuiltInSymbol(const std::string &symbol) {
  SymbolStatus status;
  status.status.setVaild();
  type_def_symbols.emplace(symbol, status);
}

void ConstEvaluator::attachNodeToTypeDef(ASTNode *node,
                                         const std::string &symbol) {
  SymbolStatus status(node);
  type_def_symbols.emplace(symbol, status);
}

void ConstEvaluator::attachNodeToConst(ASTNode *node,
                                       const std::string &symbol) {
  SymbolStatus status(node);
  const_symbols.emplace(symbol, status);
}

void ConstEvaluator::attachNodeToStructConst(ASTNode *node,
                                             const std::string &struct_name,
                                             const std::string &symbol) {
  struct_const_symbols[struct_name].emplace(symbol, node);
}

auto ConstEvaluator::evaluateType(TypeNode *node) -> std::shared_ptr<TypeKind> {
  if (node == nullptr) {
    return std::make_shared<TypeKindPath>(
        (*current_scope_)->getType("unit")->getType());
  }
  if (is_instance_of<TypePathNode, TypeNode>(node)) {
    const auto *path_node = dynamic_cast<const TypePathNode *>(node);
    if (path_node->path_->size() == 1) {
      std::string type_name = path_node->path_->getPathIndexName(0);
      evaluateTypeSymbol(type_name);
      auto type_def = (*current_scope_)->getType(type_name)->getType();
      return std::make_shared<TypeKindPath>(type_def);
    } else {
      throw std::runtime_error("Unsupported complex type path.");
    }
  }
  if (is_instance_of<TypeArrayNode, TypeNode>(node)) {
    const auto *type_array = dynamic_cast<const TypeArrayNode *>(node);
    auto type_inside = evaluateType(type_array->type_.get());
    if (type_array->length_ == nullptr) {
      throw std::runtime_error("Array length cannot be null");
    }
    auto length_value =
        evaluateExprValue(type_array->length_.get())->getConstValue();
    auto length_int = dynamic_shared_ptr_cast<ConstValueInt>(length_value);
    if (!length_int) {
      throw std::runtime_error("Array length must be a constant integer");
    }
    int32_t length = length_int->getValue();
    return std::make_shared<TypeKindArray>(type_inside, length);
  }
  throw std::runtime_error("Unsupported type node for symbol collection");
  return nullptr;
}

auto ConstEvaluator::evaluateExprValue(ExprNode *node)
    -> std::shared_ptr<ConstInfo> {
  if (is_instance_of<ExprArrayNode, ExprNode>(node)) {
    std::vector<std::shared_ptr<ConstValue>> elements;
    auto array_node = static_cast<ExprArrayNode *>(node);
    if (array_node->length_ != nullptr) {
      auto length_const =
          evaluateExprValue(array_node->length_.get())->getConstValue();
      auto length_value = dynamic_shared_ptr_cast<ConstValueInt>(length_const);
      if (length_value == nullptr) {
        throw std::runtime_error("Array length must be a constant integer");
      }
      int32_t length = length_value->getValue();
      auto element_value = evaluateExprValue(array_node->elements_[0].get());
      for (int32_t i = 0; i < length; i++) {
        elements.push_back(element_value->getConstValue());
      }
      auto array_type =
          std::make_shared<TypeKindArray>(element_value->getType(), length);
      auto array_value = std::make_shared<ConstValueArray>(std::move(elements));
      return std::make_shared<ConstInfo>(array_type, array_value);
    }
    auto element_type =
        evaluateExprValue(array_node->elements_[0].get())->getType();
    for (const auto &elem : array_node->elements_) {
      auto elem_value = evaluateExprValue(elem.get());
      if (!elem_value->getType()->isEqual(element_type.get())) {
        throw std::runtime_error("Array elements must have the same type");
      }
      elements.push_back(elem_value->getConstValue());
    }
    auto array_type =
        std::make_shared<TypeKindArray>(element_type, elements.size());
    auto array_value = std::make_shared<ConstValueArray>(std::move(elements));
    return std::make_shared<ConstInfo>(array_type, array_value);
  }
  if (is_instance_of<ExprArrayIndexNode, ExprNode>(node)) {
    auto index_node = static_cast<ExprArrayIndexNode *>(node);
    auto array_const = evaluateExprValue(index_node->array_.get());
    auto array_type =
        dynamic_shared_ptr_cast<TypeKindArray>(array_const->getType());
    if (array_type == nullptr) {
      throw std::runtime_error("Array expression must be of array type");
    }
    auto array_value =
        dynamic_shared_ptr_cast<ConstValueArray>(array_const->getConstValue());
    if (array_value == nullptr) {
      throw std::runtime_error("Array expression must be a constant array");
    }
    auto index_const =
        evaluateExprValue(index_node->index_.get())->getConstValue();
    auto index_value = dynamic_shared_ptr_cast<ConstValueInt>(index_const);
    if (index_value == nullptr) {
      throw std::runtime_error("Array index must be a constant integer");
    }
    auto element_type = array_type->getType();
    auto element_value = array_value->getElement(index_value->getValue());
    if (element_value == nullptr) {
      throw std::runtime_error("Array index out of bounds");
    }
    return std::make_shared<ConstInfo>(element_type, element_value);
  }
  if (is_instance_of<ExprGroupNode, ExprNode>(node)) {
    auto group_node = static_cast<ExprGroupNode *>(node);
    return evaluateExprValue(group_node->expr_.get());
  }
  if (is_instance_of<ExprLiteralIntNode, ExprNode>(node)) {
    auto int_node = static_cast<ExprLiteralIntNode *>(node);
    std::shared_ptr<TypeKind> int_type;
    switch (int_node->int_type_) {
    case ExprLiteralIntNode::IntType::I32: {
      auto i32_type = (*current_scope_)->getType("i32")->getType();
      int_type = std::make_shared<TypeKindPath>(i32_type);
      break;
    }
    case ExprLiteralIntNode::IntType::U32: {
      auto u32_type = (*current_scope_)->getType("u32")->getType();
      int_type = std::make_shared<TypeKindPath>(u32_type);
      break;
    }
    case ExprLiteralIntNode::IntType::NUM: {
      auto i32_type = (*current_scope_)->getType("i32")->getType();
      auto u32_type = (*current_scope_)->getType("u32")->getType();
      auto isize_type = (*current_scope_)->getType("isize")->getType();
      auto usize_type = (*current_scope_)->getType("usize")->getType();
      int_type =
          std::make_shared<TypeKindPossi>(std::vector<std::shared_ptr<TypeDef>>{
              i32_type, u32_type, isize_type, usize_type});
      break;
    }
    }
    auto int_value = std::make_shared<ConstValueInt>(int_node->value_);
    return std::make_shared<ConstInfo>(int_type, int_value);
  }
  if (is_instance_of<ExprLiteralBoolNode, ExprNode>(node)) {
    auto bool_node = static_cast<ExprLiteralBoolNode *>(node);
    auto bool_type = (*current_scope_)->getType("bool")->getType();
    auto bool_value = std::make_shared<ConstValueBool>(bool_node->value_);
    return std::make_shared<ConstInfo>(
        std::make_shared<TypeKindPath>(bool_type), bool_value);
  }
  if (is_instance_of<ExprLiteralCharNode, ExprNode>(node)) {
    auto char_node = static_cast<ExprLiteralCharNode *>(node);
    auto char_type = (*current_scope_)->getType("char")->getType();
    auto char_value = std::make_shared<ConstValueChar>(char_node->value_);
    return std::make_shared<ConstInfo>(
        std::make_shared<TypeKindPath>(char_type), char_value);
  }
  if (is_instance_of<ExprLiteralStringNode, ExprNode>(node)) {
    auto string_node = static_cast<ExprLiteralStringNode *>(node);
    auto string_type = (*current_scope_)->getType("str")->getType();
    auto string_value = std::make_shared<ConstValueString>(string_node->value_);
    return std::make_shared<ConstInfo>(
        std::make_shared<TypeKindPath>(string_type), string_value);
  }
  if (is_instance_of<ExprOperBinaryNode, ExprNode>(node)) {
    auto binary_node = static_cast<ExprOperBinaryNode *>(node);
    auto lhs_const = evaluateExprValue(binary_node->lhs_.get());
    auto rhs_const = evaluateExprValue(binary_node->rhs_.get());
    auto lhs_value = lhs_const->getConstValue();
    auto rhs_value = rhs_const->getConstValue();
    std::shared_ptr<TypeKind> result_kind;
    std::shared_ptr<ConstValue> result_value;
    switch (binary_node->op_) {
    case BinaryOperator::PLUS:
    case BinaryOperator::MINUS:
    case BinaryOperator::MUL:
    case BinaryOperator::DIV:
    case BinaryOperator::MOD:
    case BinaryOperator::XOR:
    case BinaryOperator::AND:
    case BinaryOperator::OR:
    case BinaryOperator::LEFT_SHIFT:
    case BinaryOperator::RIGHT_SHIFT: {
      auto lhs_int = dynamic_shared_ptr_cast<ConstValueInt>(lhs_value);
      auto rhs_int = dynamic_shared_ptr_cast<ConstValueInt>(rhs_value);
      if (lhs_int == nullptr || rhs_int == nullptr) {
        throw std::runtime_error("Plus requires integer operands in const.");
      }
      int32_t lhs_val = lhs_int->getValue();
      int32_t rhs_val = rhs_int->getValue();
      switch (binary_node->op_) {
      case BinaryOperator::PLUS:
        result_value = std::make_shared<ConstValueInt>(lhs_val + rhs_val);
        break;
      case BinaryOperator::MINUS:
        result_value = std::make_shared<ConstValueInt>(lhs_val - rhs_val);
        break;
      case BinaryOperator::MUL:
        result_value = std::make_shared<ConstValueInt>(lhs_val * rhs_val);
        break;
      case BinaryOperator::DIV:
        if (rhs_val == 0) {
          throw std::runtime_error("Division by zero in const expression.");
        }
        result_value = std::make_shared<ConstValueInt>(lhs_val / rhs_val);
        break;
      case BinaryOperator::MOD:
        if (rhs_val == 0) {
          throw std::runtime_error("Modulo by zero in const expression.");
        }
        result_value = std::make_shared<ConstValueInt>(lhs_val % rhs_val);
        break;
      case BinaryOperator::XOR:
        result_value = std::make_shared<ConstValueInt>(lhs_val ^ rhs_val);
        break;
      case BinaryOperator::AND:
        result_value = std::make_shared<ConstValueInt>(lhs_val & rhs_val);
        break;
      case BinaryOperator::OR:
        result_value = std::make_shared<ConstValueInt>(lhs_val | rhs_val);
        break;
      case BinaryOperator::LEFT_SHIFT:
        result_value = std::make_shared<ConstValueInt>(lhs_val << rhs_val);
        break;
      case BinaryOperator::RIGHT_SHIFT:
        result_value = std::make_shared<ConstValueInt>(lhs_val >> rhs_val);
        break;
      default:
        throw std::runtime_error("Unsupported binary operator in constant.");
      }
      result_kind = lhs_const->getType();
      break;
    }
    case BinaryOperator::EQUAL:
    case BinaryOperator::NOT_EQUAL:
    case BinaryOperator::LESS_THAN:
    case BinaryOperator::LESS_EQUAL:
    case BinaryOperator::GREATER_THAN:
    case BinaryOperator::GREATER_EQUAL: {
      auto lhs_int = dynamic_shared_ptr_cast<ConstValueInt>(lhs_value);
      auto rhs_int = dynamic_shared_ptr_cast<ConstValueInt>(rhs_value);
      if (lhs_int == nullptr || rhs_int == nullptr) {
        throw std::runtime_error("Comparison requires integer operands.");
      }
      int32_t lhs_val = lhs_int->getValue();
      int32_t rhs_val = rhs_int->getValue();
      bool cmp_result;
      switch (binary_node->op_) {
      case BinaryOperator::EQUAL:
        cmp_result = (lhs_val == rhs_val);
        break;
      case BinaryOperator::NOT_EQUAL:
        cmp_result = (lhs_val != rhs_val);
        break;
      case BinaryOperator::LESS_THAN:
        cmp_result = (lhs_val < rhs_val);
        break;
      case BinaryOperator::LESS_EQUAL:
        cmp_result = (lhs_val <= rhs_val);
        break;
      case BinaryOperator::GREATER_THAN:
        cmp_result = (lhs_val > rhs_val);
        break;
      case BinaryOperator::GREATER_EQUAL:
        cmp_result = (lhs_val >= rhs_val);
        break;
      default:
        throw std::runtime_error(
            "Unsupported comparison operator in constant.");
      }
      result_value = std::make_shared<ConstValueBool>(cmp_result);
      auto bool_type = (*current_scope_)->getType("bool")->getType();
      result_kind = std::make_shared<TypeKindPath>(bool_type);
      break;
    }
    case BinaryOperator::LOGIC_AND:
    case BinaryOperator::LOGIC_OR: {
      auto lhs_bool = dynamic_shared_ptr_cast<ConstValueBool>(lhs_value);
      auto rhs_bool = dynamic_shared_ptr_cast<ConstValueBool>(rhs_value);
      if (lhs_bool == nullptr || rhs_bool == nullptr) {
        throw std::runtime_error(
            "Logical operation requires boolean operands.");
      }
      bool lhs_val = lhs_bool->getValue();
      bool rhs_val = rhs_bool->getValue();
      bool logic_result;
      switch (binary_node->op_) {
      case BinaryOperator::LOGIC_AND:
        logic_result = lhs_val && rhs_val;
        break;
      case BinaryOperator::LOGIC_OR:
        logic_result = lhs_val || rhs_val;
        break;
      default:
        throw std::runtime_error("Unsupported logical operator in constant.");
      }
      result_value = std::make_shared<ConstValueBool>(logic_result);
      auto bool_type = (*current_scope_)->getType("bool")->getType();
      result_kind = std::make_shared<TypeKindPath>(bool_type);
      break;
    }
    default:
      throw std::runtime_error("Unsupported binary operator in constant.");
    }
    return std::make_shared<ConstInfo>(result_kind, result_value);
  }
  if (is_instance_of<ExprOperUnaryNode, ExprNode>(node)) {
    auto unary_node = static_cast<ExprOperUnaryNode *>(node);
    auto operand_const = evaluateExprValue(unary_node->operand_.get());
    auto operand_value = operand_const->getConstValue();
    std::shared_ptr<TypeKind> result_kind;
    std::shared_ptr<ConstValue> result_value;
    switch (unary_node->op_) {
    case UnaryOperator::NEGATE: {
      auto operand_int = dynamic_shared_ptr_cast<ConstValueInt>(operand_value);
      if (operand_int == nullptr) {
        throw std::runtime_error("Negation requires an integer operand.");
      }
      result_value = std::make_shared<ConstValueInt>(-operand_int->getValue());
      result_kind = operand_const->getType();
      break;
    }
    case UnaryOperator::NOT: {
      if (is_instance_of<ConstValueBool, ConstValue>(operand_value.get())) {
        auto operand_bool = static_cast<ConstValueBool *>(operand_value.get());
        result_value =
            std::make_shared<ConstValueBool>(!operand_bool->getValue());
        auto bool_type = (*current_scope_)->getType("bool")->getType();
        result_kind = std::make_shared<TypeKindPath>(bool_type);
      } else if (is_instance_of<ConstValueInt, ConstValue>(
                     operand_value.get())) {
        auto operand_int = static_cast<ConstValueInt *>(operand_value.get());
        result_value =
            std::make_shared<ConstValueInt>(~operand_int->getValue());
        result_kind = operand_const->getType();
      }
    }
    default:
      throw std::runtime_error("Unsupported unary operator in constant.");
    }
    return std::make_shared<ConstInfo>(result_kind, result_value);
  }
  if (is_instance_of<ExprPathNode, ExprNode>(node)) {
    auto path_node = static_cast<ExprPathNode *>(node);
    if (path_node->path_->size() == 1) {
      std::string var_name = path_node->path_->getPathIndexName(0);
      evaluateConstSymbol(var_name);
      auto symbol_info = (*current_scope_)->getSymbol(var_name);
      auto const_symbol_info =
          std::dynamic_pointer_cast<SymbolConstInfo>(symbol_info);
      if (!const_symbol_info) {
        throw std::runtime_error("Const symbol does not exist: " + var_name);
      }
      auto const_value = const_symbol_info->getValue();
      auto const_type = const_symbol_info->getType();
      return std::make_shared<ConstInfo>(const_type, const_value);
    } else if (path_node->path_->size() == 2) {
      std::string struct_name = path_node->path_->getPathIndexName(0);
      std::string var_name = path_node->path_->getPathIndexName(1);
      evaluateStructConst(struct_name, var_name);
      auto type_def = (*current_scope_)->getType(struct_name)->getType();
      auto const_symbol_info = type_def->getConst(var_name);
      if (!const_symbol_info) {
        throw std::runtime_error("Struct const symbol does not exist: " +
                                 struct_name + "::" + var_name);
      }
      auto const_value = const_symbol_info->getValue();
      auto const_type = const_symbol_info->getType();
      return std::make_shared<ConstInfo>(const_type, const_value);
    } else {
      throw std::runtime_error("Unsupported complex path in const expr.");
    }
  }
  if (is_instance_of<ExprFieldNode, ExprNode>(node)) {
    auto field_node = static_cast<ExprFieldNode *>(node);
    auto instance_const = evaluateExprValue(field_node->instance_.get());
    auto instance_value = instance_const->getConstValue();
    auto path_type =
        dynamic_shared_ptr_cast<TypeKindPath>(instance_const->getType());
    if (path_type == nullptr) {
      throw std::runtime_error("Field access requires a path type instance.");
    }
    auto struct_def =
        std::dynamic_pointer_cast<StructDef>(path_type->getTypeDef());
    if (struct_def == nullptr) {
      throw std::runtime_error("Field access requires a struct type instance.");
    }
    auto field_type = struct_def->getMember(field_node->ID_);
    if (field_type == nullptr) {
      throw std::runtime_error("Field does not exist in struct: " +
                               field_node->ID_);
    }
    auto struct_value = static_cast<ConstValueStruct *>(instance_value.get());
    auto field_value = struct_value->getField(field_node->ID_);
    return std::make_shared<ConstInfo>(field_type, field_value);
  }
  if (is_instance_of<ExprStructNode, ExprNode>(node)) {
    auto struct_node = static_cast<ExprStructNode *>(node);
    std::string struct_name = struct_node->path_->path_->getPathIndexName(0);
    evaluateTypeSymbol(struct_name);
    auto type_def = (*current_scope_)->getType(struct_name)->getType();
    auto struct_def = std::dynamic_pointer_cast<StructDef>(type_def);
    if (!struct_def) {
      throw std::runtime_error("Struct " + struct_name + " not found");
    }
    std::unordered_map<std::string, std::shared_ptr<ConstValue>> field_values;
    for (const auto &field : struct_node->fields_) {
      auto field_value_const = evaluateExprValue(field.expr.get());
      auto field_type = struct_def->getMember(field.ID);
      if (!field_type) {
        throw std::runtime_error(field.ID + " not found in struct " +
                                 struct_name);
      }
      if (!field_value_const->getType()->isEqual(field_type.get())) {
        throw std::runtime_error("Field " + field.ID +
                                 " type mismatch in struct " + struct_name);
      }
      field_values[field.ID] = field_value_const->getConstValue();
    }
    auto struct_type = std::make_shared<TypeKindPath>(struct_def);
    auto struct_value =
        std::make_shared<ConstValueStruct>(std::move(field_values));
    return std::make_shared<ConstInfo>(struct_type, struct_value);
  }
  throw std::runtime_error("Unsupported expression node for const eval.");
  return nullptr;
}

void ConstEvaluator::evaluateTypeSymbol(const std::string &symbol) {
  if (type_def_symbols.find(symbol) == type_def_symbols.end()) {
    throw std::runtime_error("Type symbol not found: " + symbol);
  }
  auto symbol_status = type_def_symbols[symbol];
  if (symbol_status.status.isValid()) {
    return;
  }
  if (!symbol_status.status.touch()) {
    throw std::runtime_error("Cyclic dependency detected in type: " + symbol);
  }
  auto node = symbol_status.node;
  if (is_instance_of<ItemStructNode, ASTNode>(node)) {
    auto struct_node = dynamic_cast<ItemStructNode *>(node);
    auto type_def = (*current_scope_)->getType(struct_node->ID_)->getType();
    auto struct_def = std::dynamic_pointer_cast<StructDef>(type_def);
    for (const auto &field : struct_node->fields_) {
      auto field_type = evaluateType(field.type.get());
      if (!struct_def->addMember(field.identifier, field_type)) {
        throw std::runtime_error("Duplicate member name: " + field.identifier);
      }
    }
  } else if (is_instance_of<ItemEnumNode, ASTNode>(node)) {
    auto enum_node = dynamic_cast<ItemEnumNode *>(node);
    auto type_def = (*current_scope_)->getType(enum_node->ID_)->getType();
    auto enum_def = std::dynamic_pointer_cast<EnumDef>(type_def);
    for (const auto &variant : enum_node->variants_) {
      if (!enum_def->addVariant(variant)) {
        throw std::runtime_error("Duplicate variant name: " + variant);
      }
    }
  } else {
    throw std::runtime_error("Unsupported type def node for symbol: " + symbol);
  }
}

void ConstEvaluator::evaluateConstSymbol(const std::string &symbol) {
  if (const_symbols.find(symbol) == const_symbols.end()) {
    throw std::runtime_error("Const symbol not found: " + symbol);
  }
  auto symbol_status = const_symbols[symbol];
  if (symbol_status.status.isValid()) {
    return;
  }
  if (!symbol_status.status.touch()) {
    throw std::runtime_error("Cyclic dependency detected in const: " + symbol);
  }
  auto node = symbol_status.node;
  if (is_instance_of<ItemConstNode, ASTNode>(node)) {
    auto const_node = dynamic_cast<ItemConstNode *>(node);
    auto const_type = evaluateType(const_node->type_.get());
    auto const_value = evaluateExprValue(const_node->value_.get());
    if (!const_value->getType()->isEqual(const_type.get())) {
      throw std::runtime_error("Const value type mismatch: " + symbol);
    }
    auto const_info = std::make_shared<ConstInfo>(std::move(const_type));
    const_info->setConstValue(const_value.get());
    auto const_symbol_info = std::dynamic_pointer_cast<SymbolConstInfo>(
        (*current_scope_)->getSymbol(symbol));
    if (!const_symbol_info) {
      throw std::runtime_error("Const symbol does not exist: " + symbol);
    }
    const_symbol_info->setValue(std::move(const_value));
  } else {
    throw std::runtime_error("Unsupported const node for symbol: " + symbol);
  }
  symbol_status.status.setVaild();
}

void ConstEvaluator::evaluateStructConst(const std::string &struct_name,
                                         const std::string &symbol) {
  std::string name = struct_name + "::" + symbol;
  if (struct_const_symbols.find(struct_name) == struct_const_symbols.end() ||
      struct_const_symbols[struct_name].find(symbol) ==
          struct_const_symbols[struct_name].end()) {
    throw std::runtime_error("Struct const symbol not found: " + name);
  }
  auto symbol_status = struct_const_symbols[struct_name][symbol];
  if (symbol_status.status.isValid()) {
    return;
  }
  if (!symbol_status.status.touch()) {
    throw std::runtime_error("Cyclic dependency detected in const: " + name);
  }
  auto node = symbol_status.node;
  if (is_instance_of<ItemConstNode, ASTNode>(node)) {
    auto const_node = dynamic_cast<ItemConstNode *>(node);
    auto const_type = evaluateType(const_node->type_.get());
    auto const_value = evaluateExprValue(const_node->value_.get());
    if (!const_value->getType()->isEqual(const_type.get())) {
      throw std::runtime_error("Const value type mismatch: " + symbol);
    }
    auto const_info = std::make_shared<ConstInfo>(std::move(const_type));
    const_info->setConstValue(const_value.get());
    auto type_def = (*current_scope_)->getType(struct_name)->getType();
    auto const_symbol_info = type_def->getConst(symbol);
    if (!const_symbol_info) {
      throw std::runtime_error("Const symbol does not exist: " + symbol);
    }
    const_symbol_info->setValue(std::move(const_value));
  } else {
    throw std::runtime_error("Unsupported const node for symbol: " + symbol);
  }
  symbol_status.status.setVaild();
}