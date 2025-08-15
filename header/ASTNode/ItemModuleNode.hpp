#include "ItemANode.hpp"

class ItemModuleNode : public ItemNode {
private:
  std::string ID_;
  std::vector<std::unique_ptr<ItemNode>> items_;

public:
  ItemModuleNode(const std::string &ID,
                 std::vector<std::unique_ptr<ItemNode>> &&items);
  ~ItemModuleNode();
  void accept(Visitor &visitor) override;
};