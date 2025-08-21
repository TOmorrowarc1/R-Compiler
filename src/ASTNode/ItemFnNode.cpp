#include "ItemFnNode.hpp"
#include "ExprBlockNode.hpp"
#include "PatternANode.hpp"
#include "TypeANode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ItemFnNode::ItemFnNode(const std::string &ID,
                       std::unique_ptr<TypeNode> &&return_type,
                       std::vector<ItemFnPara> &&parameters,
                       std::unique_ptr<ExprBlockNode> &&body, FnType fn_type,
                       bool const_flag, Position position)
    : ItemNode(position), ID_(ID), return_type_(std::move(return_type)),
      parameters_(std::move(parameters)), body_(std::move(body)),
      fn_type_(fn_type), const_flag_(const_flag) {}
ItemFnNode::~ItemFnNode() = default;
void ItemFnNode::accept(Visitor &visitor) { visitor.visit(this); }