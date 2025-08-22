#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

class TypeDef;
class TypeKind;

class SymbolInfo {
public:
  virtual ~SymbolInfo() = default;
};

class SymbolVariableInfo : public SymbolInfo {
private:
  std::string name_;
  std::shared_ptr<TypeKind> type_;

public:
  SymbolVariableInfo(const std::string &name, std::shared_ptr<TypeKind> type);
  ~SymbolVariableInfo();
  auto getName() const -> const std::string &;
  auto getType() const -> std::shared_ptr<TypeKind>;
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