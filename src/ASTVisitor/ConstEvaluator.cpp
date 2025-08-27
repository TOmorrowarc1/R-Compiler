#include "ConstEvaluator.hpp"
#include "ASTNodeTotal.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include "cast.hpp"
#include "exception.hpp"

ConstEvaluator::ConstEvaluator(Scope *initial_scope)
    : current_scope_(initial_scope) {}
ConstEvaluator::~ConstEvaluator() = default;

auto ConstEvaluator::getPathIndexName(const PathNode *path_node, uint32_t index)
    -> std::string {
  if (index >= path_node->segments_.size()) {
    throw std::out_of_range("Index out of range for path segments");
  }
  return path_node->segments_[index].name;
}

auto ConstEvaluator::judgeNum(const ExprNode *node) -> bool {
  auto type = node->value_info_->getType();
  return type->isTypePath(current_scope_->getType("i32")->getType().get()) ||
         type->isTypePath(current_scope_->getType("u32")->getType().get());
}

void ConstEvaluator::visit(ASTRootNode *node) {}

void ConstEvaluator::visit(ItemConstNode *node) {
  Position position = node->position_;
  node->value_->accept(*this);
  auto const_symbol = current_scope_->getSymbol(node->ID_);
  auto const_info = std::dynamic_pointer_cast<SymbolConstInfo>(const_symbol);
  if (!const_info) {
    throw CompilerException("Const symbol info is null.", position);
  }
  const_info->setValue(node->value_->const_value_->getValue());
}

void ConstEvaluator::visit(ItemFnNode *node) {}
void ConstEvaluator::visit(ItemStructNode *node) {}
void ConstEvaluator::visit(ItemEnumNode *node) {}
void ConstEvaluator::visit(ItemImplNode *node) {}
void ConstEvaluator::visit(ItemTraitNode *node) {}

void ConstEvaluator::visit(ExprNode *node) {
  if (is_instance_of<ExprLiteralIntNode, ExprNode>(node)) {
    visit(dynamic_cast<ExprLiteralIntNode *>(node));
  } else if (is_instance_of<ExprOperBinaryNode, ExprNode>(node)) {
    visit(dynamic_cast<ExprOperBinaryNode *>(node));
  } else if (is_instance_of<ExprOperUnaryNode, ExprNode>(node)) {
    visit(dynamic_cast<ExprOperUnaryNode *>(node));
  } else if (is_instance_of<ExprPathNode, ExprNode>(node)) {
    visit(dynamic_cast<ExprPathNode *>(node));
  } else {
    throw CompilerException("Unsupported expression in constant evaluation",
                            node->position_);
  }
}
void ConstEvaluator::visit(ExprArrayNode *node) {}
void ConstEvaluator::visit(ExprArrayIndexNode *node) {}
void ConstEvaluator::visit(ExprBlockNode *node) {}
void ConstEvaluator::visit(ExprBlockConstNode *node) {}
void ConstEvaluator::visit(ExprCallNode *node) {}
void ConstEvaluator::visit(ExprBreakNode *node) {}
void ConstEvaluator::visit(ExprReturnNode *node) {}
void ConstEvaluator::visit(ExprContinueNode *node) {}
void ConstEvaluator::visit(ExprGroupNode *node) {}
void ConstEvaluator::visit(ExprIfNode *node) {}

void ConstEvaluator::visit(ExprLiteralIntNode *node) {
  node->const_value_ = std::make_unique<ConstValueInfo>(node->value_);
}

void ConstEvaluator::visit(ExprLiteralBoolNode *node) {}
void ConstEvaluator::visit(ExprLiteralCharNode *node) {}
void ConstEvaluator::visit(ExprLiteralStringNode *node) {}
void ConstEvaluator::visit(ExprLoopNode *node) {}
void ConstEvaluator::visit(ExprWhileNode *node) {}

