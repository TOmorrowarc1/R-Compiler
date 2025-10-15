# TraitDef - 特征定义

## 简介

`TraitDef` 类是 AST 标注系统中用于定义语言特征（trait）的核心组件。它管理特征中的常量、方法和关联函数，为编译器的特征系统实现提供了基础支持。特征系统允许定义一组行为接口，类型可以实现这些特征以提供特定的功能。

## 类结构

```cpp
class TraitDef {
private:
  std::string name_;  // 特征名称
  std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>
      trait_consts_;  // 特征常量映射
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>
      trait_methods_;  // 特征方法映射
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>
      trait_functions_;  // 特征关联函数映射

public:
  // 构造和析构函数
  TraitDef();
  TraitDef(const std::string &name);
  ~TraitDef();
  
  // 常量管理
  auto addConst(const std::string &name,
                std::shared_ptr<SymbolConstInfo> trait_const) -> bool;
  auto getConst(const std::string &name) const
      -> std::shared_ptr<SymbolConstInfo>;
  auto getAllConst() const
      -> std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>;
  
  // 关联函数管理
  auto addFunction(const std::string &name,
                   std::shared_ptr<SymbolFunctionInfo> method) -> bool;
  auto getFunction(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
  auto getAllFunction() const
      -> std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>;
  
  // 方法管理
  auto addMethod(const std::string &name,
                 std::shared_ptr<SymbolFunctionInfo> method) -> bool;
  auto getMethod(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
  auto getAllMethod() const
      -> std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>;
};
```

## 成员变量

### name_

```cpp
std::string name_;
```

- **类型**：`std::string`
- **描述**：特征的名称
- **作用**：标识特征，用于特征查找和引用
- **特点**：
  - 在同一作用域中必须唯一
  - 用于特征实现和类型绑定的标识

### trait_consts_

```cpp
std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>> trait_consts_;
```

- **类型**：`std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>`
- **描述**：特征常量映射
- **作用**：存储特征中定义的常量
- **特点**：
  - 使用哈希表实现快速查找
  - 使用 `shared_ptr` 支持共享所有权
  - 常量在特征的所有实现中必须具有相同的值

### trait_methods_

```cpp
std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>> trait_methods_;
```

- **类型**：`std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>`
- **描述**：特征方法映射
- **作用**：存储特征中定义的方法
- **特点**：
  - 使用哈希表实现快速查找
  - 使用 `shared_ptr` 支持共享所有权
  - 方法是特征的核心组成部分，定义了特征的行为

### trait_functions_

```cpp
std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>> trait_functions_;
```

- **类型**：`std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>`
- **描述**：特征关联函数映射
- **作用**：存储特征中定义的关联函数
- **特点**：
  - 使用哈希表实现快速查找
  - 使用 `shared_ptr` 支持共享所有权
  - 关联函数是与特征相关但不依赖于特定实例的函数

## 成员函数

### 构造函数

#### 1. 默认构造函数

```cpp
TraitDef();
```

- **功能**：创建未命名的特征定义
- **特点**：
  - `name_` 设置为空字符串
  - 适用于临时创建或后续命名的特征

#### 2. 带名称的构造函数

```cpp
TraitDef(const std::string &name);
```

- **参数**：
  - `name`：特征名称
- **功能**：创建带名称的特征定义
- **特点**：
  - `name_` 设置为传入的名称
  - 适用于直接创建命名的特征

### 常量管理

#### addConst

```cpp
auto addConst(const std::string &name,
              std::shared_ptr<SymbolConstInfo> trait_const) -> bool;
```

- **参数**：
  - `name`：常量名称
  - `trait_const`：常量符号信息
- **返回值**：`bool`
- **功能**：添加常量到特征定义
- **返回值说明**：
  - `true`：添加成功
  - `false`：添加失败（通常是名称冲突）

#### getConst

```cpp
auto getConst(const std::string &name) const
    -> std::shared_ptr<SymbolConstInfo>;
```

