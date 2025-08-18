#include "Scope.hpp"
#include "Symbol.hpp"

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
  return nullptr;
}

auto Scope::addImplSymbol(const std::string &impl_name,
                          const std::string &func_name,
                          std::shared_ptr<SymbolFunctionInfo> func) -> bool {
  auto &func_map = impl_symbols_map_[impl_name];
  if (func_map.find(func_name) != func_map.end()) {
    return false;
  }
  func_map[func_name] = std::move(func);
  return true;
}

auto Scope::getImplSymbol(const std::string &impl_name,
                          const std::string &func_name)
    -> std::shared_ptr<SymbolFunctionInfo> {
  auto it = impl_symbols_map_.find(impl_name);
  if (it != impl_symbols_map_.end()) {
    auto func_it = it->second.find(func_name);
    if (func_it != it->second.end()) {
      return func_it->second;
    }
  }
  if (parent_) {
    return parent_->getImplSymbol(impl_name, func_name);
  }
  return nullptr;
}