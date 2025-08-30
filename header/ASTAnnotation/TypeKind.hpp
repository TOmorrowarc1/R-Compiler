#pragma once
#include "TypeDef.hpp"

/*
The instance of TypeKind represents what type of a var/const/field it is.
It can be a PathType, an ArrayType, a ReferenceType, or a PossibleType that
serves for numbers.
*/

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

class TypeKindPossi : public TypeKind {
private:
  std::vector<std::shared_ptr<TypeDef>> possi_;
  std::shared_ptr<TypeDef> lock_type_;

public:
  TypeKindPossi(std::vector<std::shared_ptr<TypeDef>> &&possi);
  ~TypeKindPossi() override;
  auto isEqual(const TypeKind *other) const -> bool override;
  auto isTypePath(const TypeDef *typeDef) const -> bool override;
  void lockType(std::shared_ptr<TypeDef> type);
  auto getPossi() const -> const std::vector<std::shared_ptr<TypeDef>> &;
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
  bool is_mut_ref_;

public:
  TypeKindRefer(std::shared_ptr<TypeKind> type_kind, bool is_mut);
  ~TypeKindRefer() override;
  auto isEqual(const TypeKind *other) const -> bool override;
  auto isTypePath(const TypeDef *typeDef) const -> bool override;
  auto canCast(const TypeKind *other) const -> bool;
  auto getType() const -> const std::shared_ptr<TypeKind>;
  auto isMutRef() const -> bool;
};