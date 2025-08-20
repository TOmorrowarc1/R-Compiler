#include "TypeKind.hpp"
#include "TypeDef.hpp"
#include <stdexcept>

TypeKindPath::TypeKindPath(std::shared_ptr<TypeDef> typeDef)
    : typeDef(std::move(typeDef)) {}
TypeKindPath::~TypeKindPath() = default;
auto TypeKindPath::isEqual(const TypeKind *other) const -> bool {
  if (const auto *otherPath = dynamic_cast<const TypeKindPath *>(other)) {
    return typeDef == otherPath->typeDef;
  }
  return false;
}
auto TypeKindPath::isTypePath(const TypeDef *typeDef) const -> bool {
  return this->typeDef.get() == typeDef;
}
auto TypeKindPath::getTypeDef() const -> std::shared_ptr<TypeDef> {
  return typeDef;
}

TypeKindArray::TypeKindArray(std::shared_ptr<TypeKind> type_kind, uint32_t size)
    : type_kind_(std::move(type_kind)), size(size) {}
TypeKindArray::~TypeKindArray() = default;
auto TypeKindArray::isEqual(const TypeKind *other) const -> bool {
  if (const auto *otherArray = dynamic_cast<const TypeKindArray *>(other)) {
    return type_kind_->isEqual(otherArray->getType()) &&
           size == otherArray->size;
  }
  return false;
}
auto TypeKindArray::isTypePath(const TypeDef *typeDef) const -> bool {
  return false;
}
auto TypeKindArray::getType() const -> const TypeKind * {
  if (type_kind_ == nullptr) {
    throw std::runtime_error("TypeKindArray type_kind_ is null");
  }
  return type_kind_.get();
}
auto TypeKindArray::getSize() const -> uint32_t { return size; }
