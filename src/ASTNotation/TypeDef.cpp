#include "TypeDef.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"

TypeDef::TypeDef(const std::string &name,
                 std::vector<std::string> &&member_names,
                 std::vector<std::shared_ptr<TypeKind>> &&member_types)
    : name_(name) {
  for (size_t i = 0; i < member_names.size(); ++i) {
    members_.emplace(std::move(member_names[i]), std::move(member_types[i]));
  }
}
TypeDef::~TypeDef() = default;
auto TypeDef::getName() const -> const std::string & { return name_; }
auto TypeDef::getMember(const std::string &name) const
    -> std::shared_ptr<TypeKind> {
  auto it = members_.find(name);
  if (it != members_.end()) {
    return it->second;
  }
  return nullptr;
}
auto TypeDef::getMethod(const std::string &name) const
    -> std::shared_ptr<SymbolFunctionInfo> {
  auto it = methods_.find(name);
  if (it != methods_.end()) {
    return it->second;
  }
  return nullptr;
}
auto TypeDef::addMethod(std::shared_ptr<SymbolFunctionInfo> method) -> bool {
  if (method && !method->getName().empty()) {
    methods_[method->getName()] = std::move(method);
    return true;
  }
  return false;
}
auto TypeDef::getAssociatedFunction(const std::string &name) const
    -> std::shared_ptr<SymbolFunctionInfo> {
  auto it = associated_functions_.find(name);
  if (it != associated_functions_.end()) {
    return it->second;
  }
  return nullptr;
}
auto TypeDef::addAssociatedFunction(
    std::shared_ptr<SymbolFunctionInfo> function) -> bool {
  if (function && !function->getName().empty()) {
    associated_functions_[function->getName()] = std::move(function);
    return true;
  }
  return false;
}