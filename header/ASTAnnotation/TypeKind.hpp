#pragma once
#include "TypeDef.hpp"

class TypeKind {
public:
  TypeKind() = default;
  virtual ~TypeKind() = default;
  virtual auto isEqual(const TypeKind *other) const -> bool = 0;
  virtual auto isTypePath(const TypeDef *typeDef) const -> bool = 0;
};

class TypeKindPath : public TypeKind {
private:
  std::shared_ptr<TypeDef> typeDef;

public:
  TypeKindPath(std::shared_ptr<TypeDef> typeDef);
  ~TypeKindPath() override;
  auto isEqual(const TypeKind *other) const -> bool override;
  auto isTypePath(const TypeDef *typeDef) const -> bool override;
  auto getTypeDef() const -> std::shared_ptr<TypeDef>;
};

class TypeKindArray : public TypeKind {
private:
  std::shared_ptr<TypeKind> type_kind_;
  uint32_t size;

public:
  TypeKindArray(std::shared_ptr<TypeKind> type_kind, uint32_t size);
  ~TypeKindArray() override;
  auto isEqual(const TypeKind *other) const -> bool override;
  auto isTypePath(const TypeDef *typeDef) const -> bool override;
  auto getType() const -> const std::shared_ptr<TypeKind>;
  auto getSize() const -> uint32_t;
};

class TypeKindRefer : public TypeKind {
private:
  std::shared_ptr<TypeKind> type_kind_;
  bool is_mutable_;

public:
  TypeKindRefer(std::shared_ptr<TypeKind> type_kind, bool is_mutable);
  ~TypeKindRefer() override;
  auto isEqual(const TypeKind *other) const -> bool override;
  auto isTypePath(const TypeDef *typeDef) const -> bool override;
  auto getType() const -> const std::shared_ptr<TypeKind>;
  auto isMutable() const -> bool;
};