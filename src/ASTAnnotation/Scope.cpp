#include "Scope.hpp"
#include "Symbol.hpp"
#include "cast.hpp"
#include <stdexcept>

Scope::Scope() : parent_(nullptr), index_now(0) {}

Scope::Scope(Scope *parent) : parent_(parent), index_now(0) {}

Scope::~Scope() = default;

auto Scope::getParent() const -> Scope * { return parent_; }

auto Scope::getConstSymbol(const std::string &name, bool is_recursive) const
    -> std::shared_ptr<SymbolConstInfo> {
  auto it = consts_.find(name);
  if (it != consts_.end()) {
    return it->second;
  }
  if (is_recursive && parent_) {
    return parent_->getConstSymbol(name, is_recursive);
  }
  return nullptr;
}

auto Scope::addConst(const std::string &name,
                     std::shared_ptr<SymbolConstInfo> const_var) -> bool {
  if (getConstSymbol(name, false) != nullptr) {
    return false;
  }
  consts_[name] = std::move(const_var);
  return true;
}

auto Scope::addFunction(const std::string &name,
                        std::shared_ptr<SymbolFunctionInfo> function) -> bool {
  if (getConstSymbol(name, false) != nullptr) {
    return false;
  }
  if (symbols_.find(name) != symbols_.end()) {
    return false;
  }
  symbols_[name] = std::move(function);
  return true;
}

auto Scope::addVarible(const std::string &name,
                       std::shared_ptr<SymbolVariableInfo> symbol) -> bool {
  if (getConstSymbol(name, true) != nullptr) {
    return false;
  }
  symbols_[name] = std::move(symbol);
  return true;
}

auto Scope::getSymbol(const std::string &name) const
    -> std::shared_ptr<SymbolInfo> {
  auto const_symbol = getConstSymbol(name, false);
  if (const_symbol != nullptr) {
    return const_symbol;
  }
  if (symbols_.find(name) != symbols_.end()) {
    return symbols_.at(name);
  }
  return parent_ == nullptr ? nullptr : parent_->getSymbol(name);
}

auto Scope::addType(const std::string &name,
                    std::shared_ptr<SymbolTypeInfo> type) -> bool {
  if (types_.find(name) != types_.end() || getTrait(name) != nullptr) {
    return false;
  }
  types_[name] = std::move(type);
  return true;
}

auto Scope::getType(const std::string &name) const
    -> std::shared_ptr<SymbolTypeInfo> {
  auto it = types_.find(name);
  if (it != types_.end()) {
    return it->second;
  }
  if (parent_) {
    return parent_->getType(name);
  }
  return nullptr;
}

auto Scope::addTrait(const std::string &name,
                     std::shared_ptr<SymbolTraitInfo> symbol) -> bool {
  if (traits_.find(name) != traits_.end() || getType(name) != nullptr) {
    return false;
  }
  traits_[name] = std::move(symbol);
  return true;
}

auto Scope::getTrait(const std::string &name) const
    -> std::shared_ptr<SymbolTraitInfo> {
  auto it = traits_.find(name);
  if (it != traits_.end()) {
    return it->second;
  }
  if (parent_) {
    return parent_->getTrait(name);
  }
  return nullptr;
}

auto Scope::addNextChildScope() -> Scope * {
  auto child = std::make_unique<Scope>(this);
  children_.push_back(std::move(child));
  return children_.back().get();
}

auto Scope::getNextChildScope() -> Scope * {
  if (index_now >= children_.size()) {
    throw std::runtime_error("No more child scopes available");
    return nullptr;
  }
  return children_[index_now++].get();
}

void Scope::resetIndex() {
  index_now = 0;
  for (const auto &child_scope : children_) {
    child_scope->resetIndex();
  }
}