void ConstEvaluator::visit(ExprOperBinaryNode *node) {
  Position position = node->position_;
  node->lhs_->accept(*this);
  node->rhs_->accept(*this);
  auto lhs_value = node->lhs_->const_value_->getValue();
  auto rhs_value = node->rhs_->const_value_->getValue();
  std::unique_ptr<ConstValueInfo> result;
  switch (node->op_) {
  case BinaryOperator::PLUS:
    result = std::make_unique<ConstValueInfo>(lhs_value + rhs_value);
    break;
  case BinaryOperator::MINUS:
    result = std::make_unique<ConstValueInfo>(lhs_value - rhs_value);
    break;
  case BinaryOperator::MUL:
    result = std::make_unique<ConstValueInfo>(lhs_value * rhs_value);
    break;
  case BinaryOperator::DIV:
    if (rhs_value == 0) {
      throw CompilerException("Division by zero in constant expression",
                              position);
    }
    result = std::make_unique<ConstValueInfo>(lhs_value / rhs_value);
    break;
  case BinaryOperator::MOD:
    if (rhs_value == 0) {
      throw CompilerException("Modulo by zero in constant expression",
                              position);
    }
    result = std::make_unique<ConstValueInfo>(lhs_value % rhs_value);
    break;
  case BinaryOperator::XOR:
    result = std::make_unique<ConstValueInfo>(lhs_value ^ rhs_value);
    break;
  case BinaryOperator::AND:
    result = std::make_unique<ConstValueInfo>(lhs_value & rhs_value);
    break;
  case BinaryOperator::OR:
    result = std::make_unique<ConstValueInfo>(lhs_value | rhs_value);
    break;
  default:
    throw CompilerException(
        "Unsupported binary operator in constant expression", position);
  }
  node->const_value_ = std::move(result);
}

void ConstEvaluator::visit(ExprOperUnaryNode *node) {
  Position position = node->position_;
  node->operand_->accept(*this);
  auto operand_value = node->operand_->const_value_->getValue();
  std::unique_ptr<ConstValueInfo> result;
  switch (node->op_) {
  case UnaryOperator::NEGATE:
    result = std::make_unique<ConstValueInfo>(-operand_value);
    break;
  case UnaryOperator::NOT:
    result = std::make_unique<ConstValueInfo>(!operand_value);
    break;
  default:
    throw CompilerException("Unsupported unary operator in constant expression",
                            position);
  }
  node->const_value_ = std::move(result);
}

void ConstEvaluator::visit(ExprPathNode *node) {
  auto const_name = getPathIndexName(node->path_.get(), 0);
  auto const_symbol = current_scope_->getSymbol(const_name);
  auto const_info = std::dynamic_pointer_cast<SymbolConstInfo>(const_symbol);
  if (!const_info) {
    throw CompilerException("Const symbol info is null.", node->position_);
  }
  auto const_value = const_info->calcValue();
  if (!const_value.second) {
    throw CompilerException("Const value has not been calculated.",
                            node->position_);
  }
  node->const_value_ = std::make_unique<ConstValueInfo>(const_value.first);
}
void ConstEvaluator::visit(ExprFieldNode *node) {}
void ConstEvaluator::visit(ExprMethodNode *node) {}
void ConstEvaluator::visit(ExprStructNode *node) {}
void ConstEvaluator::visit(ExprUnderScoreNode *node) {}

void ConstEvaluator::visit(StmtExprNode *node) {}
void ConstEvaluator::visit(StmtEmptyNode *node) {}
void ConstEvaluator::visit(StmtItemNode *node) {}
void ConstEvaluator::visit(StmtLetNode *node) {}

void ConstEvaluator::visit(PatternLiteralNode *node) {}
void ConstEvaluator::visit(PatternWildNode *node) {}
void ConstEvaluator::visit(PatternPathNode *node) {}
void ConstEvaluator::visit(PatternIDNode *node) {}
void ConstEvaluator::visit(PatternReferNode *node) {}

void ConstEvaluator::visit(TypeArrayNode *node) {}
void ConstEvaluator::visit(TypePathNode *node) {}
void ConstEvaluator::visit(TypeReferNode *node) {}

void ConstEvaluator::visit(PathNode *node) {}