# Scope - 作用域管理

## 简介

`Scope` 类是 AST 标注系统中负责管理符号作用域的核心组件。它实现了层次化的作用域结构，支持符号的添加、查找和生命周期管理，为编译器的语义分析提供了基础支持。

## 类结构

```cpp
class Scope {
private:
  Scope *parent_;                                    // 父作用域指针
  uint32_t index_now;                               // 当前子作用域索引
  std::vector<std::unique_ptr<Scope>> children_;    // 子作用域列表
  std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>> consts_;  // 常量符号表
  std::unordered_map<std::string, std::shared_ptr<SymbolInfo>> symbols_;       // 通用符号表
  std::unordered_map<std::string, std::shared_ptr<SymbolTypeInfo>> types_;     // 类型符号表
  std::unordered_map<std::string, std::shared_ptr<SymbolTraitInfo>> traits_;   // 特征符号表
  
  // 私有方法：递归查找常量符号
  auto getConstSymbol(const std::string &name, bool is_recursive) const
      -> std::shared_ptr<SymbolConstInfo>;

public:
  // 构造和析构函数
  Scope();
  Scope(Scope *parent);
  ~Scope();
  
  // 作用域层次管理
  auto getParent() const -> Scope *;
  auto addNextChildScope() -> Scope *;
  auto getNextChildScope() -> Scope *;
  void resetIndex();
  
  // 符号管理
  auto addConst(const std::string &name,
                std::shared_ptr<SymbolConstInfo> const_var) -> bool;
  auto addFunction(const std::string &name,
                   std::shared_ptr<SymbolFunctionInfo> function) -> bool;
  auto addVarible(const std::string &name,
                  std::shared_ptr<SymbolVariableInfo> symbol) -> bool;
  auto getSymbol(const std::string &name) const -> std::shared_ptr<SymbolInfo>;
  auto addType(const std::string &name, std::shared_ptr<SymbolTypeInfo> symbol)
      -> bool;
  auto getType(const std::string &name) const
      -> std::shared_ptr<SymbolTypeInfo>;
  auto addTrait(const std::string &name,
                std::shared_ptr<SymbolTraitInfo> symbol) -> bool;
  auto getTrait(const std::string &name) const
      -> std::shared_ptr<SymbolTraitInfo>;
};
```

## 成员变量

### parent_

```cpp
Scope *parent_;
```

- **类型**：`Scope*`
- **描述**：指向父作用域的指针
- **作用**：支持作用域层次结构和符号查找
- **特点**：
  - 根作用域的 `parent_` 为 `nullptr`
  - 用于实现递归符号查找

### index_now

```cpp
uint32_t index_now;
```

- **类型**：`uint32_t`
- **描述**：当前子作用域的索引
- **作用**：管理子作用域的创建和访问
- **特点**：
  - 用于实现子作用域的顺序访问
  - 可以通过 `resetIndex()` 重置

### children_

```cpp
std::vector<std::unique_ptr<Scope>> children_;
```

- **类型**：`std::vector<std::unique_ptr<Scope>>`
- **描述**：子作用域列表
- **作用**：管理作用域的层次结构
- **特点**：
  - 使用 `unique_ptr` 确保明确的内存所有权
  - 支持动态添加子作用域

### consts_

```cpp
std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>> consts_;
```

- **类型**：`std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>`
- **描述**：常量符号表
- **作用**：存储当前作用域中的常量符号
- **特点**：
  - 使用哈希表实现快速查找
  - 使用 `shared_ptr` 支持共享所有权

### symbols_

```cpp
std::unordered_map<std::string, std::shared_ptr<SymbolInfo>> symbols_;
```

- **类型**：`std::unordered_map<std::string, std::shared_ptr<SymbolInfo>>`
- **描述**：通用符号表
- **作用**：存储当前作用域中的通用符号（变量、函数等）
- **特点**：
  - 使用哈希表实现快速查找
  - 使用 `shared_ptr` 支持多态和共享所有权

### types_

```cpp
std::unordered_map<std::string, std::shared_ptr<SymbolTypeInfo>> types_;
```

- **类型**：`std::unordered_map<std::string, std::shared_ptr<SymbolTypeInfo>>`
- **描述**：类型符号表
- **作用**：存储当前作用域中的类型符号
- **特点**：
  - 使用哈希表实现快速查找
  - 使用 `shared_ptr` 支持共享所有权

### traits_

```cpp
std::unordered_map<std::string, std::shared_ptr<SymbolTraitInfo>> traits_;
```

- **类型**：`std::unordered_map<std::string, std::shared_ptr<SymbolTraitInfo>>`
- **描述**：特征符号表
- **作用**：存储当前作用域中的特征符号
- **特点**：
  - 使用哈希表实现快速查找
  - 使用 `shared_ptr` 支持共享所有权

## 成员函数

### 构造函数

