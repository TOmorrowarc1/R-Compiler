#pragma once
#include "Scope.hpp"

class ASTRootNode;

void ASTAnnotate(ASTRootNode *root, Scope *initial_scope);