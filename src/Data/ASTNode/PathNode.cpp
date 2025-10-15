#include "PathNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

PathNode::PathNode(std::vector<PathSegment> &&segments, Position position)
    : ASTNode(position), segments_(std::move(segments)) {}
PathNode::~PathNode() = default;
void PathNode::accept(Visitor &visitor) { visitor.visit(this); }
auto PathNode::getPathIndexName(uint32_t index) const -> std::string {
  if (index >= segments_.size()) {
    throw std::out_of_range("Index out of range for path segments");
  }
  return segments_[index].name;
}
auto PathNode::size() const -> uint32_t { return segments_.size(); }