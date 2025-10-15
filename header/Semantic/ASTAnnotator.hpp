#pragma once

class Scope;
class ASTRootNode;

void ASTAnnotate(ASTRootNode *root, Scope *initial_scope);