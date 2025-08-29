#include "ValueInfo.hpp"

ValueInfo::ValueInfo(std::shared_ptr<TypeKind> type, bool is_left_value,
                     bool is_mut)
    : type_(std::move(type)), is_left_value_(is_left_value),
      is_mutable_(is_mut) {}

ValueInfo::~ValueInfo() = default;

auto ValueInfo::getType() const -> std::shared_ptr<TypeKind> { return type_; }

auto ValueInfo::isLeftValue() const -> bool { return is_left_value_; }

void ValueInfo::setLeftValue(bool is_left_value) {
  is_left_value_ = is_left_value;
}

auto ValueInfo::isMutable() const -> bool { return is_mutable_; }

void ValueInfo::setMutable(bool is_mutable) { is_mutable_ = is_mutable; }