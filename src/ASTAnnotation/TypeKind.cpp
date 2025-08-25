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

TypeKindPossi::TypeKindPossi(std::vector<std::shared_ptr<TypeDef>> &&possi)
    : possi_(std::move(possi)) {}
TypeKindPossi::~TypeKindPossi() = default;
auto TypeKindPossi::isEqual(const TypeKind *other) const -> bool {
  if (const auto *other_type = dynamic_cast<const TypeKindPath *>(other)) {
    if (lock_type_) {
      return lock_type_.get() == other_type->getTypeDef().get();
    }
    for (const auto &p : possi_) {
      if (other_type->isTypePath(p.get())) {
        return true;
      }
    }
  }
  if (const auto *other_type = dynamic_cast<const TypeKindPossi *>(other)) {
    if (lock_type_) {
      return other_type->isTypePath(lock_type_.get());
    }
    for (const auto &p1 : possi_) {
      for (const auto &p2 : other_type->getPossi()) {
        if (p1 == p2) {
          return true;
        }
      }
    }
  }
  return false;
}
auto TypeKindPossi::isTypePath(const TypeDef *typeDef) const -> bool {
  if (lock_type_) {
    return lock_type_.get() == typeDef;
  }
  for (const auto &p : possi_) {
    if (p.get() == typeDef) {
      return true;
    }
  }
  return false;
}
auto TypeKindPossi::getPossi() const
    -> const std::vector<std::shared_ptr<TypeDef>> & {
  return possi_;
}
void TypeKindPossi::lockType(std::shared_ptr<TypeDef> type) {
  lock_type_ = type;
}

TypeKindArray::TypeKindArray(std::shared_ptr<TypeKind> type_kind, uint32_t size)
    : type_kind_(std::move(type_kind)), size(size) {}
TypeKindArray::~TypeKindArray() = default;
auto TypeKindArray::isEqual(const TypeKind *other) const -> bool {
  if (const auto *otherArray = dynamic_cast<const TypeKindArray *>(other)) {
    return type_kind_->isEqual(otherArray->getType().get()) &&
           size == otherArray->size;
  }
  return false;
}
auto TypeKindArray::isTypePath(const TypeDef *typeDef) const -> bool {
  return false;
}
auto TypeKindArray::getType() const -> const std::shared_ptr<TypeKind> {
  if (type_kind_ == nullptr) {
    throw std::runtime_error("TypeKindArray type_kind_ is null");
  }
  return type_kind_;
}
auto TypeKindArray::getSize() const -> uint32_t { return size; }

TypeKindRefer::TypeKindRefer(std::shared_ptr<TypeKind> type_kind,
                             bool is_mutable)
    : TypeKind(), type_kind_(std::move(type_kind)), is_mutable_(is_mutable) {}

TypeKindRefer::~TypeKindRefer() = default;

auto TypeKindRefer::isEqual(const TypeKind *other) const -> bool {
  if (const auto *otherRefer = dynamic_cast<const TypeKindRefer *>(other)) {
    return type_kind_->isEqual(otherRefer->getType().get()) &&
           is_mutable_ == otherRefer->isMutable();
  }
  return false;
}

auto TypeKindRefer::isTypePath(const TypeDef *typeDef) const -> bool {
  return type_kind_->isTypePath(typeDef);
}

auto TypeKindRefer::getType() const -> const std::shared_ptr<TypeKind> {
  if (type_kind_ == nullptr) {
    throw std::runtime_error("TypeKindRefer type_kind_ is null");
  }
  return type_kind_;
}

auto TypeKindRefer::isMutable() const -> bool { return is_mutable_; }