#include "ASTRootNode.hpp"
#include "ItemANode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ASTRootNode::ASTRootNode(std::vector<std::unique_ptr<ItemNode>> &&items,
                         Position position)
    : ASTNode(position), items_(std::move(items)) {}
ASTRootNode::~ASTRootNode() = default;
void ASTRootNode::accept(Visitor &visitor) { visitor.visit(this); }