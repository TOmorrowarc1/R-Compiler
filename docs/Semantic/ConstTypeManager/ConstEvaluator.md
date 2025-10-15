# ConstEvaluator - 常量求值器

## 简介

`ConstEvaluator` 是 AST 标注系统中的核心组件，负责控制常量表达式的求值和类型定义的处理。它通过 `Evaluate()` 函数执行求值操作，并调整作用域中的符号信息，之后可以通过作用域的 `getSymbol()` 方法获取更新后的符号信息。

## 功能概述

`ConstEvaluator` 主要跟踪三种类型的符号：
1. **类型定义**（Type definitions）
2. **类型外的常量项**（Constant items out of type）
3. **impl 和 trait 中的常量**（Constants in impls and traits）

## 类结构

### StatusRecorder

`StatusRecorder` 类用于跟踪符号的求值状态，避免循环依赖和重复计算。

```cpp
class StatusRecorder {
private:
  enum Status { UNTOUCH, CALC, VALID };
  Status status_;

public:
  StatusRecorder();
  StatusRecorder(const StatusRecorder &other) = default;
  ~StatusRecorder();
  auto touch() -> bool;          // 标记为正在计算，返回是否可以开始计算
  auto isValid() const -> bool;  // 检查是否已有效计算
  void setVaild();              // 标记为有效
};
```

**状态说明**：
- `UNTOUCH`：未处理状态
- `CALC`：正在计算状态
- `VALID`：已有效计算状态

### SymbolStatus

`SymbolStatus` 结构将 AST 节点与状态记录器关联起来。

```cpp
struct SymbolStatus {
  ASTNode *node;          // 关联的 AST 节点
  StatusRecorder status;  // 状态记录器
  
  SymbolStatus();
  SymbolStatus(ASTNode *target);
  SymbolStatus(const SymbolStatus &other) = default;
};
```

### ConstEvaluator

`ConstEvaluator` 是主要的求值器类，管理所有符号的求值过程。

```cpp
class ConstEvaluator {
private:
  Scope **current_scope_;  // 当前作用域指针的指针
  // 类型定义符号表
  std::unordered_map<std::string, SymbolStatus> type_def_symbols;
  // 常量符号表
  std::unordered_map<std::string, SymbolStatus> const_symbols;
  // 类型常量符号表（类型名 -> (常量名 -> SymbolStatus)）
  std::unordered_map<std::string, std::unordered_map<std::string, SymbolStatus>>
      type_const_symbols;
  // trait 常量符号表（trait 名 -> (常量名 -> SymbolStatus)）
  std::unordered_map<std::string, std::unordered_map<std::string, SymbolStatus>>
      trait_const_symbols;
      
public:
  // 构造和析构函数
  ConstEvaluator();
  ~ConstEvaluator();
  
  // 作用域管理
  void bindScopePointer(Scope **current_scope);
  void addBuiltInSymbol(const std::string &symbol);
  
  // 节点附加方法
  void attachNodeToTypeDef(ASTNode *node, const std::string &symbol);
  void attachNodeToConst(ASTNode *node, const std::string &symbol);
  void attachNodeToTypeConst(ASTNode *node, const std::string &struct_name,
                             const std::string &symbol);
  void attachNodeToTraitConst(ASTNode *node, const std::string &trait_name,
                              const std::string &symbol);
  
  // 求值方法
  void evaluateTypeSymbol(const std::string &symbol);
  void evaluateConstSymbol(const std::string &symbol);
  void evaluateTypeConst(const std::string &struct_name,
                         const std::string &symbol);
  void evaluateTraitConst(const std::string &trait_name,
                          const std::string &symbol);
  
  // 类型表达式和值表达式求值
  auto evaluateType(TypeNode *node) -> std::shared_ptr<TypeKind>;
  auto evaluateExprValue(ExprNode *node) -> std::shared_ptr<ConstInfo>;
};
```

## 上下文类型

```cpp
// 用于访问者的上下文枚举类
enum class ContextType { IN_TYPE_DEF, IN_TRAIT_DEF };
```

**上下文说明**：
- `IN_TYPE_DEF`：在类型定义上下文中
- `IN_TRAIT_DEF`：在 trait 定义上下文中

