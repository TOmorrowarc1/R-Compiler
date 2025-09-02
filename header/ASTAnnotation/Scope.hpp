#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

class SymbolInfo;
class SymbolTypeInfo;
class SymbolTraitInfo;
class SymbolFunctionInfo;
class SymbolVariableInfo;
class SymbolConstInfo;

class Scope {
private:
  Scope *parent_;
  uint32_t index_now;
  std::vector<std::unique_ptr<Scope>> children_;
  std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>> consts_;
  std::unordered_map<std::string, std::shared_ptr<SymbolInfo>> symbols_;
  std::unordered_map<std::string, std::shared_ptr<SymbolTypeInfo>> types_;
  std::unordered_map<std::string, std::shared_ptr<SymbolTraitInfo>> traits_;
  auto getConstSymbol(const std::string &name) const
      -> std::shared_ptr<SymbolConstInfo>;

public:
  Scope();
  Scope(Scope *parent);
  ~Scope();
  auto getParent() const -> Scope *;
  auto addConst(const std::string &name,
                std::shared_ptr<SymbolConstInfo> const_var) -> bool;
  auto addFunction(const std::string &name,
                   std::shared_ptr<SymbolFunctionInfo> function) -> bool;
  auto addVarible(const std::string &name,
                  std::shared_ptr<SymbolVariableInfo> symbol) -> bool;
  auto getSymbol(const std::string &name) const -> std::shared_ptr<SymbolInfo>;
  auto addType(const std::string &name, std::shared_ptr<SymbolTypeInfo> symbol)
      -> bool;
  auto getType(const std::string &name) const
      -> std::shared_ptr<SymbolTypeInfo>;
  auto addTrait(const std::string &name,
                std::shared_ptr<SymbolTraitInfo> symbol) -> bool;
  auto getTrait(const std::string &name) const
      -> std::shared_ptr<SymbolTraitInfo>;
  auto addNextChildScope() -> Scope *;
  auto getNextChildScope() -> Scope *;
  void resetIndex();
};