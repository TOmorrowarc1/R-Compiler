# ConstValue - 常量值

## 简介

`ConstValue` 类及其派生类构成了 AST 标注系统中的常量值表示体系。这个多态类层次结构用于表示编译时已知的各种类型的常量值，为编译器的常量表达式求值和优化提供支持。

## 基类结构

```cpp
class ConstValue {
public:
  ConstValue() = default;
  virtual ~ConstValue() = default;
  virtual auto clone() const -> std::unique_ptr<ConstValue> = 0;
};
```

### 核心方法

#### clone

```cpp
virtual auto clone() const -> std::unique_ptr<ConstValue> = 0;
```

- **返回值**：`std::unique_ptr<ConstValue>`
- **功能**：创建当前对象的深拷贝
- **特点**：
  - 纯虚函数，必须在派生类中实现
  - 返回 `unique_ptr` 确保明确的内存所有权
  - 支持多态拷贝

## 派生类体系

### 1. ConstValueInt - 整型常量

```cpp
class ConstValueInt : public ConstValue {
private:
  int32_t value_;  // 32位整数值

public:
  ConstValueInt(int32_t value);
  ~ConstValueInt() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getValue() const -> int32_t;
};
```

**特点**：
- 存储 32 位整数值
- 支持基本的整数运算
- 用于表示整型字面量和常量表达式结果

**使用示例**：
```cpp
auto int_value = std::make_shared<ConstValueInt>(42);
auto cloned = int_value->clone();
auto value = int_value->getValue();  // 返回 42
```

### 2. ConstValueBool - 布尔常量

```cpp
class ConstValueBool : public ConstValue {
private:
  bool value_;  // 布尔值

public:
  ConstValueBool(bool value);
  ~ConstValueBool() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getValue() const -> bool;
};
```

**特点**：
- 存储布尔值（true/false）
- 支持逻辑运算
- 用于表示布尔字面量和条件表达式结果

**使用示例**：
```cpp
auto bool_value = std::make_shared<ConstValueBool>(true);
auto value = bool_value->getValue();  // 返回 true
```

### 3. ConstValueChar - 字符常量

```cpp
class ConstValueChar : public ConstValue {
private:
  char value_;  // 字符值

public:
  ConstValueChar(char value);
  ~ConstValueChar() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getValue() const -> char;
};
```

**特点**：
- 存储单个字符值
- 支持字符比较和转换
- 用于表示字符字面量

**使用示例**：
```cpp
auto char_value = std::make_shared<ConstValueChar>('A');
auto value = char_value->getValue();  // 返回 'A'
```

### 4. ConstValueString - 字符串常量

```cpp
class ConstValueString : public ConstValue {
private:
  std::string value_;  // 字符串值

public:
  ConstValueString(const std::string &value);
  ~ConstValueString() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getValue() const -> const std::string &;
};
```

**特点**：
- 存储字符串值
- 支持字符串操作
- 用于表示字符串字面量

**使用示例**：
```cpp
auto string_value = std::make_shared<ConstValueString>("Hello");
auto value = string_value->getValue();  // 返回 "Hello"
```

### 5. ConstValueStruct - 结构体常量

```cpp
class ConstValueStruct : public ConstValue {
private:
  std::unordered_map<std::string, std::shared_ptr<ConstValue>> fields_;  // 字段映射

public:
  ConstValueStruct(
      std::unordered_map<std::string, std::shared_ptr<ConstValue>> &&fields);
  ~ConstValueStruct() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getField(const std::string &name) const -> const std::shared_ptr<ConstValue>;
};
```

**特点**：
- 存储结构体的字段值
- 支持字段访问
- 用于表示结构体字面量和常量结构体

**使用示例**：
```cpp
std::unordered_map<std::string, std::shared_ptr<ConstValue>> fields;
fields["x"] = std::make_shared<ConstValueInt>(10);
fields["y"] = std::make_shared<ConstValueInt>(20);
auto struct_value = std::make_shared<ConstValueStruct>(std::move(fields));
auto x_field = struct_value->getField("x");  // 返回包含 10 的 ConstValueInt
```

### 6. ConstValueEnum - 枚举常量

```cpp
class ConstValueEnum: public ConstValue {
private:
  std::string variant_;  // 枚举变体名称

public:
  ConstValueEnum(const std::string &variant);
  ~ConstValueEnum() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getVariant() const -> const std::string &;
};
```

**特点**：
- 存储枚举变体名称
- 支持枚举比较
- 用于表示枚举值

**使用示例**：
```cpp
auto enum_value = std::make_shared<ConstValueEnum>("SomeVariant");
auto variant = enum_value->getVariant();  // 返回 "SomeVariant"
```

### 7. ConstValueArray - 数组常量

```cpp
class ConstValueArray : public ConstValue {
private:
  std::vector<std::shared_ptr<ConstValue>> elements_;  // 元素向量

public:
  ConstValueArray(std::vector<std::shared_ptr<ConstValue>> &&elements);
  ~ConstValueArray() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getElement(uint32_t index) const -> const std::shared_ptr<ConstValue>;
};
```

**特点**：
- 存储数组元素
- 支持索引访问
- 用于表示数组字面量

