#include "Scope.hpp"
#include "Symbol.hpp"
#include "TypeDef.hpp"

Scope::Scope(std::shared_ptr<Scope> parent) : parent_(std::move(parent)) {}

Scope::~Scope() = default;

auto Scope::getParent() const -> std::shared_ptr<Scope> { return parent_; }

auto Scope::addSymbol(const std::string &name,
                      std::shared_ptr<SymbolInfo> symbol) -> bool {
  if (symbols_.find(name) != symbols_.end()) {
    return false;
  }
  symbols_[name] = std::move(symbol);
  return true;
}

auto Scope::getSymbol(const std::string &name) const
    -> std::shared_ptr<SymbolInfo> {
  auto it = symbols_.find(name);
  if (it != symbols_.end()) {
    return it->second;
  }
  if (parent_) {
    return parent_->getSymbol(name);
  }
  return nullptr;
}

auto Scope::addType(const std::string &name, std::shared_ptr<TypeDef> type)
    -> bool {
  if (types_.find(name) != types_.end()) {
    return false;
  }
  types_[name] = std::move(type);
  return true;
}

auto Scope::getType(const std::string &name) const -> std::shared_ptr<TypeDef> {
  auto it = types_.find(name);
  if (it != types_.end()) {
    return it->second;
  }
  if (parent_) {
    return parent_->getType(name);
  }
  return nullptr;
}