#### 1. 默认构造函数

```cpp
Scope();
```

- **功能**：创建根作用域
- **特点**：
  - `parent_` 设置为 `nullptr`
  - `index_now` 初始化为 0
  - 适用于全局作用域

#### 2. 带父作用域的构造函数

```cpp
Scope(Scope *parent);
```

- **参数**：
  - `parent`：父作用域指针
- **功能**：创建子作用域
- **特点**：
  - `parent_` 设置为传入的父作用域
  - 自动将当前作用域添加到父作用域的子作用域列表中
  - 适用于局部作用域

### 作用域层次管理

#### getParent

```cpp
auto getParent() const -> Scope *;
```

- **返回值**：`Scope*`
- **功能**：获取父作用域指针
- **特点**：
  - 根作用域返回 `nullptr`
  - 用于递归符号查找

#### addNextChildScope

```cpp
auto addNextChildScope() -> Scope *;
```

- **返回值**：`Scope*`
- **功能**：创建并添加新的子作用域
- **特点**：
  - 创建新的子作用域，当前作用域作为父作用域
  - 更新 `index_now`
  - 返回新创建的子作用域指针

#### getNextChildScope

```cpp
auto getNextChildScope() -> Scope *;
```

- **返回值**：`Scope*`
- **功能**：获取当前索引的子作用域
- **特点**：
  - 根据 `index_now` 获取子作用域
  - 不修改 `index_now`
  - 如果索引超出范围，返回 `nullptr`

#### resetIndex

```cpp
void resetIndex();
```

- **功能**：重置子作用域索引
- **特点**：
  - 将 `index_now` 重置为 0
  - 用于重新开始遍历子作用域

### 符号管理

#### addConst

```cpp
auto addConst(const std::string &name,
              std::shared_ptr<SymbolConstInfo> const_var) -> bool;
```

- **参数**：
  - `name`：常量名称
  - `const_var`：常量符号信息
- **返回值**：`bool`
- **功能**：添加常量符号到当前作用域
- **返回值说明**：
  - `true`：添加成功
  - `false`：添加失败（通常是名称冲突）

#### addFunction

```cpp
auto addFunction(const std::string &name,
                 std::shared_ptr<SymbolFunctionInfo> function) -> bool;
```

- **参数**：
  - `name`：函数名称
  - `function`：函数符号信息
- **返回值**：`bool`
- **功能**：添加函数符号到当前作用域
- **返回值说明**：
  - `true`：添加成功
  - `false`：添加失败（通常是名称冲突）

#### addVarible

```cpp
auto addVarible(const std::string &name,
                std::shared_ptr<SymbolVariableInfo> symbol) -> bool;
```

- **参数**：
  - `name`：变量名称
  - `symbol`：变量符号信息
- **返回值**：`bool`
- **功能**：添加变量符号到当前作用域
- **返回值说明**：
  - `true`：添加成功
  - `false`：添加失败（通常是名称冲突）

#### getSymbol

```cpp
auto getSymbol(const std::string &name) const -> std::shared_ptr<SymbolInfo>;
```

- **参数**：
  - `name`：符号名称
- **返回值**：`std::shared_ptr<SymbolInfo>`
- **功能**：查找符号（递归查找）
- **特点**：
  - 首先在当前作用域查找
  - 如果找不到，递归在父作用域查找
  - 如果最终找不到，返回 `nullptr`

#### addType

```cpp
auto addType(const std::string &name, std::shared_ptr<SymbolTypeInfo> symbol)
    -> bool;
```

- **参数**：
  - `name`：类型名称
  - `symbol`：类型符号信息
- **返回值**：`bool`
- **功能**：添加类型符号到当前作用域
- **返回值说明**：
  - `true`：添加成功
  - `false`：添加失败（通常是名称冲突）

#### getType

```cpp
auto getType(const std::string &name) const
    -> std::shared_ptr<SymbolTypeInfo>;
```

- **参数**：
  - `name`：类型名称
- **返回值**：`std::shared_ptr<SymbolTypeInfo>`
- **功能**：查找类型符号（递归查找）
- **特点**：
  - 首先在当前作用域查找
  - 如果找不到，递归在父作用域查找
  - 如果最终找不到，返回 `nullptr`

#### addTrait

```cpp
auto addTrait(const std::string &name,
              std::shared_ptr<SymbolTraitInfo> symbol) -> bool;
```

- **参数**：
  - `name`：特征名称
  - `symbol`：特征符号信息
- **返回值**：`bool`
- **功能**：添加特征符号到当前作用域
- **返回值说明**：
  - `true`：添加成功
  - `false`：添加失败（通常是名称冲突）

#### getTrait

```cpp
auto getTrait(const std::string &name) const
    -> std::shared_ptr<SymbolTraitInfo>;
```

- **参数**：
  - `name`：特征名称