**使用示例**：
```cpp
std::vector<std::shared_ptr<ConstValue>> elements;
elements.push_back(std::make_shared<ConstValueInt>(1));
elements.push_back(std::make_shared<ConstValueInt>(2));
elements.push_back(std::make_shared<ConstValueInt>(3));
auto array_value = std::make_shared<ConstValueArray>(std::move(elements));
auto element = array_value->getElement(0);  // 返回包含 1 的 ConstValueInt
```

## 设计特点

### 1. 多态设计

使用继承和多态实现不同类型的常量值：

- **基类接口统一**：所有常量值都继承自 `ConstValue`
- **类型安全**：通过虚函数确保类型安全
- **扩展性强**：易于添加新的常量类型

### 2. 智能指针管理

使用 `std::shared_ptr` 管理常量值对象：

- **内存安全**：自动管理内存，避免泄漏
- **共享所有权**：多个对象可以共享同一个常量值
- **引用计数**：自动处理对象生命周期

### 3. 深拷贝支持

通过 `clone()` 方法实现深拷贝：

- **独立性**：拷贝后的对象与原对象独立
- **多态拷贝**：支持派生类的正确拷贝
- **内存安全**：使用 `unique_ptr` 明确所有权

## 使用模式

### 1. 创建常量值

```cpp
// 基本类型
auto int_val = std::make_shared<ConstValueInt>(42);
auto bool_val = std::make_shared<ConstValueBool>(true);
auto char_val = std::make_shared<ConstValueChar>('A');
auto string_val = std::make_shared<ConstValueString>("Hello");

// 复合类型
std::unordered_map<std::string, std::shared_ptr<ConstValue>> fields;
fields["x"] = std::make_shared<ConstValueInt>(10);
fields["y"] = std::make_shared<ConstValueInt>(20);
auto struct_val = std::make_shared<ConstValueStruct>(std::move(fields));

std::vector<std::shared_ptr<ConstValue>> elements;
elements.push_back(std::make_shared<ConstValueInt>(1));
elements.push_back(std::make_shared<ConstValueInt>(2));
auto array_val = std::make_shared<ConstValueArray>(std::move(elements));

// 枚举类型
auto enum_val = std::make_shared<ConstValueEnum>("SomeVariant");
```

### 2. 类型检查和转换

```cpp
// 检查常量值类型
if (auto int_val = std::dynamic_pointer_cast<ConstValueInt>(const_value)) {
    // 处理整型常量
    int value = int_val->getValue();
} else if (auto bool_val = std::dynamic_pointer_cast<ConstValueBool>(const_value)) {
    // 处理布尔常量
    bool value = bool_val->getValue();
}
// ... 其他类型检查
```

### 3. 常量值操作

```cpp
// 拷贝常量值
auto cloned = const_value->clone();

// 访问复合类型的组成部分
if (auto struct_val = std::dynamic_pointer_cast<ConstValueStruct>(const_value)) {
    auto field = struct_val->getField("fieldName");
}

if (auto array_val = std::dynamic_pointer_cast<ConstValueArray>(const_value)) {
    auto element = array_val->getElement(0);
}
```

## 与其他组件的关系

### 与 ConstInfo 的关系

`ConstValue` 是 `ConstInfo` 的一部分：

```cpp
class ConstInfo {
private:
  std::shared_ptr<TypeKind> type_;
  std::shared_ptr<ConstValue> const_value_;  // 包含 ConstValue
  
public:
  auto getConstValue() const -> std::shared_ptr<ConstValue>;
};
```

### 与 ConstEvaluator 的关系

`ConstEvaluator` 创建和操作 `ConstValue` 对象：

```cpp
class ConstEvaluator {
public:
  auto evaluateExprValue(ExprNode *node) -> std::shared_ptr<ConstInfo>;
  // 内部会创建相应的 ConstValue 对象
};
```

### 与 TypeKind 的关系

`ConstValue` 的类型由 `TypeKind` 描述：

```cpp
// ConstValue 和 TypeKind 的对应关系
ConstValueInt     <-> TypeKindPath (int type)
ConstValueBool    <-> TypeKindPath (bool type)
ConstValueChar    <-> TypeKindPath (char type)
ConstValueString  <-> TypeKindPath (string type)
ConstValueStruct  <-> TypeKindPath (struct type)
ConstValueEnum    <-> TypeKindPath (enum type)
ConstValueArray   <-> TypeKindArray
```

## 注意事项

1. **类型一致性**：确保 `ConstValue` 的类型与对应的 `TypeKind` 匹配
2. **内存管理**：正确使用智能指针，避免循环引用
3. **线程安全**：当前的 `ConstValue` 实现不是线程安全的
4. **性能考虑**：频繁的拷贝操作可能影响性能，应尽量使用引用

## 总结

`ConstValue` 类及其派生类构成了一个完整的常量值表示体系，支持编译器中的各种常量表达式求值和优化需求。通过多态设计和智能指针管理，提供了类型安全、内存安全和扩展性强的常量值处理机制。与 `ConstInfo`、`ConstEvaluator` 和 `TypeKind` 等组件紧密协作，为编译器的语义分析阶段提供了坚实的基础。