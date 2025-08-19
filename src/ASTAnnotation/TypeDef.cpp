#include "TypeDef.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"

TypeDef::TypeDef(const std::string &name) : name_(name) {}

TypeDef::TypeDef(const std::string &name,
                 const std::vector<std::string> &member_names,
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
  auto iter = members_.find(name);
  if (iter != members_.end()) {
    return iter->second;
  }
  return nullptr;
}

auto TypeDef::addMethod(const std::string &name,
                        std::shared_ptr<SymbolFunctionInfo> &&method) -> bool {
  if (methods_.find(name) != methods_.end()) {
    return false;
  }
  methods_.emplace(name, std::move(method));
  return true;
}

auto TypeDef::addAssociatedFunction(
    const std::string &name, std::shared_ptr<SymbolFunctionInfo> &&function)
    -> bool {
  if (associated_functions_.find(name) != associated_functions_.end()) {
    return false;
  }
  associated_functions_.emplace(name, std::move(function));
  return true;
}

auto TypeDef::getMethod(const std::string &name) const
    -> std::shared_ptr<SymbolFunctionInfo> {
  auto iter = methods_.find(name);
  if (iter != methods_.end()) {
    return iter->second;
  }
  return nullptr;
}

auto TypeDef::getAssociatedFunction(const std::string &name) const
    -> std::shared_ptr<SymbolFunctionInfo> {
  auto iter = associated_functions_.find(name);
  if (iter != associated_functions_.end()) {
    return iter->second;
  }
  return nullptr;
}