# TypeDef - 类型定义

## 简介

`TypeDef` 类及其派生类构成了 AST 标注系统中的类型定义体系，用于表示程序中的各种类型定义（结构体、枚举等），为编译器的类型系统实现提供了基础支持。
该多态类在 `Data/ASTAnnotation/TypeDef.hpp` 中声明，对应的实现位于 `Data/ASTAnnotation/TypeDef.cpp` 中。

## 继承结构
TypeDef
  ├── StructDef
  └── EnumDef

## 依赖类型

`TypeDef` 类依赖以下类：
声明中需要`SymbolFunctionInfo``SymbolConstInfo``TypeKind`的声明；
定义中需要以上类的实现。

## TypeDef 类接口与数据结构

``` cpp
class TypeDef {
private:
  std::string name_;
  std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>>
      type_consts_;
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>> methods_;
  std::unordered_map<std::string, std::shared_ptr<SymbolFunctionInfo>>
      associated_functions_;
  std::set<std::string> impl_traits_;

public:
  TypeDef(const std::string &name);
  virtual ~TypeDef();
  auto getName() const -> const std::string &;
  auto addConst(const std::string &name,
                std::shared_ptr<SymbolConstInfo> type_const) -> bool;
  auto getConst(const std::string &name) const
      -> std::shared_ptr<SymbolConstInfo>;
  auto addMethod(const std::string &name,
                 std::shared_ptr<SymbolFunctionInfo> method) -> bool;
  auto getMethod(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
  auto addAssociatedFunction(const std::string &name,
                             std::shared_ptr<SymbolFunctionInfo> function)
      -> bool;
  auto getAssociatedFunction(const std::string &name) const
      -> std::shared_ptr<SymbolFunctionInfo>;
  auto addImplTrait(const std::string &trait_name) -> bool;
};

class StructDef : public TypeDef {
private:
  std::unordered_map<std::string, std::shared_ptr<TypeKind>> members_;

public:
  StructDef(const std::string &name);
  StructDef(const std::string &name,
            const std::vector<std::string> &member_names,
            std::vector<std::shared_ptr<TypeKind>> &&member_types);
  ~StructDef() override;
  auto addMember(const std::string &name, std::shared_ptr<TypeKind> type)
      -> bool;
  auto getMember(const std::string &name) const -> std::shared_ptr<TypeKind>;
  auto getMemNum() const -> size_t;
};

class EnumDef : public TypeDef {
private:
  std::set<std::string> variants_;

public:
  EnumDef(const std::string &name);
  EnumDef(const std::string &name, const std::vector<std::string> &variants);
  ~EnumDef() override;
  auto addVariant(const std::string &variant) -> bool;
  auto getVariant(const std::string &variant) const -> bool;
  auto getVariants() const -> const std::vector<std::string>;
};
```

### 核心方法

#### getName

```cpp
auto getName() const -> const std::string &;
```

- **返回值**：`const std::string &`
- **功能**：获取类型名称
- **特点**：
  - 返回常量引用，避免拷贝
  - 用于类型标识和查找

#### addImplTrait

```cpp
auto addImplTrait(const std::string &trait_name) -> bool;
```

- **参数**：
  - `trait_name`：特征名称
- **返回值**：`bool`
- **功能**：添加类型实现的特征
- **返回值说明**：
  - `true`：添加成功
  - `false`：添加失败（通常是特征已存在）

#### getImplTraits

```cpp
auto getImplTraits() const -> const std::set<std::string> &;
```

- **返回值**：`const std::set<std::string> &`
- **功能**：获取类型实现的所有特征
- **特点**：
  - 返回常量引用，避免拷贝
  - 用于特征检查和类型比较

#### hasImplTrait

```cpp
auto hasImplTrait(const std::string &trait_name) const -> bool;
```

- **参数**：
  - `trait_name`：特征名称
- **返回值**：`bool`
- **功能**：检查类型是否实现了特定特征
- **返回值说明**：
  - `true`：类型实现了该特征
  - `false`：类型未实现该特征

## 派生类体系

### 1. StructDef - 结构体定义

```cpp
class StructDef : public TypeDef {
private:
  std::unordered_map<std::string, std::shared_ptr<TypeKind>> fields_;  // 字段映射

public:
  StructDef();
  StructDef(const std::string &name);
  ~StructDef() override;
  
  // 字段管理
  auto addField(const std::string &name, std::shared_ptr<TypeKind> type)
      -> bool;
  auto getField(const std::string &name) const
      -> std::shared_ptr<TypeKind>;
  auto getAllFields() const
      -> const std::unordered_map<std::string, std::shared_ptr<TypeKind>> &;
};
```

**成员变量**：
- `fields_`：字段名称到类型的映射

**功能**：
- 表示结构体类型定义
- 支持字段添加和访问
- 支持特征实现

**使用示例**：
```cpp
// 创建结构体定义
auto point_def = std::make_shared<StructDef>("Point");

// 添加字段
auto int_type = std::make_shared<TypeKindPath>(int_def);
point_def->addField("x", int_type);
point_def->addField("y", int_type);

// 实现特征
point_def->addImplTrait("Drawable");

// 获取字段信息
auto x_type = point_def->getField("x");
auto all_fields = point_def->getAllFields();
```

### 2. EnumDef - 枚举定义

```cpp
class EnumDef : public TypeDef {
private:
  std::unordered_map<std::string, std::shared_ptr<TypeKind>> variants_;  // 变体映射

public:
  EnumDef();
  EnumDef(const std::string &name);
  ~EnumDef() override;
  
  // 变体管理
  auto addVariant(const std::string &name, std::shared_ptr<TypeKind> type)
      -> bool;
  auto getVariant(const std::string &name) const
      -> std::shared_ptr<TypeKind>;
  auto getAllVariants() const
      -> const std::unordered_map<std::string, std::shared_ptr<TypeKind>> &;
};
```

