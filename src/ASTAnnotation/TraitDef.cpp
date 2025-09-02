#include "TraitDef.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"
#include <stdexcept>

TraitDef::TraitDef() = default;

TraitDef::~TraitDef() = default;

auto TraitDef::addConst(const std::string &name,
                        std::shared_ptr<SymbolConstInfo> type_const) -> bool {
  if (trait_consts_.find(name) != trait_consts_.end()) {
    return false;
  }
  trait_consts_[name] = type_const;
  return true;
}

auto TraitDef::getConst(const std::string &name) const
    -> std::shared_ptr<SymbolConstInfo> {
  auto iter = trait_consts_.find(name);
  if (iter != trait_consts_.end()) {
    return iter->second;
  }
  return nullptr;
}

auto TraitDef::getAllConst() const
    -> std::vector<std::shared_ptr<SymbolConstInfo>> {
  std::vector<std::shared_ptr<SymbolConstInfo>> consts;
  for (auto &pair : trait_consts_) {
    consts.push_back(pair.second);
  }
  return consts;
}

auto TraitDef::addMethod(const std::string &name,
                         std::shared_ptr<SymbolFunctionInfo> method) -> bool {
  if (trait_methods_.find(name) != trait_methods_.end()) {
    return false;
  }
  trait_methods_.emplace(name, std::move(method));
  return true;
}

auto TraitDef::getMethod(const std::string &name) const
    -> std::shared_ptr<SymbolFunctionInfo> {
  auto iter = trait_methods_.find(name);
  if (iter != trait_methods_.end()) {
    return iter->second;
  }
  return nullptr;
}

auto TraitDef::getAllMethod() const
    -> std::vector<std::shared_ptr<SymbolFunctionInfo>> {
  std::vector<std::shared_ptr<SymbolFunctionInfo>> methods;
  for (auto &pair : trait_methods_) {
    methods.push_back(pair.second);
  }
  return methods;
}