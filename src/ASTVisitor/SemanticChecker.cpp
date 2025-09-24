#include "SemanticChecker.hpp"
#include "ASTNodeTotal.hpp"
#include "ConstEvaluator.hpp"
#include "ConstInfo.hpp"
#include "ConstValue.hpp"
#include "Symbol.hpp"
#include "TraitDef.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include "exception.hpp"

SemanticChecker::SemanticChecker(Scope *initial_scope,
                                 ConstEvaluator *const_evaluator)
    : current_scope_(initial_scope), const_evaluator_(const_evaluator) {
  const_evaluator_->bindScopePointer(&current_scope_);
}
SemanticChecker::~SemanticChecker() = default;

auto SemanticChecker::judgeTypeEqual(const std::string &type_name,
                                     const TypeKind *node) -> bool {
  if (type_name == "num") {
    auto i32_type = current_scope_->getType("i32")->getType();
    auto u32_type = current_scope_->getType("u32")->getType();
    auto isize_type = current_scope_->getType("isize")->getType();
    auto usize_type = current_scope_->getType("usize")->getType();
    auto type_vector = std::vector<std::shared_ptr<TypeDef>>{
        i32_type, u32_type, isize_type, usize_type};
    auto possi_type = std::make_shared<TypeKindPossi>(std::move(type_vector));
    return node->isEqual(possi_type.get());
  }
  auto target_type = current_scope_->getType(type_name)->getType();
  return node->isTypePath(target_type.get());
}

auto SemanticChecker::judgeTypeEqual(const TypeKind *lhs, const TypeKind *rhs,
                                     bool allow_cast) -> bool {
  if (allow_cast && is_instance_of<TypeKindRefer, TypeKind>(rhs)) {
    auto rhs_refer = dynamic_cast<const TypeKindRefer *>(rhs);
    return rhs_refer->isEqual(lhs) || rhs_refer->canCast(lhs);
  }
  return rhs->isEqual(lhs);
}

auto SemanticChecker::canAssign(const TypeKind *lhs, const ExprNode *rhs,
                                bool allow_cast) -> bool {
  if (rhs == nullptr) {
    return judgeTypeEqual("unit", lhs);
  }
  auto rhs_type = rhs->value_info_->getType();
  if (judgeTypeEqual("i32", lhs) || judgeTypeEqual("isize", lhs)) {
    if (is_instance_of<ExprLiteralIntNode, ExprNode>(rhs)) {
      const auto *int_node = dynamic_cast<const ExprLiteralIntNode *>(rhs);
      if (int_node->value_ > INT32_MAX) {
        return false;
      }
    }
  }
  if (judgeTypeEqual("u32", lhs) || judgeTypeEqual("usize", lhs)) {
    if (is_instance_of<ExprLiteralIntNode, ExprNode>(rhs)) {
      const auto *int_node = dynamic_cast<const ExprLiteralIntNode *>(rhs);
      if (int_node->value_ > UINT32_MAX) {
        return false;
      }
    }
  }
  return judgeTypeEqual(lhs, rhs_type.get(), allow_cast);
}

auto SemanticChecker::canAssign(const std::string &type_name,
                                const ExprNode *rhs, bool allow_cast) -> bool {
  auto rhs_type = rhs->value_info_->getType();
  if (type_name == "-i32") {
    if (is_instance_of<ExprLiteralIntNode, ExprNode>(rhs)) {
      const auto *int_node = dynamic_cast<const ExprLiteralIntNode *>(rhs);
      if (int_node->value_ > uint32_t(INT32_MAX) + 1) {
        return false;
      }
    }
    return judgeTypeEqual("i32", rhs_type.get());
  }
  if (type_name == "i32" || type_name == "isize") {
    if (is_instance_of<ExprLiteralIntNode, ExprNode>(rhs)) {
      const auto *int_node = dynamic_cast<const ExprLiteralIntNode *>(rhs);
      if (int_node->value_ > INT32_MAX) {
        return false;
      }
    }
  }
  if (type_name == "u32" || type_name == "usize") {
    if (is_instance_of<ExprLiteralIntNode, ExprNode>(rhs)) {
      const auto *int_node = dynamic_cast<const ExprLiteralIntNode *>(rhs);
      if (int_node->value_ > UINT32_MAX) {
        return false;
      }
    }
  }
  return judgeTypeEqual(type_name, rhs_type.get());
}

auto SemanticChecker::judgeValueMutable(const ValueInfo *value_info) -> bool {
  auto type = value_info->getType();
  auto refer_type = dynamic_shared_ptr_cast<TypeKindRefer>(type);
  if (refer_type != nullptr) {
    return refer_type->isMutRef();
  }
  return value_info->isMutable();
}

