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
class TypeNode;
class ExprNode;
class Scope;
class TypeKind;
class ConstInfo;

class StatusRecorder {
private:
  enum Status { UNTOUCH, CALC, VAILD };
  Status status_;

public:
  StatusRecorder();
  ~StatusRecorder();
  auto touch() -> bool;
  auto isValid() const -> bool;
  void setVaild();
};

class ConstEvaluator {
private:
  Scope **current_scope_;
  std::unordered_map<std::string, ASTNode *> type_def_symbols;
  std::unordered_map<std::string, ASTNode *> const_symbols;
  std::unordered_map<std::string, std::unordered_map<std::string, ASTNode *>>
      struct_const_symbols;

public:
  ConstEvaluator();
  ~ConstEvaluator();
  void bindScopePointer(Scope **current_scope);
  void attachNodeToTypeDef(ASTNode *node, const std::string &symbol);
  void attachNodeToConst(ASTNode *node, const std::string &symbol);
  void attachNodeToStructConst(ASTNode *node, const std::string &struct_name,
                               const std::string &symbol);
  void evaluateTypeSymbol(const std::string &symbol);
  void evaluateConstSymbol(const std::string &symbol);
  void evaluateStructConst(const std::string &struct_name,
                           const std::string &symbol);
  auto evaluateTypeKind(TypeNode *node) -> std::shared_ptr<TypeKind>;
  auto evaluateExprValue(ExprNode *node) -> std::shared_ptr<ConstInfo>;
};