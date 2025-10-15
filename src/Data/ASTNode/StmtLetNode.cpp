#include "StmtLetNode.hpp"
#include "ExprBlockNode.hpp"
#include "PatternANode.hpp"
#include "TypeANode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

StmtLetNode::StmtLetNode(std::unique_ptr<PatternNode> &&pattern,
                         std::unique_ptr<TypeNode> &&type,
                         std::unique_ptr<ExprNode> &&init_value,
                         Position position)
    : StmtNode(position), pattern_(std::move(pattern)), type_(std::move(type)),
      init_value_(std::move(init_value)) {}
StmtLetNode::~StmtLetNode() = default;
void StmtLetNode::accept(Visitor &visitor) { visitor.visit(this); }