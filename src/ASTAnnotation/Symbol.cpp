#include "Symbol.hpp"
#include "TypeDef.hpp"
#include "TypeKind.hpp"
#include <stdexcept>

SymbolVariableInfo::SymbolVariableInfo(const std::string &name,
                                       std::shared_ptr<TypeKind> type)
    : SymbolInfo(), name_(name), type_(std::move(type)) {}
SymbolVariableInfo::~SymbolVariableInfo() = default;
auto SymbolVariableInfo::getName() const -> const std::string & {
  return name_;
}
auto SymbolVariableInfo::getType() const -> std::shared_ptr<TypeKind> {
  return type_;
}

SymbolFunctionInfo::SymbolFunctionInfo(
    const std::string &name, std::shared_ptr<TypeKind> returnType,
    std::vector<std::shared_ptr<TypeKind>> &&parameters)
    : SymbolInfo(), name_(name), return_type_(std::move(returnType)),
      parameters_(std::move(parameters)) {}
SymbolFunctionInfo::~SymbolFunctionInfo() = default;
auto SymbolFunctionInfo::getName() const -> const std::string & {
  return name_;
}
auto SymbolFunctionInfo::getReturnType() const -> std::shared_ptr<TypeKind> {
  return return_type_;
}
auto SymbolFunctionInfo::getParametersType() const
    -> const std::vector<std::shared_ptr<TypeKind>> & {
  return parameters_;
}
void SymbolFunctionInfo::setFnType(FnType type) { fn_type_ = type; }

SymbolTypeInfo::SymbolTypeInfo(const std::string &name,
                               std::shared_ptr<TypeDef> type)
    : SymbolInfo(), name_(name), type_(std::move(type)) {}
SymbolTypeInfo::~SymbolTypeInfo() = default;
auto SymbolTypeInfo::getName() const -> const std::string & { return name_; }
auto SymbolTypeInfo::getType() const -> std::shared_ptr<TypeDef> {
  return type_;
}

SymbolConstInfo::SymbolConstInfo(const std::string &name,
                                 std::shared_ptr<TypeKind> type)
    : SymbolInfo(), name_(name), type_(std::move(type)) {}
SymbolConstInfo::~SymbolConstInfo() = default;
auto SymbolConstInfo::getName() const -> const std::string & { return name_; }
auto SymbolConstInfo::getType() const -> std::shared_ptr<TypeKind> {
  return type_;
}
auto SymbolConstInfo::calcValue() -> std::pair<int32_t, bool> {
  int32_t result = 0;
  if (value_.isValid()) {
    result = value_.getValue();
  } else if (value_.isTouched()) {
    throw std::runtime_error(
        "Cyclic dependency detected when evaluating const " + name_);
  } else {
    value_.touch();
  }
  return std::pair<int32_t, bool>(result, value_.isValid());
}
void SymbolConstInfo::setValue(int32_t value) { value_.setValue(value); }