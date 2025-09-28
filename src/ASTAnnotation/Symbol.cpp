#include "Symbol.hpp"
#include "ConstInfo.hpp"
#include "TraitDef.hpp"
#include "TypeDef.hpp"
#include "TypeKind.hpp"
#include <stdexcept>

SymbolVariableInfo::SymbolVariableInfo(const std::string &name,
                                       std::shared_ptr<TypeKind> type,
                                       bool is_mutable)
    : SymbolInfo(), name_(name), type_(std::move(type)),
      is_mutable_(is_mutable) {}
SymbolVariableInfo::~SymbolVariableInfo() = default;
auto SymbolVariableInfo::getName() const -> const std::string & {
  return name_;
}
auto SymbolVariableInfo::getType() const -> std::shared_ptr<TypeKind> {
  return type_;
}
auto SymbolVariableInfo::isMutable() const -> bool { return is_mutable_; }

SymbolFunctionInfo::SymbolFunctionInfo(
    const std::string &name, std::shared_ptr<TypeKind> returnType,
    std::vector<std::shared_ptr<TypeKind>> &&parameters)
    : SymbolInfo(), name_(name), return_type_(std::move(returnType)),
      parameters_(std::move(parameters)), fn_type_(FnType::Normal) {}
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
auto SymbolFunctionInfo::isEqual(const SymbolFunctionInfo *other) const
    -> bool {
  if (other == nullptr) {
    return false;
  }
  if (parameters_.size() != other->parameters_.size()) {
    return false;
  }
  for (size_t i = 0; i < parameters_.size(); ++i) {
    if (!parameters_[i]->isEqual(other->parameters_[i].get())) {
      return false;
    }
  }
  return return_type_->isEqual(other->return_type_.get()) &&
         fn_type_ == other->fn_type_;
}
auto SymbolFunctionInfo::getFnType() const -> FnType { return fn_type_; }
void SymbolFunctionInfo::setFnType(FnType type) { fn_type_ = type; }

SymbolTypeInfo::SymbolTypeInfo(const std::string &name,
                               std::shared_ptr<TypeDef> type)
    : SymbolInfo(), name_(name), type_(std::move(type)) {}
SymbolTypeInfo::~SymbolTypeInfo() = default;
auto SymbolTypeInfo::getName() const -> const std::string & { return name_; }
auto SymbolTypeInfo::getType() const -> std::shared_ptr<TypeDef> {
  return type_;
}

SymbolTraitInfo::SymbolTraitInfo(const std::string &name,
                                 std::shared_ptr<TraitDef> trait)
    : SymbolInfo(), name_(name), trait_(std::move(trait)) {}
SymbolTraitInfo::~SymbolTraitInfo() = default;
auto SymbolTraitInfo::getName() const -> const std::string & { return name_; }
auto SymbolTraitInfo::getTrait() const -> std::shared_ptr<TraitDef> {
  return trait_;
}

SymbolConstInfo::SymbolConstInfo(const std::string &name)
    : SymbolInfo(), name_(name), const_info_(nullptr) {}
SymbolConstInfo::~SymbolConstInfo() = default;
auto SymbolConstInfo::getName() const -> const std::string & { return name_; }
auto SymbolConstInfo::getType() const -> std::shared_ptr<TypeKind> {
  if (const_info_ == nullptr) {
    throw std::runtime_error("ConstInfo is not set for const: " + name_);
  }
  return const_info_->getType();
}
auto SymbolConstInfo::getValue() const -> std::shared_ptr<ConstValue> {
  return const_info_->getConstValue();
}
void SymbolConstInfo::setValue(std::shared_ptr<ConstInfo> &&const_info) {
  const_info_ = std::move(const_info);
}