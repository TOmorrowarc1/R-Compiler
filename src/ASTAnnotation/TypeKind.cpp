#include "TypeKind.hpp"
#include "TypeDef.hpp"

TypeKindPath::TypeKindPath(std::shared_ptr<TypeDef> typeDef)
    : typeDef(std::move(typeDef)) {}
TypeKindPath::~TypeKindPath() = default;
auto TypeKindPath::isEqual(const TypeKind &other) const -> bool {
  if (const auto *otherPath = dynamic_cast<const TypeKindPath *>(&other)) {
    return typeDef == otherPath->typeDef;
  }
  return false;
}
auto TypeKindPath::getTypeDef() const -> std::shared_ptr<TypeDef> {
  return typeDef;
}

TypeKindArray::TypeKindArray(std::shared_ptr<TypeDef> typeDef, uint32_t size)
    : typeDef(std::move(typeDef)), size(size) {}
TypeKindArray::~TypeKindArray() = default;
auto TypeKindArray::isEqual(const TypeKind &other) const -> bool {
  if (const auto *otherArray = dynamic_cast<const TypeKindArray *>(&other)) {
    return typeDef == otherArray->typeDef && size == otherArray->size;
  }
  return false;
}
auto TypeKindArray::getTypeDef() const -> std::shared_ptr<TypeDef> {
  return typeDef;
}
auto TypeKindArray::getSize() const -> uint32_t { return size; }
