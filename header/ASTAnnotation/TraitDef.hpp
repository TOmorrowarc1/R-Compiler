#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class SymbolFunctionInfo;
class SymbolConstInfo;

class TraitDef {
private:
  std::string name_;
  std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>
      trait_consts_;
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>
      trait_methods_;
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>
      trait_functions_;

public:
  TraitDef();
  ~TraitDef();
  auto addConst(const std::string &name,
                std::shared_ptr<SymbolConstInfo> trait_const) -> bool;
  auto getConst(const std::string &name) const
      -> std::shared_ptr<SymbolConstInfo>;
  auto getAllConst() const
      -> std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>;
  auto addFunction(const std::string &name,
                   std::shared_ptr<SymbolFunctionInfo> method) -> bool;
  auto getFunction(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
  auto getAllFunction() const
      -> std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>;
  auto addMethod(const std::string &name,
                 std::shared_ptr<SymbolFunctionInfo> method) -> bool;
  auto getMethod(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
  auto getAllMethod() const
      -> std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>;
};