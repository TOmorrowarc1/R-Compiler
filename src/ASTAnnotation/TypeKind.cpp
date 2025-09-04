#include "TypeKind.hpp"
#include "TypeDef.hpp"
#include <stdexcept>

TypeKindPath::TypeKindPath(std::shared_ptr<TypeDef> typeDef)
    : typeDef(std::move(typeDef)) {}
TypeKindPath::~TypeKindPath() = default;
auto TypeKindPath::isEqual(const TypeKind *other) const -> bool {
  if (const auto *other_path = dynamic_cast<const TypeKindPath *>(other)) {
    return typeDef == other_path->typeDef;
  }
  if (const auto *other_possi = dynamic_cast<const TypeKindPossi *>(other)) {
    return other_possi->isTypePath(typeDef.get());
  }
  if (const auto *other_never = dynamic_cast<const TypeKindNever *>(other)) {
    return true;
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
  if (const auto *other_never = dynamic_cast<const TypeKindNever *>(other)) {
    return true;
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
  if (const auto *other_never = dynamic_cast<const TypeKindNever *>(other)) {
    return true;
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

TypeKindRefer::TypeKindRefer(std::shared_ptr<TypeKind> type_kind, bool is_mut)
    : TypeKind(), type_kind_(std::move(type_kind)), is_mut_ref_(is_mut) {}
TypeKindRefer::~TypeKindRefer() = default;
auto TypeKindRefer::isEqual(const TypeKind *other) const -> bool {
  const auto *other_refer = dynamic_cast<const TypeKindRefer *>(other);
  if (other_refer != nullptr) {
    return type_kind_->isEqual(other_refer->type_kind_.get()) &&
           (is_mut_ref_ || !other_refer->is_mut_ref_);
  }
  if (const auto *other_never = dynamic_cast<const TypeKindNever *>(other)) {
    return true;
  }
  return false;
}
auto TypeKindRefer::isTypePath(const TypeDef *typeDef) const -> bool {
  return false;
}
auto TypeKindRefer::canCast(const TypeKind *other) const -> bool {
  if (const auto *other_refer = dynamic_cast<const TypeKindRefer *>(other)) {
    return (is_mut_ref_ || !other_refer->is_mut_ref_) &&
           type_kind_->isEqual(other_refer);
  }
  if (const auto *other_path = dynamic_cast<const TypeKindPath *>(other)) {
    return type_kind_->isEqual(other_path);
  }
  return false;
}
auto TypeKindRefer::getType() const -> const std::shared_ptr<TypeKind> {
  if (type_kind_ == nullptr) {
    throw std::runtime_error("TypeKindRefer type_kind_ is null");
  }
  return type_kind_;
}
auto TypeKindRefer::isMutRef() const -> bool { return is_mut_ref_; }

TypeKindNever::TypeKindNever() = default;
TypeKindNever::~TypeKindNever() = default;
auto TypeKindNever::isEqual(const TypeKind *other) const -> bool {
  return true;
}
auto TypeKindNever::isTypePath(const TypeDef *typeDef) const -> bool {
  return true;
}