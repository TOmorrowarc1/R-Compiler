#pragma once
#include <memory>
#include <stdint.h>
#include <string>

class TypeKind;

class ValueInfo {
private:
  std::shared_ptr<TypeKind> type_;
  bool is_left_value_;
  bool is_mutable_;

public:
  ValueInfo(std::shared_ptr<TypeKind> type, bool is_left_value, bool is_mut);
  ~ValueInfo();
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto isLeftValue() const -> bool;
  void setLeftValue(bool is_left_value);
  auto isMutable() const -> bool;
  void setMutable(bool is_mutable);
  auto getValue() const -> int32_t;
  void setValue(int32_t const_value);
};