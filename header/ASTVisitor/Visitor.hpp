#include <stdint.h>
class ItemConstantNode;
class ItemNode;

class Visitor {
public:
  virtual ~Visitor() = default;

  virtual void visit(ItemConstantNode *node) = 0;
  virtual void visit(ItemNode *node) = 0;
};