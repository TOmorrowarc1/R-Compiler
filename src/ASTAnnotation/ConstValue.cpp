#include "ConstValue.hpp"
#include <stdexcept>

ConstValueInt::ConstValueInt(int32_t value) { value_ = value; }
ConstValueInt::~ConstValueInt() = default;
auto ConstValueInt::clone() const -> std::unique_ptr<ConstValue> {
  return std::make_unique<ConstValueInt>(value_);
}
auto ConstValueInt::getValue() const -> int32_t { return value_; }

ConstValueBool::ConstValueBool(bool value) { value_ = value; }
ConstValueBool::~ConstValueBool() = default;
auto ConstValueBool::clone() const -> std::unique_ptr<ConstValue> {
  return std::make_unique<ConstValueBool>(value_);
}
auto ConstValueBool::getValue() const -> bool { return value_; }

ConstValueChar::ConstValueChar(char value) { value_ = value; }
ConstValueChar::~ConstValueChar() = default;
auto ConstValueChar::clone() const -> std::unique_ptr<ConstValue> {
  return std::make_unique<ConstValueChar>(value_);
}
auto ConstValueChar::getValue() const -> char { return value_; }

ConstValueString::ConstValueString(const std::string &value) { value_ = value; }
ConstValueString::~ConstValueString() = default;
auto ConstValueString::clone() const -> std::unique_ptr<ConstValue> {
  return std::make_unique<ConstValueString>(value_);
}
auto ConstValueString::getValue() const -> const std::string & {
  return value_;
}

ConstValueStruct::ConstValueStruct(
    std::unordered_map<std::string, std::shared_ptr<ConstValue>> &&fields) {
  fields_ = std::move(fields);
}
ConstValueStruct::~ConstValueStruct() = default;
auto ConstValueStruct::clone() const -> std::unique_ptr<ConstValue> {
  return std::make_unique<ConstValueStruct>(std::move(fields_));
}
auto ConstValueStruct::getField(const std::string &name) const
    -> const std::shared_ptr<ConstValue> {
  auto iter = fields_.find(name);
  if (iter == fields_.end()) {
    throw std::runtime_error("Field not found: " + name);
  }
  return iter->second;
}

ConstValueEnum::ConstValueEnum(const std::string &variant) {
  variant_ = variant;
}
ConstValueEnum::~ConstValueEnum() = default;
auto ConstValueEnum::clone() const -> std::unique_ptr<ConstValue> {
  return std::make_unique<ConstValueEnum>(variant_);
}
auto ConstValueEnum::getVariant() const -> const std::string & {
  return variant_;
}

ConstValueArray::ConstValueArray(
    std::vector<std::shared_ptr<ConstValue>> &&elements) {
  elements_ = std::move(elements);
}
ConstValueArray::~ConstValueArray() = default;
auto ConstValueArray::clone() const -> std::unique_ptr<ConstValue> {
  return std::make_unique<ConstValueArray>(std::move(elements_));
}
auto ConstValueArray::getElement(uint32_t index) const
    -> const std::shared_ptr<ConstValue> {
  if (index >= elements_.size()) {
    throw std::runtime_error("Index out of bounds");
  }
  return elements_[index];
}
