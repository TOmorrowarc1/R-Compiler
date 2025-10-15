#include "ItemStructNode.hpp"
#include "TypeANode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ItemStructNode::ItemStructNode(const std::string &ID,
                               std::vector<ItemStructField> &&fields,
                               Position position)
    : ItemNode(position), ID_(ID), fields_(std::move(fields)) {}
ItemStructNode::~ItemStructNode() = default;
void ItemStructNode::accept(Visitor &visitor) { visitor.visit(this); }