auto SemanticChecker::bindVarSymbol(const PatternNode *pattern_node,
                                    std::shared_ptr<TypeKind> type) -> bool {
  if (is_instance_of<PatternIDNode, PatternNode>(pattern_node)) {
    const auto *id_pattern = dynamic_cast<const PatternIDNode *>(pattern_node);
    bool is_mutable = id_pattern->id_type_ == PatternIDNode::IDType::MUT;
    auto var_info = std::make_shared<SymbolVariableInfo>(
        id_pattern->identifier_, type, is_mutable);
    return current_scope_->addVarible(id_pattern->identifier_, var_info);
  }
  if (is_instance_of<PatternReferNode, PatternNode>(pattern_node)) {
    const auto *refer_pattern =
        dynamic_cast<const PatternReferNode *>(pattern_node);
    if (!is_instance_of<TypeKindRefer, TypeKind>(type.get())) {
      throw std::runtime_error("Pattern refer type must be a reference type");
    }
    auto refer_type = std::dynamic_pointer_cast<TypeKindRefer>(type);
    if (refer_type->isMutRef() != refer_pattern->is_mutable_) {
      throw std::runtime_error("Pattern refer type mismatch");
    }
    return bindVarSymbol(refer_pattern->pattern_.get(), refer_type->getType());
  }
  if (is_instance_of<PatternWildNode, PatternNode>(pattern_node)) {
    return true;
  }
  throw std::runtime_error("Patterns in function paras must be irrefutable.");
  return false;
}

auto SemanticChecker::fnNodeToFunc(const ItemFnNode *node)
    -> std::shared_ptr<SymbolFunctionInfo> {
  std::vector<std::shared_ptr<TypeKind>> parameters;
  auto return_type = const_evaluator_->evaluateType(node->return_type_.get());
  for (const auto &param : node->parameters_) {
    parameters.push_back(const_evaluator_->evaluateType(param.type.get()));
  }
  return std::make_shared<SymbolFunctionInfo>(node->ID_, return_type,
                                              std::move(parameters));
}

auto SemanticChecker::implCheck(ItemImplNode *node) -> bool {
  Position position = node->position_;
  auto target_trait = current_scope_->getTrait(node->trait_name_)->getTrait();
  auto all_consts = target_trait->getAllConst();
  auto all_functions = target_trait->getAllFunction();
  if (is_instance_of<TypePathNode, TypeNode>(node->type_)) {
    auto type_path = dynamic_cast<const TypePathNode *>(node->type_.get());
    auto type_name = type_path->path_->getPathIndexName(0);
    auto type_def = current_scope_->getType(type_name)->getType();
    for (auto &item : node->items_) {
      if (item.constant) {
        auto impl_const = type_def->getConst(item.constant->ID_);
        auto iter = all_consts.find(item.constant->ID_);
        if (iter == all_consts.end()) {
          throw CompilerException(
              "Impl does not have such const: " + item.constant->ID_, position);
        }
        if (!iter->second->getType()->isEqual(impl_const->getType().get())) {
          throw CompilerException(
              "Const type mismatch for const: " + item.constant->ID_, position);
        }
        all_consts.erase(iter);
      } else {
        auto impl_func = fnNodeToFunc(item.function.get());
        auto iter = all_functions.find(item.function->ID_);
        if (iter == all_functions.end()) {
          throw CompilerException("Impl does not have such function: " +
                                      item.function->ID_,
                                  position);
        }
        if (!iter->second->isEqual(impl_func.get())) {
          throw CompilerException("Function signature mismatch for function: " +
                                      item.function->ID_,
                                  position);
        }
        all_functions.erase(iter);
      }
    }
  } else {
    throw std::runtime_error("Impl type must be a path type");
  }
  return all_consts.empty() && all_functions.empty();
}

auto SemanticChecker::breakCheck(const ExprNode *node) -> bool {
  auto loop_type = loop_type_stack_.top();
  auto unit_type = current_scope_->getType("unit")->getType();
  auto value_type = node == nullptr ? std::make_shared<TypeKindPath>(unit_type)
                                    : node->value_info_->getType();
  if (loop_type == nullptr) {
    loop_type_stack_.top() = value_type;
    return true;
  }
  return canAssign(loop_type.get(), node, true);
}

auto SemanticChecker::exitCheck(const ExprNode *node) -> bool {
  auto exit_func = dynamic_cast<const ExprCallNode *>(node);
  if (exit_func == nullptr ||
      !is_instance_of<ExprPathNode, ExprNode>(exit_func->caller_.get())) {
    return false;
  }
  auto path =
      static_cast<ExprPathNode *>(exit_func->caller_.get())->path_.get();
  if (path->segments_.size() != 1 || path->getPathIndexName(0) != "exit") {
    return false;
  }
  return true;
}

