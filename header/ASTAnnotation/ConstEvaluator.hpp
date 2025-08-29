#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>

/*
The ConstEvaluator controls the evaluation of constant expressions and type
definitions. Evaluate() functions perform evaluations and adjust info of symbols
in scopes, after which use the getSymbol() of scope can retrieve the updated
symbol information.
*/

class ASTNode;
class ItemStructNode;
class ItemEnumNode;
class TypeNode;
class ExprNode;
class Scope;
class TypeKind;
class StructDef;
class EnumDef;
class ConstInfo;

class StatusRecorder {
private:
  enum Status { UNTOUCH, CALC, VAILD };
  Status status_;

public:
  StatusRecorder();
  StatusRecorder(const StatusRecorder &other) = default;
  ~StatusRecorder();
  auto touch() -> bool;
  auto isValid() const -> bool;
  void setVaild();
};

struct SymbolStatus {
  ASTNode *node;
  StatusRecorder status;
  SymbolStatus();
  SymbolStatus(ASTNode *target);
  SymbolStatus(const SymbolStatus &other) = default;
};

class ConstEvaluator {
private:
  Scope **current_scope_;
  std::unordered_map<std::string, SymbolStatus> type_def_symbols;
  std::unordered_map<std::string, SymbolStatus> const_symbols;
  std::unordered_map<std::string, std::unordered_map<std::string, SymbolStatus>>
      struct_const_symbols;

public:
  ConstEvaluator();
  ~ConstEvaluator();
  void bindScopePointer(Scope **current_scope);
  void addBuiltInSymbol(const std::string &symbol);
  void attachNodeToTypeDef(ASTNode *node, const std::string &symbol);
  void attachNodeToConst(ASTNode *node, const std::string &symbol);
  void attachNodeToStructConst(ASTNode *node, const std::string &struct_name,
                               const std::string &symbol);
  void evaluateTypeSymbol(const std::string &symbol);
  void evaluateConstSymbol(const std::string &symbol);
  void evaluateStructConst(const std::string &struct_name,
                           const std::string &symbol);
  auto evaluateType(TypeNode *node) -> std::shared_ptr<TypeKind>;
  auto evaluateExprValue(ExprNode *node) -> std::shared_ptr<ConstInfo>;
};