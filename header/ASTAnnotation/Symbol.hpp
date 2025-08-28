#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

class TypeDef;
class TypeKind;
class ConstInfo;
class ConstValue;

class SymbolInfo {
public:
  virtual ~SymbolInfo() = default;
};

class SymbolVariableInfo : public SymbolInfo {
private:
  std::string name_;
  std::shared_ptr<TypeKind> type_;
  bool is_mutable_;

public:
  SymbolVariableInfo(const std::string &name, std::shared_ptr<TypeKind> type,
                     bool is_mutable);
  ~SymbolVariableInfo();
  auto getName() const -> const std::string &;
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto isMutable() const -> bool;
};

class SymbolFunctionInfo : public SymbolInfo {
public:
  enum class FnType {
    Normal,
    Method,
    MutMethod,
  };

private:
  std::string name_;
  std::shared_ptr<TypeKind> return_type_;
  std::vector<std::shared_ptr<TypeKind>> parameters_;
  FnType fn_type_;

public:
  SymbolFunctionInfo(const std::string &name,
                     std::shared_ptr<TypeKind> returnType,
                     std::vector<std::shared_ptr<TypeKind>> &&parameters);
  ~SymbolFunctionInfo();
  auto getName() const -> const std::string &;
  auto getReturnType() const -> std::shared_ptr<TypeKind>;
  auto getParametersType() const
      -> const std::vector<std::shared_ptr<TypeKind>> &;
  void setFnType(FnType type);
};

class SymbolTypeInfo : public SymbolInfo {
private:
  std::string name_;
  std::shared_ptr<TypeDef> type_;

public:
  SymbolTypeInfo(const std::string &name, std::shared_ptr<TypeDef> type);
  ~SymbolTypeInfo();
  auto getName() const -> const std::string &;
  auto getType() const -> std::shared_ptr<TypeDef>;
};

class SymbolConstInfo : public SymbolInfo {
private:
  std::string name_;
  std::unique_ptr<ConstInfo> const_info_;

public:
  SymbolConstInfo(const std::string &name,
                  std::unique_ptr<ConstInfo> &&const_info);
  ~SymbolConstInfo();
  auto getName() const -> const std::string &;
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto getValue() -> std::shared_ptr<ConstValue>;
  void setValue(int32_t value);
};