## 工作流程

### 1. 初始化阶段

```cpp
// 创建常量求值器
ConstEvaluator evaluator;

// 绑定当前作用域
evaluator.bindScopePointer(&current_scope);

// 添加内置符号
evaluator.addBuiltInSymbol("i32");
evaluator.addBuiltInSymbol("bool");
```

### 2. 节点附加阶段

```cpp
// 将 AST 节点附加到类型定义
evaluator.attachNodeToTypeDef(struct_node, "MyStruct");

// 将 AST 节点附加到常量
evaluator.attachNodeToConst(const_node, "MY_CONSTANT");

// 将 AST 节点附加到类型常量
evaluator.attachNodeToTypeConst(const_node, "MyStruct", "ASSOC_CONSTANT");

// 将 AST 节点附加到 trait 常量
evaluator.attachNodeToTraitConst(const_node, "MyTrait", "TRAIT_CONSTANT");
```

### 3. 求值阶段

```cpp
// 评估类型符号
evaluator.evaluateTypeSymbol("MyStruct");

// 评估常量符号
evaluator.evaluateConstSymbol("MY_CONSTANT");

// 评估类型常量
evaluator.evaluateTypeConst("MyStruct", "ASSOC_CONSTANT");

// 评估 trait 常量
evaluator.evaluateTraitConst("MyTrait", "TRAIT_CONSTANT");

// 评估类型表达式
auto type_kind = evaluator.evaluateType(type_node);

// 评估表达式值
auto const_info = evaluator.evaluateExprValue(expr_node);
```

## 设计特点

### 1. 状态管理

使用 `StatusRecorder` 跟踪每个符号的求值状态，有效避免循环依赖和重复计算：

- 首次访问时，状态从 `UNTOUCH` 变为 `CALC`
- 如果再次访问到 `CALC` 状态的符号，说明存在循环依赖
- 求值完成后，状态变为 `VALID`

### 2. 分层符号表

使用多层次的符号表结构，分别管理不同类型的符号：

- `type_def_symbols`：管理类型定义符号
- `const_symbols`：管理全局常量符号
- `type_const_symbols`：管理类型关联常量
- `trait_const_symbols`：管理 trait 关联常量

### 3. 作用域绑定

通过 `bindScopePointer` 方法绑定当前作用域指针，使得求值器可以访问和修改作用域中的符号信息。

## 使用示例

```cpp
// 创建作用域
Scope global_scope;
Scope* current_scope = &global_scope;

// 创建常量求值器
ConstEvaluator evaluator;
evaluator.bindScopePointer(&current_scope);

// 添加内置类型
evaluator.addBuiltInSymbol("i32");
evaluator.addBuiltInSymbol("bool");

// 假设有以下 AST 节点
ASTNode* struct_node = ...;  // 结构体定义节点
ASTNode* const_node = ...;   // 常量定义节点
ASTNode* type_node = ...;    // 类型表达式节点
ASTNode* expr_node = ...;    // 值表达式节点

// 附加节点到符号
evaluator.attachNodeToTypeDef(struct_node, "Point");
evaluator.attachNodeToConst(const_node, "ORIGIN");

// 执行求值
evaluator.evaluateTypeSymbol("Point");
evaluator.evaluateConstSymbol("ORIGIN");

// 评估表达式
auto type_kind = evaluator.evaluateType(type_node);
auto const_info = evaluator.evaluateExprValue(expr_node);
```

## 注意事项

1. **循环依赖检测**：求值器会检测符号之间的循环依赖，如果发现循环依赖会抛出异常。
2. **作用域生命周期**：确保绑定的作用域指针在求值器使用期间保持有效。
3. **线程安全**：当前的 `ConstEvaluator` 实现不是线程安全的，不适合在多线程环境中使用。
4. **内存管理**：使用智能指针管理符号和类型信息，避免内存泄漏。

## 总结

`ConstEvaluator` 是 AST 标注系统中的核心组件，负责常量表达式的求值和类型定义的处理。它通过状态管理和分层符号表，有效地避免了循环依赖和重复计算，为编译器的语义分析阶段提供了重要支持。