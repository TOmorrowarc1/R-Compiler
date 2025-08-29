#include "ASTAnnotator.hpp"
#include "ASTRootNode.hpp"
#include "BuiltInInit.hpp"
#include "ConstEvaluator.hpp"
#include "ConstTypeCollector.hpp"
#include "ConstTypeEvaluator.hpp"
#include "FunctionCollector.hpp"
#include "Scope.hpp"
#include "SemanticChecker.hpp"

void ASTAnnotate(ASTRootNode *root, Scope *initial_scope) {
  ConstEvaluator const_evaluator;
  builtInInit(initial_scope, &const_evaluator);
  ConstTypeCollector collector(initial_scope, &const_evaluator);
  root->accept(collector);
  initial_scope->resetIndex();
  ConstTypeEvaluator evaluator(initial_scope, &const_evaluator);
  root->accept(evaluator);
  initial_scope->resetIndex();
  FunctionCollector function_collector(initial_scope, &const_evaluator);
  root->accept(function_collector);
  initial_scope->resetIndex();
  SemanticChecker checker(initial_scope, &const_evaluator);
  root->accept(checker);
  initial_scope->resetIndex();
}