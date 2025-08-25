#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

class SymbolInfo;
class SymbolTypeInfo;
class SymbolFunctionInfo;
class SymbolVariableInfo;

class Scope {
private:
  Scope *parent_;
  uint32_t index_now;
  std::vector<std::unique_ptr<Scope>> children_;
  std::unordered_map<std::string, std::shared_ptr<SymbolInfo>> symbols_;
  std::unordered_map<std::string, std::shared_ptr<SymbolTypeInfo>> types_;

public:
  Scope();
  Scope(Scope *parent);
  ~Scope();
  auto getParent() const -> Scope *;
  auto addFunction(const std::string &name,
                   std::shared_ptr<SymbolFunctionInfo> function) -> bool;
  auto addVarible(const std::string &name,
                  std::shared_ptr<SymbolVariableInfo> symbol) -> bool;
  auto getSymbol(const std::string &name) const -> std::shared_ptr<SymbolInfo>;
  auto addType(const std::string &name, std::shared_ptr<SymbolTypeInfo> symbol)
      -> bool;
  auto getType(const std::string &name) const
      -> std::shared_ptr<SymbolTypeInfo>;
  auto addNextChildScope() -> Scope *;
  auto getNextChildScope() -> Scope *;
  void resetIndex();
};