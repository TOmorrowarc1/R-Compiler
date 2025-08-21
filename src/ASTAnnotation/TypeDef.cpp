#include "TypeDef.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include <stdexcept>

TypeDef::TypeDef(const std::string &name) : name_(name) {}

TypeDef::~TypeDef() = default;

auto TypeDef::getName() const -> const std::string & { return name_; }

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

StructDef::StructDef(const std::string &name) : TypeDef(name) {}

StructDef::StructDef(const std::string &name,
                     const std::vector<std::string> &member_names,
                     std::vector<std::shared_ptr<TypeKind>> &&member_types)
    : TypeDef(name) {
  if (member_names.size() != member_types.size()) {
    throw std::runtime_error("Member names and types size mismatch");
  }
  for (size_t i = 0; i < member_names.size(); ++i) {
    if (!addMember(member_names[i], std::move(member_types[i]))) {
      throw std::runtime_error("Failed to add member: " + member_names[i]);
    }
  }
}

StructDef::~StructDef() = default;

auto StructDef::addMember(const std::string &name,
                          std::shared_ptr<TypeKind> &&type) -> bool {
  if (members_.find(name) != members_.end()) {
    return false;
  }
  members_.emplace(name, std::move(type));
  return true;
}

auto StructDef::getMember(const std::string &name) const
    -> std::shared_ptr<TypeKind> {
  auto iter = members_.find(name);
  if (iter != members_.end()) {
    return iter->second;
  }
  return nullptr;
}

EnumDef::EnumDef(const std::string &name) : TypeDef(name) {}

EnumDef::EnumDef(const std::string &name,
                 const std::vector<std::string> &variants)
    : TypeDef(name) {
  for (const auto &variant : variants) {
    if (!addVariant(variant)) {
      throw std::runtime_error("Failed to add variant: " + variant);
    }
  }
}

EnumDef::~EnumDef() = default;

auto EnumDef::addVariant(const std::string &variant) -> bool {
  if (variants_.count(variant) != 0) {
    return false;
  }
  variants_.insert(variant);
  return true;
}

auto EnumDef::getVariant(const std::string &variant) const -> bool {
  return variants_.count(variant) != 0;
}

auto EnumDef::getVariants() const -> const std::vector<std::string> {
  return std::vector<std::string>(variants_.begin(), variants_.end());
}