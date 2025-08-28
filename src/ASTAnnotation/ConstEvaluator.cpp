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

ConstEvaluator::ConstEvaluator() { current_scope_ = nullptr; }

ConstEvaluator::~ConstEvaluator() = default;

void ConstEvaluator::bindScopePointer(Scope *current_scope) {
  current_scope_ = &current_scope;
}

void ConstEvaluator::attachNodeToTypeDef(ASTNode *node,
                                         const std::string &symbol) {
  type_def_symbols.emplace(symbol, node);
}

void ConstEvaluator::attachNodeToConst(ASTNode *node,
                                       const std::string &symbol) {
  const_symbols.emplace(symbol, node);
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
    int32_t length;
    if (array_node->length_ != nullptr) {
      auto length_const = evaluateExprValue(array_node->length_.get());
      auto length_value = dynamic_shared_ptr_cast<ConstValueInt>(length_const);
      if (length_value == nullptr) {
        throw std::runtime_error("Array length must be a constant integer");
      }
      length = length_value->getValue();
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
  }
  if (is_instance_of<ExprGroupNode, ExprNode>(node)) {
  }
  if (is_instance_of<ExprLiteralIntNode, ExprNode>(node)) {
  }
  if (is_instance_of<ExprLiteralBoolNode, ExprNode>(node)) {
  }
  if (is_instance_of<ExprLiteralCharNode, ExprNode>(node)) {
  }
  if (is_instance_of<ExprLiteralStringNode, ExprNode>(node)) {
  }
  if (is_instance_of<ExprOperBinaryNode, ExprNode>(node)) {
  }
  if (is_instance_of<ExprOperUnaryNode, ExprNode>(node)) {
  }
  if (is_instance_of<ExprPathNode, ExprNode>(node)) {
  }
  if (is_instance_of<ExprFieldNode, ExprNode>(node)) {
  }
  if (is_instance_of<ExprStructNode, ExprNode>(node)) {
  }
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
    auto const_info = std::make_unique<ConstInfo>(std::move(const_type));
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
    auto const_info = std::make_unique<ConstInfo>(std::move(const_type));
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