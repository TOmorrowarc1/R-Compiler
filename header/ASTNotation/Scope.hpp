#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>

class SymbolInfo;
class TypeDef;

class Scope {
private:
  std::shared_ptr<Scope> parent_;
  std::unordered_map<std::string, std::shared_ptr<SymbolInfo>> symbols_;
  std::unordered_map<std::string, std::shared_ptr<TypeDef>> types_;

public:
  Scope(std::shared_ptr<Scope> parent);
  ~Scope();
  auto getParent() const -> std::shared_ptr<Scope>;
  auto addSymbol(const std::string &name, std::shared_ptr<SymbolInfo>) -> bool;
  auto getSymbol(const std::string &name) const -> std::shared_ptr<SymbolInfo>;
  auto addType(const std::string &name, std::shared_ptr<TypeDef>) -> bool;
  auto getType(const std::string &name) const -> std::shared_ptr<TypeDef>;
};