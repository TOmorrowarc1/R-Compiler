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
  bool is_const_;

public:
  ValueInfo(std::shared_ptr<TypeKind> &&type, bool is_left_value,
            bool is_mutable, bool is_const);
  ~ValueInfo();
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto isLeftValue() const -> bool;
  void setLeftValue(bool is_left_value);
  auto isMutable() const -> bool;
  void setMutable(bool is_mutable);
  auto isConst() const -> bool;
  void setConst(bool is_const);
};