#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

class TypeKind;
class SymbolFunctionInfo;

class TypeDef {
private:
  std::string name_;
  std::unordered_map<std::string, std::shared_ptr<TypeKind>> members_;
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>> methods_;
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>
      associated_functions_;

public:
  TypeDef(const std::string &name);
  TypeDef(const std::string &name, const std::vector<std::string> &member_names,
          std::vector<std::shared_ptr<TypeKind>> &&member_types);
  ~TypeDef();
  auto getName() const -> const std::string &;
  auto getMember(const std::string &name) const -> std::shared_ptr<TypeKind>;
  auto addMethod(const std::string &name,
                 std::shared_ptr<SymbolFunctionInfo> &&method) -> bool;
  auto addAssociatedFunction(const std::string &name,
                             std::shared_ptr<SymbolFunctionInfo> &&function)
      -> bool;
  auto getMethod(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
  auto getAssociatedFunction(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
};