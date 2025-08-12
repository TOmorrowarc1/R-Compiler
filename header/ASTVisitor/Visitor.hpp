#include <stdint.h>

class ASTRootNode;

class ItemConstantNode;
class ItemFnNode;

class ExprBlockNode;
class ExprBlockConstNode;
class ExprCallNode;
class ExprBreakNode;
class ExprReturnNode;
class ExprContinueNode;
class ExprGroupNode;
class ExprIfNode;
class ExprLiteralIntNode;
class ExprLiteralBoolNode;
class ExprLiteralCharNode;
class ExprLiteralStringNode;
class ExprLiteralByteNode;
class ExprLoopNode;
class ExprWhileNode;
class ExprOperBinaryNode;
class ExprOperUnaryNode;
class ExprPathNode;

class StmtExprNode;
class StmtEmptyNode;
class StmtItemNode;
class StmtLetNode;

class PatternMultNode;
class PatternLiteralNode;
class PatternStructNode;
class PatternSliceNode;
class PatternTupleNode;
class PatternWildNode;
class PatternPathNode;
class PatternGroupNode;
class PatternIDNode;

class TypeTupleNode;
class TypeArrayNode;
class TypeSliceNode;
class TypeInferNode;
class TypeNeverNode;
class TypePathNode;
class TypeParentNode;

class PathSimpleNode;
class PathQualifiedNode;

class Visitor {
public:
  Visitor() = default;
  virtual ~Visitor() = default;

  virtual void visit(ASTRootNode &node) = 0;

  virtual void visit(ItemConstantNode &node) = 0;
  virtual void visit(ItemFnNode &node) = 0;

  virtual void visit(ExprBlockNode &node) = 0;
  virtual void visit(ExprBlockConstNode &node) = 0;
  virtual void visit(ExprCallNode &node) = 0;
  virtual void visit(ExprBreakNode &node) = 0;
  virtual void visit(ExprReturnNode &node) = 0;
  virtual void visit(ExprContinueNode &node) = 0;
  virtual void visit(ExprGroupNode &node) = 0;
  virtual void visit(ExprIfNode &node) = 0;
  virtual void visit(ExprLiteralIntNode &node) = 0;
  virtual void visit(ExprLiteralBoolNode &node) = 0;
  virtual void visit(ExprLiteralCharNode &node) = 0;
  virtual void visit(ExprLiteralStringNode &node) = 0;
  virtual void visit(ExprLiteralByteNode &node) = 0;
  virtual void visit(ExprLoopNode &node) = 0;
  virtual void visit(ExprWhileNode &node) = 0;
  virtual void visit(ExprOperBinaryNode &node) = 0;
  virtual void visit(ExprOperUnaryNode &node) = 0;
  virtual void visit(ExprPathNode &node) = 0;

  virtual void visit(StmtExprNode &node) = 0;
  virtual void visit(StmtEmptyNode &node) = 0;
  virtual void visit(StmtItemNode &node) = 0;
  virtual void visit(StmtLetNode &node) = 0;

  virtual void visit(PatternMultNode &node) = 0;
  virtual void visit(PatternLiteralNode &node) = 0;
  virtual void visit(PatternStructNode &node) = 0;
  virtual void visit(PatternSliceNode &node) = 0;
  virtual void visit(PatternTupleNode &node) = 0;
  virtual void visit(PatternWildNode &node) = 0;
  virtual void visit(PatternPathNode &node) = 0;
  virtual void visit(PatternGroupNode &node) = 0;
  virtual void visit(PatternIDNode &node) = 0;

  virtual void visit(TypeTupleNode &node) = 0;
  virtual void visit(TypeArrayNode &node) = 0;
  virtual void visit(TypeSliceNode &node) = 0;
  virtual void visit(TypeInferNode &node) = 0;
  virtual void visit(TypeNeverNode &node) = 0;
  virtual void visit(TypePathNode &node) = 0;
  virtual void visit(TypeParentNode &node) = 0;

  virtual void visit(PathSimpleNode &node) = 0;
  virtual void visit(PathQualifiedNode &node) = 0;
};