- **返回值**：`std::shared_ptr<SymbolTraitInfo>`
- **功能**：查找特征符号（递归查找）
- **特点**：
  - 首先在当前作用域查找
  - 如果找不到，递归在父作用域查找
  - 如果最终找不到，返回 `nullptr`

### 私有方法

#### getConstSymbol

```cpp
auto getConstSymbol(const std::string &name, bool is_recursive) const
    -> std::shared_ptr<SymbolConstInfo>;
```

- **参数**：
  - `name`：常量名称
  - `is_recursive`：是否递归查找
- **返回值**：`std::shared_ptr<SymbolConstInfo>`
- **功能**：查找常量符号
- **特点**：
  - 根据 `is_recursive` 参数决定是否递归查找
  - 私有方法，供内部使用

## 使用示例

### 1. 创建作用域层次结构

```cpp
// 创建全局作用域
auto global_scope = std::make_unique<Scope>();

// 创建函数作用域
auto func_scope = std::make_unique<Scope>(global_scope.get());

// 创建块作用域
auto block_scope = std::make_unique<Scope>(func_scope.get());
```

### 2. 添加符号

```cpp
// 添加类型符号
auto type_info = std::make_shared<SymbolTypeInfo>("MyType", type_def);
global_scope->addType("MyType", type_info);

// 添加函数符号
auto func_info = std::make_shared<SymbolFunctionInfo>("myFunc", return_type, std::move(params));
global_scope->addFunction("myFunc", func_info);

// 添加变量符号
auto var_info = std::make_shared<SymbolVariableInfo>("x", int_type, false);
func_scope->addVarible("x", var_info);

// 添加常量符号
auto const_info = std::make_shared<SymbolConstInfo>("PI");
const_info->setValue(std::make_shared<ConstInfo>(float_type, pi_value));
global_scope->addConst("PI", const_info);
```

### 3. 查找符号

```cpp
// 查找类型符号
auto type_info = global_scope->getType("MyType");

// 查找函数符号
auto func_info = global_scope->getSymbol("myFunc");

// 查找变量符号（递归查找）
auto var_info = block_scope->getSymbol("x");

// 查找常量符号
auto const_info = global_scope->getSymbol("PI");
```

### 4. 管理子作用域

```cpp
// 添加子作用域
auto child_scope = global_scope->addNextChildScope();

// 获取子作用域
auto current_child = global_scope->getNextChildScope();

// 重置索引
global_scope->resetIndex();
```

## 设计特点

### 1. 层次化作用域

使用树形结构实现层次化作用域：

- **父子关系**：每个作用域（除根作用域外）都有一个父作用域
- **递归查找**：符号查找支持递归向上查找
- **封装性**：内层作用域可以访问外层作用域的符号，反之不行

### 2. 分类符号表

使用不同的符号表存储不同类型的符号：

- **consts_**：存储常量符号
- **symbols_**：存储通用符号（变量、函数等）
- **types_**：存储类型符号
- **traits_**：存储特征符号

### 3. 智能指针管理

使用智能指针管理作用域和符号对象：

- **unique_ptr**：管理子作用域，确保明确的内存所有权
- **shared_ptr**：管理符号对象，支持共享所有权和多态

## 与其他组件的关系

### 与 SymbolInfo 的关系

`Scope` 管理各种 `SymbolInfo` 派生类对象：

```cpp
// SymbolInfo 的派生类
class SymbolVariableInfo : public SymbolInfo { /* ... */ };
class SymbolFunctionInfo : public SymbolInfo { /* ... */ };
class SymbolTypeInfo : public SymbolInfo { /* ... */ };
class SymbolTraitInfo : public SymbolInfo { /* ... */ };
class SymbolConstInfo : public SymbolInfo { /* ... */ };
```

### 与 ConstEvaluator 的关系

`ConstEvaluator` 使用 `Scope` 进行符号查找和更新：

```cpp
class ConstEvaluator {
private:
  Scope **current_scope_;  // 当前作用域指针的指针
  
public:
  void bindScopePointer(Scope **current_scope);
  // ...
};
```

## 注意事项

1. **名称冲突**：添加符号时检查名称冲突，避免重复定义
2. **生命周期**：确保作用域对象的生命周期长于使用它的对象
3. **内存管理**：正确使用智能指针，避免循环引用
4. **线程安全**：当前的 `Scope` 实现不是线程安全的
5. **递归深度**：深层嵌套的作用域可能导致递归查找性能下降

## 总结

`Scope` 类是 AST 标注系统中负责管理符号作用域的核心组件。它通过层次化的作用域结构和分类的符号表，为编译器的语义分析提供了强大的符号管理能力。与 `SymbolInfo`、`ConstEvaluator` 等组件紧密协作，支持符号的添加、查找和生命周期管理，为编译器的类型检查、常量求值等功能提供了坚实的基础。