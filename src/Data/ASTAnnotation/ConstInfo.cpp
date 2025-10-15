#include "ConstInfo.hpp"
#include "ConstValue.hpp"
#include "TypeKind.hpp"
#include <stdexcept>

ConstInfo::ConstInfo(std::shared_ptr<TypeKind> type) {
  type_ = type;
  const_value_ = nullptr;
}
ConstInfo::ConstInfo(std::shared_ptr<TypeKind> type,
                     std::shared_ptr<ConstValue> const_value) {
  type_ = type;
  const_value_ = const_value;
}
ConstInfo::~ConstInfo() = default;
auto ConstInfo::setConstValue(const ConstInfo *rhs) -> bool {
  if (!rhs->const_value_) {
    return false;
  }
  if (!type_->isEqual(rhs->type_.get())) {
    return false;
  }
  const_value_ = rhs->const_value_->clone();
  return true;
}
auto ConstInfo::getType() const -> std::shared_ptr<TypeKind> { return type_; }
auto ConstInfo::getConstValue() const -> std::shared_ptr<ConstValue> {
  return const_value_;
}