- **参数**：
  - `name`：常量名称
- **返回值**：`std::shared_ptr<SymbolConstInfo>`
- **功能**：获取特征中的常量
- **特点**：
  - 如果常量不存在，返回 `nullptr`
  - 返回智能指针，支持共享所有权

#### getAllConst

```cpp
auto getAllConst() const
    -> std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>;
```

- **返回值**：`std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>`
- **功能**：获取特征中的所有常量
- **特点**：
  - 返回常量映射的副本
  - 用于特征实现时的常量检查

### 关联函数管理

#### addFunction

```cpp
auto addFunction(const std::string &name,
                 std::shared_ptr<SymbolFunctionInfo> method) -> bool;
```

- **参数**：
  - `name`：函数名称
  - `method`：函数符号信息
- **返回值**：`bool`
- **功能**：添加关联函数到特征定义
- **返回值说明**：
  - `true`：添加成功
  - `false`：添加失败（通常是名称冲突）

#### getFunction

```cpp
auto getFunction(const std::string &name) const
    -> std::shared_ptr<SymbolFunctionInfo>;
```

- **参数**：
  - `name`：函数名称
- **返回值**：`std::shared_ptr<SymbolFunctionInfo>`
- **功能**：获取特征中的关联函数
- **特点**：
  - 如果函数不存在，返回 `nullptr`
  - 返回智能指针，支持共享所有权

#### getAllFunction

```cpp
auto getAllFunction() const
    -> std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>;
```

- **返回值**：`std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>`
- **功能**：获取特征中的所有关联函数
- **特点**：
  - 返回函数映射的副本
  - 用于特征实现时的函数检查

### 方法管理

#### addMethod

```cpp
auto addMethod(const std::string &name,
               std::shared_ptr<SymbolFunctionInfo> method) -> bool;
```

- **参数**：
  - `name`：方法名称
  - `method`：方法符号信息
- **返回值**：`bool`
- **功能**：添加方法到特征定义
- **返回值说明**：
  - `true`：添加成功
  - `false`：添加失败（通常是名称冲突）

#### getMethod

```cpp
auto getMethod(const std::string &name) const
    -> std::shared_ptr<SymbolFunctionInfo>;
```

- **参数**：
  - `name`：方法名称
- **返回值**：`std::shared_ptr<SymbolFunctionInfo>`
- **功能**：获取特征中的方法
- **特点**：
  - 如果方法不存在，返回 `nullptr`
  - 返回智能指针，支持共享所有权

#### getAllMethod

```cpp
auto getAllMethod() const
    -> std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>;
```

- **返回值**：`std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>`
- **功能**：获取特征中的所有方法
- **特点**：
  - 返回方法映射的副本
  - 用于特征实现时的方法检查

## 使用示例

### 1. 创建特征定义

```cpp
// 创建特征定义
auto drawable_trait = std::make_shared<TraitDef>("Drawable");

// 添加常量
auto color_const = std::make_shared<SymbolConstInfo>("DEFAULT_COLOR");
// 设置常量值...
drawable_trait->addConst("DEFAULT_COLOR", color_const);

// 添加方法
auto draw_params = std::vector<std::shared_ptr<TypeKind>>();
auto draw_ret_type = std::make_shared<TypeKindPath>(void_def);
auto draw_method = std::make_shared<SymbolFunctionInfo>("draw", draw_ret_type, std::move(draw_params));
draw_method->setFnType(SymbolFunctionInfo::FnType::Method);
drawable_trait->addMethod("draw", draw_method);

// 添加关联函数
auto create_params = std::vector<std::shared_ptr<TypeKind>>();
auto create_ret_type = std::make_shared<TypeKindPath>(shape_def);
auto create_func = std::make_shared<SymbolFunctionInfo>("create", create_ret_type, std::move(create_params));
drawable_trait->addFunction("create", create_func);
```

### 2. 访问特征定义

