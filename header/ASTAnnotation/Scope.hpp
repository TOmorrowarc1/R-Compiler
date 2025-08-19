#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

class SymbolInfo;
class SymbolTypeInfo;
class SymbolFunctionInfo;

class Scope {
private:
  Scope *parent_;
  uint32_t index_now;
  std::vector<std::unique_ptr<Scope>> children_;
  std::unordered_map<std::string, std::shared_ptr<SymbolInfo>> symbols_;
  std::unordered_map<std::string, std::shared_ptr<SymbolTypeInfo>> types_;

public:
  Scope() = delete;
  explicit Scope(Scope *parent);
  ~Scope();
  auto getParent() const -> Scope *;
  auto addSymbol(const std::string &name, std::shared_ptr<SymbolInfo> &&symbol)
      -> bool;
  auto getSymbol(const std::string &name) const -> std::shared_ptr<SymbolInfo>;
  auto addType(const std::string &name,
               std::shared_ptr<SymbolTypeInfo> &&symbol) -> bool;
  auto getType(const std::string &name) const
      -> std::shared_ptr<SymbolTypeInfo>;
  auto addChildScope(std::unique_ptr<Scope> &&child) -> bool;
  auto getNextChildScope() -> Scope *;
};