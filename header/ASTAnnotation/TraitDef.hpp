#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class SymbolFunctionInfo;
class SymbolConstInfo;

class TraitDef {
private:
  std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>
      trait_consts_;
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>
      trait_methods_;

public:
  TraitDef();
  ~TraitDef();
  auto addConst(const std::string &name,
                std::shared_ptr<SymbolConstInfo> type_const) -> bool;
  auto getConst(const std::string &name) const
      -> std::shared_ptr<SymbolConstInfo>;
  auto getAllConst() const -> std::vector<std::shared_ptr<SymbolConstInfo>>;
  auto addMethod(const std::string &name,
                 std::shared_ptr<SymbolFunctionInfo> method) -> bool;
  auto getMethod(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
  auto getAllMethod() const -> std::vector<std::shared_ptr<SymbolFunctionInfo>>;
};