```cpp
// 获取特征名称
auto name = drawable_trait->getName();  // 返回 "Drawable"

// 获取特定方法
auto draw_method = drawable_trait->getMethod("draw");

// 获取所有方法
auto all_methods = drawable_trait->getAllMethod();

// 获取特定常量
auto default_color = drawable_trait->getConst("DEFAULT_COLOR");

// 获取所有常量
auto all_consts = drawable_trait->getAllConst();
```

## 设计特点

### 1. 分类管理

将特征中的元素分为三类进行管理：

- **常量**：特征关联的常量值
- **方法**：依赖于实例的行为
- **关联函数**：不依赖于实例的函数

### 2. 哈希表存储

使用哈希表存储特征元素：

- **快速查找**：支持 O(1) 时间复杂度的查找
- **名称唯一性**：确保同一类型元素的名称唯一
- **高效访问**：支持高效的元素访问

### 3. 智能指针管理

使用 `std::shared_ptr` 管理特征元素：

- **内存安全**：自动管理内存，避免泄漏
- **共享所有权**：多个对象可以共享同一个特征元素
- **引用计数**：自动处理对象生命周期

## 与其他组件的关系

### 与 SymbolTraitInfo 的关系

`SymbolTraitInfo` 引用 `TraitDef` 对象：

```cpp
class SymbolTraitInfo {
private:
  std::shared_ptr<TraitDef> trait_;  // 引用特征定义
  
public:
  auto getTrait() const -> std::shared_ptr<TraitDef>;
};
```

### 与 SymbolFunctionInfo 的关系

`TraitDef` 包含多个 `SymbolFunctionInfo` 对象：

```cpp
class TraitDef {
private:
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>
      trait_methods_;  // 特征方法
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>
      trait_functions_;  // 特征关联函数
};
```

### 与 SymbolConstInfo 的关系

`TraitDef` 包含多个 `SymbolConstInfo` 对象：

```cpp
class TraitDef {
private:
  std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>
      trait_consts_;  // 特征常量
};
```

### 与 TypeDef 的关系

`TypeDef` 可以实现多个 `TraitDef`：

```cpp
class TypeDef {
private:
  std::set<std::string> impl_traits_;  // 实现的特征列表
  
public:
  auto addImplTrait(const std::string &trait_name) -> bool;
};
```

## 特征系统的工作流程

### 1. 特征定义

```cpp
// 定义特征
auto drawable_trait = std::make_shared<TraitDef>("Drawable");
// 添加方法和常量...
```

### 2. 特征注册

```cpp
// 将特征注册到作用域
auto trait_info = std::make_shared<SymbolTraitInfo>("Drawable", drawable_trait);
scope->addTrait("Drawable", trait_info);
```

### 3. 类型实现特征

```cpp
// 类型实现特征
auto shape_type = std::make_shared<TypeDef>("Shape");
shape_type->addImplTrait("Drawable");
```

### 4. 特征检查

```cpp
// 检查类型是否实现了特征
auto type_info = scope->getType("Shape");
auto type_def = type_info->getType();
// 检查 type_def 是否实现了 "Drawable" 特征
```

## 注意事项

1. **名称唯一性**：在同一特征中，常量、方法和关联函数的名称必须唯一
2. **生命周期**：确保特征定义对象的生命周期长于使用它的对象
3. **内存管理**：正确使用智能指针，避免循环引用
4. **线程安全**：当前的 `TraitDef` 实现不是线程安全的
5. **特征实现**：确保类型实现特征时提供了所有必需的方法和常量

## 总结

`TraitDef` 类是 AST 标注系统中用于定义语言特征的核心组件。它通过分类管理特征中的常量、方法和关联函数，为编译器的特征系统实现提供了基础支持。与 `SymbolTraitInfo`、`SymbolFunctionInfo`、`SymbolConstInfo` 和 `TypeDef` 等组件紧密协作，支持特征的定义、注册、实现和检查，为编译器的语义分析阶段提供了强大的特征系统支持。