#include "SymbolCollector.hpp"
#include "ASTNodeTotal.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include <stdexcept>

SymbolCollector::SymbolCollector(std::shared_ptr<Scope> initial_scope)
    : current_scope_(std::move(initial_scope)) {}

SymbolCollector::~SymbolCollector() = default;

void SymbolCollector::visit(ASTRootNode *node) {
  for (auto &item : node->items_) {
    // pass 1a: add names for self-define types->
    if (is_instance_of<ItemStructNode, ItemNode>(item)) {
      auto struct_node = dynamic_cast<ItemStructNode *>(item.get());
      auto type_def = std::make_shared<TypeDef>(struct_node->ID_);
      auto type_symbol = std::make_shared<SymbolTypeInfo>(struct_node->ID_,
                                                          std::move(type_def));
      current_scope_->addType(struct_node->ID_, std::move(type_symbol));
    }
  }
  for (auto &item : node->items_) {
    // pass 1b: add signatures of functions, impls and collect members in types->
    item->accept(*this);
  }
}

void SymbolCollector::visit(ItemConstNode *node) {
  auto type = getType(node->type_.get());
  current_scope_->addSymbol(
      node->ID_, std::make_shared<SymbolVariableInfo>(node->ID_, type));
}

void SymbolCollector::visit(ItemFnNode *node) {
  current_scope_->addSymbol(node->ID_, getFunc(node));
}

void SymbolCollector::visit(ItemStructNode *node) {
  std::vector<std::shared_ptr<TypeKind>> fields_type;
  std::vector<std::string> fields_name;
  for (const auto &field : node->fields_) {
    fields_name.push_back(field.identifier);
    fields_type.push_back(getType(field.type.get()));
    auto type_def = std::make_shared<TypeDef>(node->ID_, fields_name,
                                              std::move(fields_type));
    auto symbol_info =
        std::make_shared<SymbolTypeInfo>(node->ID_, std::move(type_def));
    current_scope_->addType(field.identifier, std::move(symbol_info));
  }
}

void SymbolCollector::visit(ItemEnumNode *node) {}

void SymbolCollector::visit(ItemImplNode *node) {
  if (!is_instance_of<TypePathNode, TypeNode>(node->type_.get())) {
    throw std::runtime_error("Impl type must be a path type");
  }
  const auto *type_path = dynamic_cast<const TypePathNode *>(node->type_.get());
  std::string type_name = getPathIndexName(type_path->path_.get(), 0);
  for (const auto &item : node->items_) {
    if (item.function) {
      auto func = getFunc(item.function.get());
      current_scope_->addImplSymbol(type_name, func->getName(),
                                    std::move(func));
    } else {
      throw std::runtime_error(
          "ItemImplNode currently does not support constants");
    }
  }
}

void SymbolCollector::visit(ItemTraitNode *node) {}

void SymbolCollector::visit(ExprArrayNode *node) {}
void SymbolCollector::visit(ExprArrayIndexNode *node) {}

void SymbolCollector::visit(ExprBlockNode *node) {
  for (auto &stmt : node->statements_) {
    stmt->accept(*this);
  }
}
void SymbolCollector::visit(ExprBlockConstNode *node) {}

void SymbolCollector::visit(ExprCallNode *node) {}
void SymbolCollector::visit(ExprBreakNode *node) {}
void SymbolCollector::visit(ExprReturnNode *node) {}
void SymbolCollector::visit(ExprContinueNode *node) {}
void SymbolCollector::visit(ExprGroupNode *node) {}
void SymbolCollector::visit(ExprIfNode *node) {}
void SymbolCollector::visit(ExprLiteralIntNode *node) {}
void SymbolCollector::visit(ExprLiteralBoolNode *node) {}
void SymbolCollector::visit(ExprLiteralCharNode *node) {}
void SymbolCollector::visit(ExprLiteralStringNode *node) {}
void SymbolCollector::visit(ExprLiteralByteNode *node) {}
void SymbolCollector::visit(ExprLoopNode *node) {}
void SymbolCollector::visit(ExprWhileNode *node) {}
void SymbolCollector::visit(ExprOperBinaryNode *node) {}
void SymbolCollector::visit(ExprOperUnaryNode *node) {}
void SymbolCollector::visit(ExprPathNode *node) {}
void SymbolCollector::visit(ExprFieldNode *node) {}
void SymbolCollector::visit(ExprMethodNode *node) {}
void SymbolCollector::visit(ExprMatchNode *node) {}
void SymbolCollector::visit(ExprStructNode *node) {}
void SymbolCollector::visit(ExprTupleNode *node) {}
void SymbolCollector::visit(ExprTupleIndexNode *node) {}

void SymbolCollector::visit(StmtExprNode *node) {}

void SymbolCollector::visit(StmtEmptyNode *node) {}

void SymbolCollector::visit(StmtItemNode *node) {}

void SymbolCollector::visit(StmtLetNode *node) {}

void SymbolCollector::visit(PatternLiteralNode *node) {}
void SymbolCollector::visit(PatternStructNode *node) {}
void SymbolCollector::visit(PatternTupleNode *node) {}
void SymbolCollector::visit(PatternWildNode *node) {}
void SymbolCollector::visit(PatternPathNode *node) {}
void SymbolCollector::visit(PatternIDNode *node) {}

void SymbolCollector::visit(TypeArrayNode *node) {}
void SymbolCollector::visit(TypeSliceNode *node) {}
void SymbolCollector::visit(TypeInferNode *node) {}
void SymbolCollector::visit(TypePathNode *node) {}

void SymbolCollector::visit(PathNode *node) {}

auto SymbolCollector::getType(const TypeNode *type_node)
    -> std::shared_ptr<TypeKind> {
  if (is_instance_of<TypePathNode, TypeNode>(type_node)) {
    const auto *type_path = dynamic_cast<const TypePathNode *>(type_node);
    std::string type_name = getPathIndexName(type_path->path_.get(), 0);
    return std::make_shared<TypeKind>(
        current_scope_->getType(type_name)->getType());
  } else if (is_instance_of<TypeInferNode, TypeNode>(type_node)) {
    return std::make_shared<TypeKind>(
        current_scope_->getType("infer")->getType());
  } else if (is_instance_of<TypeArrayNode, TypeNode>(type_node)) {
    const auto *type_array = dynamic_cast<const TypeArrayNode *>(type_node);
    auto type = SymbolCollector::getType(type_array->type_.get());
    int32_t length;
    if (type_array->length_ != nullptr &&
        is_instance_of<ExprLiteralIntNode, ExprNode>(type_array->length_)) {
      //编译期求值。
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

auto SymbolCollector::getFunc(const ItemFnNode *node)
    -> std::shared_ptr<SymbolFunctionInfo> {
  auto return_type = getType(node->return_type_.get());
  std::vector<std::shared_ptr<TypeKind>> parameters;
  for (const auto &param : node->parameters_) {
    parameters.push_back(getType(param.type.get()));
  }
  return std::make_shared<SymbolFunctionInfo>(node->ID_, std::move(return_type),
                                              std::move(parameters));
}

auto SymbolCollector::getPathIndexName(const PathNode *path_node,
                                       uint32_t index) -> std::string {
  if (index >= path_node->segments_.size()) {
    throw std::out_of_range("Index out of range for path segments");
  }
  return path_node->segments_[index].name;
}