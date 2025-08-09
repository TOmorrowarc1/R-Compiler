#include "ASTRootNode.hpp"
#include "Visitor.hpp"

ASTRootNode::ASTRootNode(std::vector<std::unique_ptr<ItemNode>> &&items)
    : items_(std::move(items)) {}
    
void ASTRootNode::accept(Visitor &visitor) { visitor.visit(*this); }