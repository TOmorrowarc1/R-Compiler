# TypeKind - 类型种类

## 简介

`TypeKind` 类及其派生类构成了 AST 标注系统中的类型种类表示体系。这个多态类层次结构用于表示程序中的各种类型（基本类型、复合类型、引用类型等），为编译器的类型检查和类型推导提供了基础支持。

## 基类结构

```cpp
class TypeKind {
public:
  TypeKind() = default;
  virtual ~TypeKind() = default;
  virtual auto isEqual(const TypeKind *other) const -> bool = 0;
};
```

### 核心方法

#### isEqual

```cpp
virtual auto isEqual(const TypeKind *other) const -> bool = 0;
```

- **参数**：
  - `other`：要比较的类型种类指针
- **返回值**：`bool`
- **功能**：比较两个类型种类是否相等
- **特点**：
  - 纯虚函数，必须在派生类中实现
  - 支持多态比较
  - 用于类型检查和类型推导

## 派生类体系

### 1. TypeKindPath - 路径类型

```cpp
class TypeKindPath : public TypeKind {
private:
  std::shared_ptr<TypeDef> type_def_;  // 类型定义引用

public:
  TypeKindPath(std::shared_ptr<TypeDef> type_def);
  ~TypeKindPath() override;
  auto isEqual(const TypeKind *other) const -> bool override;
  auto getTypeDef() const -> std::shared_ptr<TypeDef>;
};
```

**成员变量**：
- `type_def_`：类型定义引用

**功能**：
- 表示用户定义的类型（结构体、枚举等）
- 支持类型定义的引用
- 用于自定义类型的类型检查

**使用示例**：
```cpp
// 创建结构体定义
auto point_def = std::make_shared<StructDef>("Point");
// 添加字段...

// 创建路径类型
auto point_type = std::make_shared<TypeKindPath>(point_def);

// 获取类型定义
auto type_def = point_type->getTypeDef();

// 类型比较
auto other_type = std::make_shared<TypeKindPath>(point_def);
if (point_type->isEqual(other_type.get())) {
    // 类型相同
}
```

### 2. TypeKindArray - 数组类型

```cpp
class TypeKindArray : public TypeKind {
private:
  std::shared_ptr<TypeKind> base_type_;  // 基本类型
  uint32_t size_;                        // 数组大小

public:
  TypeKindArray(std::shared_ptr<TypeKind> base_type, uint32_t size);
  ~TypeKindArray() override;
  auto isEqual(const TypeKind *other) const -> bool override;
  auto getBaseType() const -> std::shared_ptr<TypeKind>;
  auto getSize() const -> uint32_t;
};
```

**成员变量**：
- `base_type_`：基本类型
- `size_`：数组大小

**功能**：
- 表示固定大小的数组类型
- 支持多维数组（通过嵌套）
- 用于数组类型的类型检查

**使用示例**：
```cpp
// 创建基本类型
auto int_type = std::make_shared<TypeKindPath>(int_def);

// 创建数组类型
auto int_array_type = std::make_shared<TypeKindArray>(int_type, 10);

// 获取数组信息
auto base_type = int_array_type->getBaseType();
auto size = int_array_type->getSize();  // 返回 10

// 类型比较
auto other_array = std::make_shared<TypeKindArray>(int_type, 10);
if (int_array_type->isEqual(other_array.get())) {
    // 类型相同
}
```

### 3. TypeKindRef - 引用类型

```cpp
class TypeKindRef : public TypeKind {
private:
  std::shared_ptr<TypeKind> ref_type_;  // 引用的类型
  bool is_mutable_;                      // 是否可变

public:
  TypeKindRef(std::shared_ptr<TypeKind> ref_type, bool is_mutable);
  ~TypeKindRef() override;
  auto isEqual(const TypeKind *other) const -> bool override;
  auto getRefType() const -> std::shared_ptr<TypeKind>;
  auto isMutable() const -> bool;
};
```

**成员变量**：
- `ref_type_`：引用的类型
- `is_mutable_`：是否可变

**功能**：
- 表示引用类型（类似于 Rust 的 & 和 &mut）
- 支持可变和不可变引用
- 用于引用类型的类型检查

**使用示例**：
```cpp
// 创建基本类型
auto int_type = std::make_shared<TypeKindPath>(int_def);

// 创建不可变引用类型
auto int_ref_type = std::make_shared<TypeKindRef>(int_type, false);

// 创建可变引用类型
auto int_mut_ref_type = std::make_shared<TypeKindRef>(int_type, true);

// 获取引用信息
auto ref_type = int_ref_type->getRefType();
auto is_mutable = int_mut_ref_type->isMutable();  // 返回 true

// 类型比较
auto other_ref = std::make_shared<TypeKindRef>(int_type, false);
if (int_ref_type->isEqual(other_ref.get())) {
    // 类型相同
}
```

### 4. TypeKindUnit - 单元类型

```cpp
class TypeKindUnit : public TypeKind {
public:
  TypeKindUnit();
  ~TypeKindUnit() override;
  auto isEqual(const TypeKind *other) const -> bool override;
};
```

**功能**：
- 表示单元类型（类似于 Rust 的 () 或 C/C++ 的 void）
- 用于函数返回类型或无参数函数
- 用于单元类型的类型检查

**使用示例**：
```cpp
// 创建单元类型
auto unit_type = std::make_shared<TypeKindUnit>();

// 类型比较
auto other_unit = std::make_shared<TypeKindUnit>();
if (unit_type->isEqual(other_unit.get())) {
    // 类型相同
}
```

