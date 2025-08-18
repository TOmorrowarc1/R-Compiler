#pragma once
#include "TypeDef.hpp"

class TypeKind {
public:
  TypeKind() = default;
  virtual ~TypeKind() = 0;
  virtual auto isEqual(const TypeKind &other) const -> bool = 0;
};

class TypeKindPath : public TypeKind {
private:
  std::shared_ptr<TypeDef> typeDef;

public:
  TypeKindPath(std::shared_ptr<TypeDef> typeDef);
  ~TypeKindPath() override;
  auto isEqual(const TypeKind &other) const -> bool override;
  auto getTypeDef() const -> std::shared_ptr<TypeDef>;
};

class TypeKindArray : public TypeKind {
private:
  std::shared_ptr<TypeDef> typeDef;
  uint32_t size;
public:
  TypeKindArray(std::shared_ptr<TypeDef> typeDef, uint32_t size);
  ~TypeKindArray() override;
  auto isEqual(const TypeKind &other) const -> bool override;
  auto getTypeDef() const -> std::shared_ptr<TypeDef>;
  auto getSize() const -> uint32_t;
};