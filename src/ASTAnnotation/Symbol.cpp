#include "Symbol.hpp"
#include "TypeDef.hpp"
#include "TypeKind.hpp"

SymbolVariableInfo::SymbolVariableInfo(const std::string &name,
                                       std::shared_ptr<TypeKind> type)
    : SymbolInfo(), name_(std::move(name)), type_(std::move(type)) {}
SymbolVariableInfo::~SymbolVariableInfo() = default;
auto SymbolVariableInfo::getName() const -> const std::string & {
  return name_;
}
auto SymbolVariableInfo::getType() const -> std::shared_ptr<TypeKind> {
  return type_;
}
auto SymbolVariableInfo::addRef() -> bool {
  if (mut_ref_) {
    return false;
  }
  ref_count_++;
  return true;
}
auto SymbolVariableInfo::addMutRef() -> bool {
  if (ref_count_ > 0) {
    return false;
  }
  mut_ref_ = true;
  ref_count_++;
  return true;
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

SymbolTypeInfo::SymbolTypeInfo(const std::string &name,
                               std::shared_ptr<TypeDef> type)
    : SymbolInfo(), name_(name), type_(std::move(type)) {}
SymbolTypeInfo::~SymbolTypeInfo() = default;
auto SymbolTypeInfo::getName() const -> const std::string & { return name_; }
auto SymbolTypeInfo::getType() const -> std::shared_ptr<TypeDef> {
  return type_;
}