## 设计特点

### 1. 多态设计

使用继承和多态实现不同类型的类型种类：

- **基类接口统一**：所有类型种类都继承自 `TypeKind`
- **类型安全**：通过虚函数确保类型安全
- **扩展性强**：易于添加新的类型种类

### 2. 类型比较

通过 `isEqual` 方法实现类型比较：

- **多态比较**：支持不同类型种类的比较
- **深度比较**：对于复合类型，递归比较组成部分
- **类型一致性**：确保类型系统的完整性

### 3. 智能指针管理

使用 `std::shared_ptr` 管理类型种类对象：

- **内存安全**：自动管理内存，避免泄漏
- **共享所有权**：多个对象可以共享同一个类型种类
- **引用计数**：自动处理对象生命周期

## 使用模式

### 1. 创建类型种类

```cpp
// 创建路径类型
auto struct_def = std::make_shared<StructDef>("Point");
auto point_type = std::make_shared<TypeKindPath>(struct_def);

// 创建数组类型
auto int_type = std::make_shared<TypeKindPath>(int_def);
auto int_array_type = std::make_shared<TypeKindArray>(int_type, 10);

// 创建引用类型
auto int_ref_type = std::make_shared<TypeKindRef>(int_type, false);
auto int_mut_ref_type = std::make_shared<TypeKindRef>(int_type, true);

// 创建单元类型
auto unit_type = std::make_shared<TypeKindUnit>();
```

### 2. 类型检查和转换

```cpp
// 检查类型种类
if (auto path_type = std::dynamic_pointer_cast<TypeKindPath>(type_kind)) {
    // 处理路径类型
    auto type_def = path_type->getTypeDef();
} else if (auto array_type = std::dynamic_pointer_cast<TypeKindArray>(type_kind)) {
    // 处理数组类型
    auto base_type = array_type->getBaseType();
    auto size = array_type->getSize();
} else if (auto ref_type = std::dynamic_pointer_cast<TypeKindRef>(type_kind)) {
    // 处理引用类型
    auto ref_type = ref_type->getRefType();
    auto is_mutable = ref_type->isMutable();
}
// ... 其他类型检查
```

### 3. 类型比较

```cpp
// 比较类型种类
if (type_kind1->isEqual(type_kind2.get())) {
    // 类型相同
}
```

## 与其他组件的关系

### 与 ConstInfo 的关系

`ConstInfo` 包含 `TypeKind`：

```cpp
class ConstInfo {
private:
  std::shared_ptr<TypeKind> type_;  // 包含类型种类
  
public:
  auto getType() const -> std::shared_ptr<TypeKind>;
};
```

### 与 SymbolInfo 的关系

`SymbolInfo` 派生类使用 `TypeKind`：

```cpp
class SymbolVariableInfo : public SymbolInfo {
private:
  std::shared_ptr<TypeKind> type_;  // 变量类型
  
public:
  auto getType() const -> std::shared_ptr<TypeKind>;
};

class SymbolFunctionInfo : public SymbolInfo {
private:
  std::shared_ptr<TypeKind> return_type_;  // 返回类型
  std::vector<std::shared_ptr<TypeKind>> parameters_;  // 参数类型列表
  
public:
  auto getReturnType() const -> std::shared_ptr<TypeKind>;
  auto getParametersType() const -> const std::vector<std::shared_ptr<TypeKind>> &;
};
```

### 与 TypeDef 的关系

`TypeKindPath` 引用 `TypeDef`：

```cpp
class TypeKindPath : public TypeKind {
private:
  std::shared_ptr<TypeDef> type_def_;  // 引用类型定义
  
public:
  auto getTypeDef() const -> std::shared_ptr<TypeDef>;
};
```

### 与 ConstEvaluator 的关系

`ConstEvaluator` 创建和操作 `TypeKind` 对象：

```cpp
class ConstEvaluator {
public:
  auto evaluateType(TypeNode *node) -> std::shared_ptr<TypeKind>;
  // 内部会创建相应的 TypeKind 对象
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

### 2. 类型创建

```cpp
// 创建路径类型
auto point_type = std::make_shared<TypeKindPath>(point_def);

// 创建数组类型
auto point_array_type = std::make_shared<TypeKindArray>(point_type, 5);

// 创建引用类型
auto point_ref_type = std::make_shared<TypeKindRef>(point_type, false);
```

### 3. 类型使用

```cpp
// 使用类型创建变量
auto var_info = std::make_shared<SymbolVariableInfo>("points", point_array_type, false);
scope->addVarible("points", var_info);
```

### 4. 类型检查

```cpp
// 检查类型一致性
if (expected_type->isEqual(actual_type.get())) {
    // 类型匹配
} else {
    // 类型不匹配
}
```

## 注意事项

1. **类型一致性**：确保 `TypeKind` 的类型与实际使用一致
2. **生命周期**：确保类型种类对象的生命周期长于使用它的对象
3. **内存管理**：正确使用智能指针，避免循环引用
4. **线程安全**：当前的 `TypeKind` 实现不是线程安全的
5. **递归类型**：处理递归类型时需要特殊考虑，避免无限递归

## 总结

`TypeKind` 类及其派生类构成了一个完整的类型种类表示体系，支持编译器中的各种类型检查和类型推导需求。通过多态设计和智能指针管理，提供了类型安全、内存安全和扩展性强的类型种类处理机制。与 `ConstInfo`、`SymbolInfo`、`TypeDef` 和 `ConstEvaluator` 等组件紧密协作，为编译器的语义分析阶段提供了强大的类型系统支持。