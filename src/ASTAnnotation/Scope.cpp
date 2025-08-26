#include "Scope.hpp"
#include "Symbol.hpp"
#include "cast.hpp"
#include <stdexcept>

Scope::Scope() : parent_(nullptr), index_now(0) {}

Scope::Scope(Scope *parent) : parent_(parent), index_now(0) {}

Scope::~Scope() = default;

auto Scope::getParent() const -> Scope * { return parent_; }

auto Scope::getConstSymbol(const std::string &name) const
    -> std::shared_ptr<SymbolConstInfo> {
  auto it = consts_.find(name);
  if (it != consts_.end()) {
    return it->second;
  }
  if (parent_) {
    return parent_->getConstSymbol(name);
  }
}

auto Scope::addConst(const std::string &name,
                     std::shared_ptr<SymbolConstInfo> const_var) -> bool {
  if (getConstSymbol(name) != nullptr) {
    return false;
  }
  consts_[name] = std::move(const_var);
  return true;
}

auto Scope::addFunction(const std::string &name,
                        std::shared_ptr<SymbolFunctionInfo> function) -> bool {
  if (getConstSymbol(name) != nullptr) {
    return false;
  }
  symbols_[name] = std::move(function);
  return true;
}

auto Scope::addVarible(const std::string &name,
                       std::shared_ptr<SymbolVariableInfo> symbol) -> bool {
  if (getConstSymbol(name) != nullptr) {
    return false;
  }
  symbols_[name] = std::move(symbol);
  return true;
}

auto Scope::getSymbol(const std::string &name) const
    -> std::shared_ptr<SymbolInfo> {
  auto const_symbol = getConstSymbol(name);
  if (const_symbol != nullptr) {
    return const_symbol;
  }
  const Scope *cursor = this;
  while (cursor) {
    auto it = cursor->symbols_.find(name);
    if (it != cursor->symbols_.end()) {
      return it->second;
    }
    cursor = cursor->parent_;
  }
  throw std::runtime_error("Symbol not found: " + name);
  return nullptr;
}

auto Scope::addType(const std::string &name,
                    std::shared_ptr<SymbolTypeInfo> type) -> bool {
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