void SemanticChecker::visit(ASTRootNode *node) {
  int32_t main_count = 0;
  for (auto &item : node->items_) {
    if (is_instance_of<ItemFnNode, ItemNode>(item.get())) {
      auto fn_node = static_cast<ItemFnNode *>(item.get());
      if (fn_node->ID_ == "main" && fn_node->fn_type_ == FnType::Fn) {
        ++main_count;
        exit_checker_.enterMain();
      }
    }
    item->accept(*this);
    exit_checker_.exitMain();
  }
  if (main_count != 1) {
    throw std::runtime_error("No or more than one main function found.");
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
  if (node->return_type_) {
    node->return_type_->accept(*this);
  }
  if (node->body_) {
    current_scope_ = current_scope_->getNextChildScope();
    if (node->fn_type_ != FnType::Fn) {
      auto self_type = std::make_shared<TypeKindPath>(current_impl_type_);
      bool is_mut = node->fn_type_ == FnType::MutMethod;
      auto var_info =
          std::make_shared<SymbolVariableInfo>("self", self_type, is_mut);
      current_scope_->addVarible("self", var_info);
    }
    for (auto &param : node->parameters_) {
      if (param.pattern) {
        auto type = const_evaluator_->evaluateType(param.type.get());
        bindVarSymbol(param.pattern.get(), type);
      } else {
        throw std::runtime_error(
            "Parameters must have a pattern in function definitions");
      }
    }
    auto return_type_kind =
        const_evaluator_->evaluateType(node->return_type_.get());
    fn_type_stack_.push(return_type_kind);
    node->body_->accept(*this);
    if (!canAssign(return_type_kind.get(), node->body_.get(), true)) {
      throw std::runtime_error("Function return type mismatch");
    }
    fn_type_stack_.pop();
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
  auto impl_type = const_evaluator_->evaluateType(node->type_.get());
  if (!is_instance_of<TypeKindPath, TypeKind>(impl_type.get())) {
    throw std::runtime_error("Impl type must be a path type");
  }
  auto impl_type_path = std::dynamic_pointer_cast<TypeKindPath>(impl_type);
  current_impl_type_ = impl_type_path->getTypeDef();
  for (const auto &item : node->items_) {
    if (item.function) {
      item.function->accept(*this);
    } else if (item.constant) {
      item.constant->accept(*this);
    } else {
      throw std::runtime_error("Item in Impl cannot be empty.");
    }
  }
  current_impl_type_ = nullptr;
  if (!node->trait_name_.empty()) {
    if (!implCheck(node)) {
      throw std::runtime_error("Impl does not implement the trait completely.");
    }
  }
}

void SemanticChecker::visit(ItemTraitNode *node) {}

void SemanticChecker::visit(ExprArrayNode *node) {
  int32_t length = 0;
  node->elements_[0]->accept(*this);
  auto element_type = node->elements_[0]->value_info_->getType();
  for (const auto &element : node->elements_) {
    element->accept(*this);
    if (!canAssign(element_type.get(), element.get(), true) &&
        !canAssign(element->value_info_->getType().get(),
                   node->elements_[0].get(), true)) {
      throw std::runtime_error("Types of all elements must be same in array.");
    }
  }
  if (node->length_) {
    node->length_->accept(*this);
    auto length_type = node->length_->value_info_->getType();
    if (!canAssign("usize", node->length_.get(), false)) {
      throw std::runtime_error("Array length must be usize.");
    }
    auto length_info = const_evaluator_->evaluateExprValue(node->length_.get())
                           ->getConstValue();
    auto length_value = dynamic_shared_ptr_cast<ConstValueInt>(length_info);
    length = length_value->getValue();
  }
  length = length == 0 ? node->elements_.size() : length;
  auto array_type = std::make_shared<TypeKindArray>(element_type, length);
  // The array expr is neither a left value nor mutable.
  node->value_info_ = std::make_unique<ValueInfo>(array_type, false, false);
}

void SemanticChecker::visit(ExprArrayIndexNode *node) {
  node->array_->accept(*this);
  node->index_->accept(*this);
  auto array_type = node->array_->value_info_->getType();
  if (is_instance_of<TypeKindRefer, TypeKind>(array_type.get())) {
    array_type =
        std::dynamic_pointer_cast<TypeKindRefer>(array_type)->getType();
  }
  if (!is_instance_of<TypeKindArray, TypeKind>(array_type.get())) {
    throw std::runtime_error("Array index operation requires an array type");
  }
  auto index_type = node->index_->value_info_->getType();
  if (!canAssign("usize", node->index_.get(), false)) {
    throw std::runtime_error("Array index must be usize.");
  }
  // ValueInfo in arrayIndexExpr is decided by the array.
  auto element_type =
      std::dynamic_pointer_cast<TypeKindArray>(array_type)->getType();
  node->value_info_ = std::make_unique<ValueInfo>(
      std::move(element_type), node->array_->value_info_->isLeftValue(),
      judgeValueMutable(node->array_->value_info_.get()));
}

void SemanticChecker::visit(ExprBlockNode *node) {
  current_scope_ = current_scope_->getNextChildScope();
  exit_checker_.addScope();
  int32_t length = node->statements_.size();
  for (int32_t i = 0; i < length - 1; ++i) {
    node->statements_[i]->accept(*this);
  }
  if (node->return_value_) {
    if (!node->statements_.empty()) {
      node->statements_.back()->accept(*this);
    }
    exit_checker_.atEnd();
    if (exit_checker_.canExit()) {
      if (!exitCheck(node->return_value_.get())) {
        throw std::runtime_error("Main function must end with exit().");
      }
    }
    node->return_value_->accept(*this);
    exit_checker_.notAtEnd();
    auto return_type = node->return_value_->value_info_->getType();
    node->value_info_ =
        std::make_unique<ValueInfo>(std::move(return_type), false, false);
  } else {
    std::shared_ptr<TypeKind> block_type = std::make_shared<TypeKindPath>(
        current_scope_->getType("unit")->getType());
    exit_checker_.atEnd();
    if (!node->statements_.empty()) {
      node->statements_.back()->accept(*this);
      auto stmt = dynamic_cast<StmtExprNode *>(node->statements_.back().get());
      if (stmt != nullptr) {
        auto expr = dynamic_cast<ExprReturnNode *>(stmt->expr_.get());
        if (expr != nullptr) {
          block_type = std::make_shared<TypeKindNever>();
        }
      }
      if (exit_checker_.canExit()) {
        if (stmt == nullptr) {
          throw std::runtime_error("Main function must end with exit().");
        }
        auto exit_expr = dynamic_cast<ExprCallNode *>(stmt->expr_.get());
        if (exit_expr == nullptr || !exitCheck(exit_expr)) {
          throw std::runtime_error("Main function must end with exit().");
        }
      }
    }
    exit_checker_.notAtEnd();
    node->value_info_ =
        std::make_unique<ValueInfo>(std::move(block_type), false, false);
  }
  exit_checker_.removeScope();
  current_scope_ = current_scope_->getParent();
}

void SemanticChecker::visit(ExprBlockConstNode *node) {
  node->block_expr_->accept(*this);
  auto block_type = node->block_expr_->value_info_->getType();
  node->value_info_ =
      std::make_unique<ValueInfo>(std::move(block_type), false, false);
}

void SemanticChecker::visit(ExprCallNode *node) {
  if (!is_instance_of<ExprPathNode, ExprNode>(node->caller_.get())) {
    throw std::runtime_error("Function call requires a path as caller");
  }
  // Located the function in the current scope.
  auto path = dynamic_cast<ExprPathNode *>(node->caller_.get())->path_.get();
  std::shared_ptr<SymbolFunctionInfo> function_info;
  if (path->segments_.size() == 1) {
    auto function_name = path->getPathIndexName(0);
    if (function_name == "exit" && !exit_checker_.canExit()) {
      // Special check for exit function.
      throw std::runtime_error("Cannot call exit() here.");
    }
    auto function = current_scope_->getSymbol(function_name);
    if (!function ||
        !is_instance_of<SymbolFunctionInfo, SymbolInfo>(function.get())) {
      throw std::runtime_error("Function " + function_name + " not found");
    }
    function_info = std::dynamic_pointer_cast<SymbolFunctionInfo>(function);
  } else {
    auto type_name = path->getPathIndexName(0);
    std::shared_ptr<TypeDef> type_def;
    if (type_name == "Self") {
      if (!current_impl_type_) {
        throw std::runtime_error("Cannot use Self outside of impl block.");
      }
      type_def = current_impl_type_;
    } else {
      type_def = current_scope_->getType(type_name)->getType();
    }
    auto function = type_def->getAssociatedFunction(path->segments_[1].name);
    function_info = function;
  }
  // Check the number and types of parameters.
  if (function_info->getParametersType().size() != node->arguments_.size()) {
    throw std::runtime_error("Function call parameters count mismatch");
  }
  for (int i = 0; i < node->arguments_.size(); ++i) {
    node->arguments_[i]->accept(*this);
    auto param_type = function_info->getParametersType()[i];
    if (!canAssign(param_type.get(), node->arguments_[i].get(), true)) {
      throw std::runtime_error("Function call parameters type mismatch");
    }
  }
  /*
  A function call returns a value, which is neither a left value nor mutable,
  and whether it is const depends on the function's qualifier.
  */
  node->value_info_ =
      std::make_unique<ValueInfo>(function_info->getReturnType(), false, false);
}

void SemanticChecker::visit(ExprBreakNode *node) {
  /*
  The return value of control flow is same as the one from block expr.
  */
  if (loop_type_stack_.empty()) {
    throw std::runtime_error("Break statement not in a loop");
  }
  if (node->value_) {
    node->value_->accept(*this);
  }
  breakCheck(node->value_.get());
  auto never_type = std::make_shared<TypeKindNever>();
  node->value_info_ = std::make_unique<ValueInfo>(never_type, false, false);
}

void SemanticChecker::visit(ExprReturnNode *node) {
  if (fn_type_stack_.empty()) {
    throw std::runtime_error("Return statement not in a function");
  }
  if (node->value_ != nullptr) {
    node->value_->accept(*this);
  }
  if (!canAssign(fn_type_stack_.top().get(), node->value_.get(), true)) {
    throw std::runtime_error("Return type mismatch");
  }
  auto never_type = std::make_shared<TypeKindNever>();
  node->value_info_ = std::make_unique<ValueInfo>(never_type, false, false);
}

void SemanticChecker::visit(ExprContinueNode *node) {
  if (loop_type_stack_.empty()) {
    throw std::runtime_error("Continue statement not in a loop");
  }
  auto unit_type = std::make_shared<TypeKindPath>(
      current_scope_->getType("unit")->getType());
  node->value_info_ = std::make_unique<ValueInfo>(unit_type, false, false);
}

void SemanticChecker::visit(ExprGroupNode *node) {
  node->expr_->accept(*this);
  auto expr_type = node->expr_->value_info_->getType();
  node->value_info_ =
      std::make_unique<ValueInfo>(std::move(expr_type), false, false);
}

void SemanticChecker::visit(ExprIfNode *node) {
  node->condition_->accept(*this);
  auto bool_type = current_scope_->getType("bool")->getType().get();
  if (!node->condition_->value_info_->getType()->isTypePath(bool_type)) {
    throw std::runtime_error("If condition must be a boolean");
  }
  node->then_block_->accept(*this);
  auto then_type = node->then_block_->value_info_->getType();
  if (node->else_block_) {
    node->else_block_->accept(*this);
    auto else_type = node->else_block_->value_info_->getType();
    if (!then_type->isEqual(else_type.get())) {
      throw std::runtime_error("Then and else blocks must have the same type");
    }
    auto block_type = is_instance_of<TypeKindNever, TypeKind>(then_type.get())
                          ? else_type
                          : then_type;
    node->value_info_ =
        std::make_unique<ValueInfo>(std::move(block_type), false, false);
  } else {
    auto unit_type = current_scope_->getType("unit")->getType();
    if (!then_type->isTypePath(unit_type.get())) {
      throw std::runtime_error("Then and else blocks must have the same type");
    }
    auto unit_kind = std::make_shared<TypeKindPath>(unit_type);
    node->value_info_ = std::make_unique<ValueInfo>(unit_kind, false, false);
  }
}

void SemanticChecker::visit(ExprLiteralIntNode *node) {
  auto i32_type = current_scope_->getType("i32")->getType();
  auto u32_type = current_scope_->getType("u32")->getType();
  auto isize_type = current_scope_->getType("isize")->getType();
  auto usize_type = current_scope_->getType("usize")->getType();
  std::shared_ptr<TypeKind> type_kind;
  switch (node->int_type_) {
  case ExprLiteralIntNode::IntType::I32:
    type_kind = std::make_shared<TypeKindPath>(i32_type);
    break;
  case ExprLiteralIntNode::IntType::ISIZE:
    type_kind = std::make_shared<TypeKindPath>(isize_type);
    break;
  case ExprLiteralIntNode::IntType::U32:
    type_kind = std::make_shared<TypeKindPath>(u32_type);
    break;
  case ExprLiteralIntNode::IntType::USIZE:
    type_kind = std::make_shared<TypeKindPath>(usize_type);
    break;
  case ExprLiteralIntNode::IntType::NUM:
    type_kind =
        std::make_shared<TypeKindPossi>(std::vector<std::shared_ptr<TypeDef>>{
            i32_type, u32_type, isize_type, usize_type});
    break;
  }
  node->value_info_ =
      std::make_unique<ValueInfo>(std::move(type_kind), false, false);
}

void SemanticChecker::visit(ExprLiteralBoolNode *node) {
  auto bool_type = std::make_shared<TypeKindPath>(
      current_scope_->getType("bool")->getType());
  node->value_info_ =
      std::make_unique<ValueInfo>(std::move(bool_type), false, false);
}

void SemanticChecker::visit(ExprLiteralCharNode *node) {
  auto char_type = std::make_shared<TypeKindPath>(
      current_scope_->getType("char")->getType());
  node->value_info_ =
      std::make_unique<ValueInfo>(std::move(char_type), false, false);
}

void SemanticChecker::visit(ExprLiteralStringNode *node) {
  auto static_string_type =
      std::make_shared<TypeKindPath>(current_scope_->getType("str")->getType());
  auto ref_str_type =
      std::make_shared<TypeKindRefer>(static_string_type, false);
  node->value_info_ =
      std::make_unique<ValueInfo>(std::move(ref_str_type), false, false);
}

void SemanticChecker::visit(ExprLoopNode *node) {
  loop_type_stack_.push(nullptr);
  node->loop_body_->accept(*this);
  auto body_type = loop_type_stack_.top();
  if (!body_type) {
    body_type = std::make_shared<TypeKindNever>();
  }
  loop_type_stack_.pop();
  node->value_info_ =
      std::make_unique<ValueInfo>(std::move(body_type), false, false);
}

void SemanticChecker::visit(ExprWhileNode *node) {
  node->condition_->accept(*this);
  auto bool_type = current_scope_->getType("bool")->getType().get();
  if (!node->condition_->value_info_->getType()->isTypePath(bool_type)) {
    throw std::runtime_error("While condition must be a boolean");
  }
  loop_type_stack_.push(nullptr);
  node->loop_body_->accept(*this);
  auto body_type = loop_type_stack_.top();
  if (!body_type) {
    auto unit_type = current_scope_->getType("unit")->getType();
    body_type = std::make_shared<TypeKindPath>(unit_type);
  }
  loop_type_stack_.pop();
  node->value_info_ =
      std::make_unique<ValueInfo>(std::move(body_type), false, false);
}

void SemanticChecker::visit(ExprOperBinaryNode *node) {
  if (node->op_ == BinaryOperator::AS_CAST) {
    node->lhs_->accept(*this);
    if (!canAssign("num", node->lhs_.get(), false) &&
        !canAssign("bool", node->lhs_.get(), false)) {
      throw std::runtime_error("As-cast only supports numeric types");
    }
    auto rhs_type_node = dynamic_cast<ExprPathNode *>(node->rhs_.get());
    if (rhs_type_node == nullptr) {
      throw std::runtime_error("As-cast rhs must be a type path");
    }
    auto type_name = rhs_type_node->path_->getPathIndexName(0);
    if (type_name != "i32" && type_name != "u32" && type_name != "isize" &&
        type_name != "usize") {
      throw std::runtime_error("As-cast only supports numeric types");
    }
    auto type_def = current_scope_->getType(type_name)->getType();
    node->value_info_ =
        std::make_unique<ValueInfo>(std::make_shared<TypeKindPath>(type_def),
                                    node->lhs_->value_info_->isLeftValue(),
                                    node->lhs_->value_info_->isMutable());
    return;
  }
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
  auto lhs_type = node->lhs_->value_info_->getType();
  auto rhs_type = node->rhs_->value_info_->getType();
  switch (node->op_) {
  case BinaryOperator::ASSIGN: {
    if (!canAssign(lhs_type.get(), node->rhs_.get(), true)) {
      throw std::runtime_error("Assignment requires types to match");
    }
    if (!node->lhs_->value_info_->isLeftValue()) {
      throw std::runtime_error("Assignment requires lhs to be a left value");
    }
    if (!judgeValueMutable(node->lhs_->value_info_.get())) {
      throw std::runtime_error("Assignment requires lhs to be mutable");
    }
    auto unit_type = current_scope_->getType("unit")->getType();
    node->value_info_ = std::make_unique<ValueInfo>(
        std::make_shared<TypeKindPath>(unit_type), true, false);
    break;
  }
  case BinaryOperator::EQUAL:
  case BinaryOperator::NOT_EQUAL:
  case BinaryOperator::LESS_THAN:
  case BinaryOperator::LESS_EQUAL:
  case BinaryOperator::GREATER_THAN:
  case BinaryOperator::GREATER_EQUAL: {
    // TODO: type deduction + range check for numeric types.
    if (!lhs_type->isEqual(rhs_type.get())) {
      throw std::runtime_error("Operands compared have the same type.");
    }
    auto bool_type = current_scope_->getType("bool")->getType();
    node->value_info_ = std::make_unique<ValueInfo>(
        std::make_shared<TypeKindPath>(bool_type), false, false);
    break;
  }
  case BinaryOperator::LOGIC_AND:
  case BinaryOperator::LOGIC_OR: {
    if (!canAssign("bool", node->lhs_.get(), false) ||
        !canAssign("bool", node->rhs_.get(), false)) {
      throw std::runtime_error("Logical operators require boolean operands");
    }
    auto bool_type = current_scope_->getType("bool")->getType();
    node->value_info_ = std::make_unique<ValueInfo>(
        std::make_shared<TypeKindPath>(bool_type), false, false);
    break;
  }
  case BinaryOperator::AND_EQUAL:
  case BinaryOperator::DIV_EQUAL:
  case BinaryOperator::LEFT_SHIFT_EQUAL:
  case BinaryOperator::MINUS_EQUAL:
  case BinaryOperator::MOD_EQUAL:
  case BinaryOperator::MUL_EQUAL:
  case BinaryOperator::PLUS_EQUAL:
  case BinaryOperator::RIGHT_SHIFT_EQUAL:
  case BinaryOperator::XOR_EQUAL:
  case BinaryOperator::OR_EQUAL: {
    if (!node->lhs_->value_info_->isLeftValue()) {
      throw std::runtime_error("Assignment requires lhs to be a left value");
    }
    if (!judgeValueMutable(node->lhs_->value_info_.get())) {
      throw std::runtime_error("Assignment requires lhs to be mutable");
    }
    if (!canAssign("num", node->lhs_.get(), false) ||
        !canAssign("num", node->rhs_.get(), false)) {
      throw std::runtime_error(
          "Calculation binary operation operands must be numeric");
    }
    node->value_info_ =
        std::make_unique<ValueInfo>(std::move(lhs_type), true, true);
    break;
  }
  default:
    // These are numeric calculations.
    if (!canAssign("num", node->lhs_.get(), false) ||
        !canAssign("num", node->rhs_.get(), false)) {
      throw std::runtime_error(
          "Calculation binary operation operands must be numeric");
    }
    if (!canAssign(lhs_type.get(), node->rhs_.get(), true)) {
      throw std::runtime_error(
          "Calculation binary operation operands must have the same type");
    }
    node->value_info_ =
        std::make_unique<ValueInfo>(std::move(lhs_type), false, false);
  }
}

void SemanticChecker::visit(ExprOperUnaryNode *node) {
  node->operand_->accept(*this);
  auto operand_type = node->operand_->value_info_->getType();
  switch (node->op_) {
  case UnaryOperator::NEGATE: {
    if (!canAssign("-i32", node->operand_.get(), false)) {
      throw std::runtime_error("Negation operator requires a i32 operand");
    }
    auto i32_type = current_scope_->getType("i32")->getType();
    node->value_info_ = std::make_unique<ValueInfo>(
        std::make_shared<TypeKindPath>(i32_type), false, false);
    break;
  }
  case UnaryOperator::NOT: {
    if (!canAssign("bool", node->operand_.get(), false) &&
        !canAssign("num", node->operand_.get(), false)) {
      throw std::runtime_error(
          "Not operator requires a boolean or numeric operand");
    }
    node->value_info_ = std::make_unique<ValueInfo>(operand_type, false, false);
    break;
  }
  case UnaryOperator::REF: {
    node->value_info_ = std::make_unique<ValueInfo>(
        std::make_shared<TypeKindRefer>(operand_type, false),
        node->operand_->value_info_->isLeftValue(), false);
    break;
  }
  case UnaryOperator::MUTREF: {
    node->value_info_ = std::make_unique<ValueInfo>(
        std::make_shared<TypeKindRefer>(operand_type, true),
        node->operand_->value_info_->isLeftValue(), false);
    break;
  }
  case UnaryOperator::DEREF: {
    if (!is_instance_of<TypeKindRefer, TypeKind>(operand_type.get())) {
      throw std::runtime_error(
          "Dereference operator requires a reference type");
    }
    auto refer_type = std::dynamic_pointer_cast<TypeKindRefer>(operand_type);
    node->value_info_ = std::make_unique<ValueInfo>(
        refer_type->getType(), node->operand_->value_info_->isLeftValue(),
        refer_type->isMutRef());
    break;
  }
  default:
    throw std::runtime_error("Unsupported unary operator");
  }
}

void SemanticChecker::visit(ExprPathNode *node) {
  if (node->path_->segments_.size() == 2) {
    auto type_name = node->path_->getPathIndexName(0);
    std::shared_ptr<TypeDef> type;
    if (type_name == "Self") {
      if (!current_impl_type_) {
        throw std::runtime_error("Cannot use Self outside of impl block.");
      }
      type = current_impl_type_;
    } else {
      type = current_scope_->getType(type_name)->getType();
    }
    if (is_instance_of<EnumDef, TypeDef>(type.get())) {
      auto enum_def = dynamic_cast<EnumDef *>(type.get());
      auto variant_name = node->path_->getPathIndexName(1);
      if (!enum_def->getVariant(variant_name)) {
        throw std::runtime_error("Enum variant not found in enum.");
      }
      node->value_info_ = std::make_unique<ValueInfo>(
          std::make_shared<TypeKindPath>(type), false, false);
    } else if (is_instance_of<StructDef, TypeDef>(type.get())) {
      // Here we lost the const item.
      auto struct_def = dynamic_cast<StructDef *>(type.get());
      auto const_info = struct_def->getConst(node->path_->getPathIndexName(1));
      if (!const_info) {
        throw std::runtime_error("Const member not found in struct.");
      }
      auto const_type = const_info->getType();
      node->value_info_ = std::make_unique<ValueInfo>(const_type, false, false);
    } else {
      throw std::runtime_error("Path must be a type or enum.");
    }
  } else {
    auto symbol_name = node->path_->getPathIndexName(0);
    auto symbol_info = current_scope_->getSymbol(symbol_name);
    if (!symbol_info) {
      throw std::runtime_error("Symbol " + symbol_name + " not found");
    }
    if (is_instance_of<SymbolVariableInfo, SymbolInfo>(symbol_info.get())) {
      auto variable_info =
          std::dynamic_pointer_cast<SymbolVariableInfo>(symbol_info);
      auto type = variable_info->getType();
      bool is_mutable = variable_info->isMutable();
      node->value_info_ = std::make_unique<ValueInfo>(type, true, is_mutable);
    } else if (is_instance_of<SymbolConstInfo, SymbolInfo>(symbol_info.get())) {
      auto const_info = std::dynamic_pointer_cast<SymbolConstInfo>(symbol_info);
      auto type = const_info->getType();
      node->value_info_ = std::make_unique<ValueInfo>(type, false, false);
    } else {
      throw std::runtime_error("Unsupported symbol type in path expr.");
    }
  }
}

void SemanticChecker::visit(ExprFieldNode *node) {
  node->instance_->accept(*this);
  auto instance_type = node->instance_->value_info_->getType();
  if (is_instance_of<TypeKindRefer, TypeKind>(instance_type.get())) {
    instance_type =
        std::dynamic_pointer_cast<TypeKindRefer>(instance_type)->getType();
  }
  auto path_type = dynamic_cast<TypeKindPath *>(instance_type.get());
  if (!path_type) {
    throw std::runtime_error("Instance requires a path type");
  }
  auto type_def = dynamic_shared_ptr_cast<StructDef>(path_type->getTypeDef());
  if (!type_def) {
    throw std::runtime_error("Field access requires a struct type");
  }
  auto member_type = type_def->getMember(node->ID_);
  if (!member_type) {
    throw std::runtime_error("Field " + node->ID_ + " not found in type");
  }
  bool is_mut = judgeValueMutable(node->instance_->value_info_.get());
  node->value_info_ = std::make_unique<ValueInfo>(member_type, true, is_mut);
}

void SemanticChecker::visit(ExprMethodNode *node) {
  node->instance_->accept(*this);
  auto instance_type = node->instance_->value_info_->getType();
  if (is_instance_of<TypeKindRefer, TypeKind>(instance_type.get())) {
    instance_type =
        std::dynamic_pointer_cast<TypeKindRefer>(instance_type)->getType();
  }
  if (is_instance_of<TypeKindArray, TypeKind>(instance_type.get())) {
    // Specialization dealing with the len(), the only builtin method of
    // array.
    auto array_type = std::dynamic_pointer_cast<TypeKindArray>(instance_type);
    if (node->ID_ == "len" && node->parameters_.size() == 0) {
      auto usize_type = current_scope_->getType("usize")->getType();
      node->value_info_ = std::make_unique<ValueInfo>(
          std::make_shared<TypeKindPath>(usize_type), false, false);
      return;
    }
    throw std::runtime_error("Array has no such method");
  }
  if (is_instance_of<TypeKindPossi, TypeKind>(instance_type.get())) {
    // dealing with constant num builtin-fixed on usize.
    auto usize_type = current_scope_->getType("usize")->getType();
    instance_type = std::make_shared<TypeKindPath>(usize_type);
  }
  auto path_type = dynamic_cast<TypeKindPath *>(instance_type.get());
  if (!path_type) {
    throw std::runtime_error("Instance requires a path type");
  }
  auto type_def = path_type->getTypeDef();
  auto method_info = type_def->getMethod(node->ID_);
  auto target_parameters = method_info->getParametersType();
  if (target_parameters.size() != node->parameters_.size()) {
    throw std::runtime_error("Method call parameters count mismatch");
  }
  for (int i = 0; i < node->parameters_.size(); ++i) {
    node->parameters_[i]->accept(*this);
    if (!canAssign(target_parameters[i].get(), node->parameters_[i].get(),
                   true)) {
      throw std::runtime_error("Method call parameters type mismatch");
    }
  }
  node->value_info_ =
      std::make_unique<ValueInfo>(method_info->getReturnType(), false, false);
}

void SemanticChecker::visit(ExprStructNode *node) {
  std::string name = node->path_->path_->getPathIndexName(0);
  auto type_def = current_scope_->getType(name);
  if (!type_def ||
      !is_instance_of<StructDef, TypeDef>(type_def->getType().get())) {
    throw std::runtime_error("Struct " + name + " not found");
  }
  auto struct_type = dynamic_shared_ptr_cast<StructDef>(type_def->getType());
  for (auto &field : node->fields_) {
    // Sugar TODO: same name exprPath to field name can be omitted.
    field.expr->accept(*this);
    auto field_type = field.expr->value_info_->getType();
    auto member_type = struct_type->getMember(field.ID);
    if (!member_type || !field_type->isEqual(member_type.get())) {
      throw std::runtime_error(field.ID + ": type mismatch in struct.");
    }
    if (node->fields_.size() != struct_type->getMemNum()) {
      throw std::runtime_error("Field count mismatch in struct.");
    }
  }
  auto struct_type_kind = std::make_shared<TypeKindPath>(struct_type);
  node->value_info_ =
      std::make_unique<ValueInfo>(struct_type_kind, true, false);
}

void SemanticChecker::visit(ExprUnderScoreNode *node) {
  auto unit_type = current_scope_->getType("unit")->getType();
  node->value_info_ = std::make_unique<ValueInfo>(
      std::make_shared<TypeKindPath>(unit_type), false, false);
}

void SemanticChecker::visit(StmtEmptyNode *node) {}

void SemanticChecker::visit(StmtItemNode *node) { node->item_->accept(*this); }

void SemanticChecker::visit(StmtLetNode *node) {
  if (is_instance_of<PatternWildNode, PatternNode>(node->pattern_)) {
    if (node->init_value_) {
      node->init_value_->accept(*this);
    }
  } else if (is_instance_of<PatternIDNode, PatternNode>(node->pattern_)) {
    if (!node->type_) {
      throw std::runtime_error("ID pattern no type infer.");
    }
    node->type_->accept(*this);
    auto type = const_evaluator_->evaluateType(node->type_.get());
    bindVarSymbol(node->pattern_.get(), type);
    if (node->init_value_) {
      node->init_value_->accept(*this);
      if (!canAssign(type.get(), node->init_value_.get(), true)) {
        throw std::runtime_error(
            "Type mismatch in let statement initialization");
      }
    }
  } else {
    throw std::runtime_error("Only ID and _ are supported in let stmt.");
  }
}

void SemanticChecker::visit(StmtExprNode *node) { node->expr_->accept(*this); }

void SemanticChecker::visit(PatternLiteralNode *node) {}
void SemanticChecker::visit(PatternWildNode *node) {}
void SemanticChecker::visit(PatternPathNode *node) {}
void SemanticChecker::visit(PatternIDNode *node) {}
void SemanticChecker::visit(PatternReferNode *node) {}

void SemanticChecker::visit(TypeArrayNode *node) {}
void SemanticChecker::visit(TypePathNode *node) {}
void SemanticChecker::visit(TypeReferNode *node) {}
void SemanticChecker::visit(TypeUnitNode *node) {}

void SemanticChecker::visit(PathNode *node) {}