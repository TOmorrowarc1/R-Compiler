#pragma once
#include <memory>
#include <stdint.h>

class TypeKind;
class ConstValue;

class ConstInfo {
private:
  std::shared_ptr<TypeKind> type_;
  std::shared_ptr<ConstValue> const_value_;

public:
  // Two constructors: one for expr, one for const definition.
  ConstInfo(std::shared_ptr<TypeKind> type);
  ConstInfo(std::shared_ptr<TypeKind> type,
            std::shared_ptr<ConstValue> const_value);
  ~ConstInfo();
  auto setConstValue(const ConstInfo *rhs) -> bool;
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto getConstValue() const -> std::shared_ptr<ConstValue>;
};