#include "ASTAnnotator.hpp"
#include "ASTRootNode.hpp"
#include "Scope.hpp"
#include "ScopeBuiltInInit.hpp"
#include "SemanticChecker.hpp"
#include "SymbolAnnotator.hpp"
#include "SymbolCollector.hpp"
#include "TypeKind.hpp"
#include "ValueInfo.hpp"

void ASTAnnotate(ASTRootNode *root, Scope *initial_scope) {
  scopeBuildInInit(initial_scope);
  SymbolCollector collector(initial_scope);
  root->accept(collector);
  initial_scope->resetIndex();
  SymbolAnnotator annotator(initial_scope);
  root->accept(annotator);
  initial_scope->resetIndex();
  SemanticChecker checker(initial_scope);
  root->accept(checker);
  initial_scope->resetIndex();
}