**成员变量**：
- `variants_`：变体名称到类型的映射

**功能**：
- 表示枚举类型定义
- 支持变体添加和访问
- 支持特征实现

**使用示例**：
```cpp
// 创建枚举定义
auto color_def = std::make_shared<EnumDef>("Color");

// 添加变体
auto unit_type = std::make_shared<TypeKindUnit>();
color_def->addVariant("Red", unit_type);
color_def->addVariant("Green", unit_type);
color_def->addVariant("Blue", unit_type);

// 实现特征
color_def->addImplTrait("Displayable");

// 获取变体信息
auto red_type = color_def->getVariant("Red");
auto all_variants = color_def->getAllVariants();
```

## 设计特点

### 1. 多态设计

使用继承和多态实现不同类型的类型定义：

- **基类接口统一**：所有类型定义都继承自 `TypeDef`
- **类型安全**：通过虚函数确保类型安全
- **扩展性强**：易于添加新的类型定义

### 2. 特征系统集成

所有类型定义都支持特征实现：

- **特征列表**：使用 `std::set` 存储实现的特征
- **特征检查**：支持检查类型是否实现了特定特征
- **特征绑定**：支持类型与特征的动态绑定

### 3. 智能指针管理

使用 `std::shared_ptr` 管理类型定义对象：

- **内存安全**：自动管理内存，避免泄漏
- **共享所有权**：多个对象可以共享同一个类型定义
- **引用计数**：自动处理对象生命周期

## 使用模式

### 1. 创建类型定义

```cpp
// 创建结构体定义
auto struct_def = std::make_shared<StructDef>("MyStruct");
auto int_type = std::make_shared<TypeKindPath>(int_def);
struct_def->addField("field1", int_type);
struct_def->addField("field2", int_type);
struct_def->addImplTrait("SomeTrait");

// 创建枚举定义
auto enum_def = std::make_shared<EnumDef>("MyEnum");
auto unit_type = std::make_shared<TypeKindUnit>();
enum_def->addVariant("Variant1", unit_type);
enum_def->addVariant("Variant2", unit_type);
enum_def->addImplTrait("AnotherTrait");
```

### 2. 类型检查和转换

```cpp
// 检查类型定义类型
if (auto struct_def = std::dynamic_pointer_cast<StructDef>(type_def)) {
    // 处理结构体定义
    auto fields = struct_def->getAllFields();
} else if (auto enum_def = std::dynamic_pointer_cast<EnumDef>(type_def)) {
    // 处理枚举定义
    auto variants = enum_def->getAllVariants();
}
// ... 其他类型检查
```

### 3. 特征实现检查

```cpp
// 检查类型是否实现了特定特征
if (type_def->hasImplTrait("SomeTrait")) {
    // 类型实现了 SomeTrait
}

// 获取类型实现的所有特征
auto traits = type_def->getImplTraits();
```

## 与其他组件的关系

### 与 SymbolTypeInfo 的关系

`SymbolTypeInfo` 引用 `TypeDef` 对象：

```cpp
class SymbolTypeInfo {
private:
  std::shared_ptr<TypeDef> type_;  // 引用类型定义
  
public:
  auto getType() const -> std::shared_ptr<TypeDef>;
};
```

### 与 TypeKind 的关系

`TypeDef` 与 `TypeKind` 紧密关联：

```cpp
// TypeKindPath 引用 TypeDef
class TypeKindPath : public TypeKind {
private:
  std::shared_ptr<TypeDef> type_def_;  // 引用类型定义
  
public:
  auto getTypeDef() const -> std::shared_ptr<TypeDef>;
};
```

### 与 TraitDef 的关系

`TypeDef` 可以实现多个 `TraitDef`：

```cpp
class TypeDef {
private:
  std::set<std::string> impl_traits_;  // 实现的特征列表
  
public:
  auto addImplTrait(const std::string &trait_name) -> bool;
};
```

### 与 ConstEvaluator 的关系

`ConstEvaluator` 使用 `TypeDef` 进行类型符号求值：

```cpp
class ConstEvaluator {
public:
  void evaluateTypeSymbol(const std::string &symbol);
  // 内部会访问 TypeDef 对象
};
```

## 类型系统的工作流程

### 1. 类型定义

```cpp
// 定义结构体
auto point_def = std::make_shared<StructDef>("Point");
auto int_type = std::make_shared<TypeKindPath>(int_def);
point_def->addField("x", int_type);
point_def->addField("y", int_type);
```

### 2. 类型注册

```cpp
// 将类型注册到作用域
auto type_info = std::make_shared<SymbolTypeInfo>("Point", point_def);
scope->addType("Point", type_info);
```

### 3. 类型使用

```cpp
// 创建类型引用
auto point_type = std::make_shared<TypeKindPath>(point_def);

// 使用类型创建变量
auto var_info = std::make_shared<SymbolVariableInfo>("origin", point_type, false);
scope->addVarible("origin", var_info);
```

### 4. 特征实现检查

```cpp
// 检查类型是否实现了特征
if (point_def->hasImplTrait("Drawable")) {
    // 类型实现了 Drawable 特征
}
```

## 注意事项

1. **名称唯一性**：在同一作用域中，类型名称必须唯一
2. **字段/变体唯一性**：在同一类型定义中，字段或变体名称必须唯一
3. **生命周期**：确保类型定义对象的生命周期长于使用它的对象
4. **内存管理**：正确使用智能指针，避免循环引用
5. **线程安全**：当前的 `TypeDef` 实现不是线程安全的
6. **特征实现**：确保类型实现特征时提供了所有必需的方法和常量
