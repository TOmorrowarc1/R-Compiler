#pragma once
#include <memory>
#include <stdint.h>

class TypeKind;
class ConstValue;

class ConstInfo {
private:
  std::shared_ptr<TypeKind> type_;
  std::unique_ptr<ConstValue> const_value_;

public:
  ConstInfo(std::shared_ptr<TypeKind> type,
            std::unique_ptr<ConstValue> const_value);
  ~ConstInfo();
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto getConstValue() const -> const std::unique_ptr<ConstValue> &;
};