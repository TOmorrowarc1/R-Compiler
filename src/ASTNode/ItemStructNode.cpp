#include "ItemStructNode.hpp"
#include "TypeANode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

ItemStructNode::ItemStructNode(const std::string &ID,
                               std::vector<ItemStructField> &&fields)
    : ItemNode(), ID_(ID), fields_(std::move(fields)) {}
ItemStructNode::~ItemStructNode() = default;
void ItemStructNode::accept(Visitor &visitor) { visitor.visit(this); }