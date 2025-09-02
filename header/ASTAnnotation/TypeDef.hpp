#pragma once
#include <memory>
#include <set>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

/*
The TypeDef class represents a PathType, which is fixed by a unique symbol.
It can be either a StructDef(contains inline types) or an EnumDef, and implied
by all impl blocks in the subtree of the scope it is in.
*/

class TypeKind;
class SymbolFunctionInfo;
class SymbolConstInfo;

class TypeDef {
private:
  std::string name_;
  std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>
      type_consts_;
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>> methods_;
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>
      associated_functions_;

public:
  TypeDef(const std::string &name);
  virtual ~TypeDef();
  auto getName() const -> const std::string &;
  auto addConst(const std::string &name,
                std::shared_ptr<SymbolConstInfo> type_const) -> bool;
  auto getConst(const std::string &name) const
      -> std::shared_ptr<SymbolConstInfo>;
  auto addMethod(const std::string &name,
                 std::shared_ptr<SymbolFunctionInfo> method) -> bool;
  auto getMethod(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
  auto addAssociatedFunction(const std::string &name,
                             std::shared_ptr<SymbolFunctionInfo> function)
      -> bool;
  auto getAssociatedFunction(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
};

class StructDef : public TypeDef {
private:
  std::unordered_map<std::string, std::shared_ptr<TypeKind>> members_;

public:
  StructDef(const std::string &name);
  StructDef(const std::string &name,
            const std::vector<std::string> &member_names,
            std::vector<std::shared_ptr<TypeKind>> &&member_types);
  ~StructDef() override;
  auto addMember(const std::string &name, std::shared_ptr<TypeKind> type)
      -> bool;
  auto getMember(const std::string &name) const -> std::shared_ptr<TypeKind>;
  auto getMemNum() const -> size_t;
};

class EnumDef : public TypeDef {
private:
  std::set<std::string> variants_;

public:
  EnumDef(const std::string &name);
  EnumDef(const std::string &name, const std::vector<std::string> &variants);
  ~EnumDef() override;
  auto addVariant(const std::string &variant) -> bool;
  auto getVariant(const std::string &variant) const -> bool;
  auto getVariants() const -> const std::vector<std::string>;
};