#include "Scope.hpp"
#include "Symbol.hpp"
#include <stdexcept>

Scope::Scope() : parent_(nullptr), index_now(0) {}

Scope::Scope(Scope *parent) : parent_(parent), index_now(0) {}

Scope::~Scope() = default;

auto Scope::getParent() const -> Scope * { return parent_; }

auto Scope::addSymbol(const std::string &name,
                      std::shared_ptr<SymbolInfo> &&symbol) -> bool {
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
  throw std::runtime_error("Symbol not found: " + name);
  return nullptr;
}

auto Scope::addType(const std::string &name,
                    std::shared_ptr<SymbolTypeInfo> &&type) -> bool {
  if (types_.find(name) != types_.end()) {
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
  throw std::runtime_error("Type not found: " + name);
  return nullptr;
}

auto Scope::addNextChildScope() -> Scope * {
  auto child = std::make_unique<Scope>(this);
  children_.push_back(std::move(child));
  return child.get();
}

auto Scope::getNextChildScope() -> Scope * {
  if (index_now >= children_.size()) {
    throw std::runtime_error("No more child scopes available");
    return nullptr;
  }
  return children_[index_now].get();
}