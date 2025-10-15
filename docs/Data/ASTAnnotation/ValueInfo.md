# ValueInfo - 值信息

## 简介

`ValueInfo` 类是 AST 标注系统中用于存储表达式值类型和值信息的核心类。它封装了表达式的类型信息和可能的值信息，为编译器的表达式求值和类型检查提供支持。与 `ConstInfo` 不同，`ValueInfo` 可以表示运行时值，而不仅仅是编译时常量。

## 类结构

```cpp
class ValueInfo {
private:
  std::shared_ptr<TypeKind> type_;        // 值的类型信息
  std::shared_ptr<ConstValue> value_;      // 值信息（可能为空）

public:
  // 构造函数
  ValueInfo(std::shared_ptr<TypeKind> type);
  ValueInfo(std::shared_ptr<TypeKind> type,
            std::shared_ptr<ConstValue> value);
  ~ValueInfo();
  
  // 设置值
  auto setValue(std::shared_ptr<ConstValue> value) -> void;
  
  // 获取类型和值
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto getValue() const -> std::shared_ptr<ConstValue>;
  auto hasValue() const -> bool;
};
```

## 成员变量

### type_

```cpp
std::shared_ptr<TypeKind> type_;
```

- **类型**：`std::shared_ptr<TypeKind>`
- **描述**：存储值的类型信息
- **作用**：用于类型检查和类型推导
- **特点**：
  - 使用智能指针管理，确保内存安全
  - 必须始终有效，不能为空

### value_

```cpp
std::shared_ptr<ConstValue> value_;
```

- **类型**：`std::shared_ptr<ConstValue>`
- **描述**：存储值的具体信息
- **作用**：保存表达式的计算结果
- **特点**：
  - 使用智能指针管理，支持多态
  - 可能为空，表示运行时值或未知值

## 构造函数

### 1. 只有类型的构造函数

```cpp
ValueInfo(std::shared_ptr<TypeKind> type);
```

- **参数**：
  - `type`：值的类型信息
- **用途**：用于创建只有类型信息而没有具体值的值信息对象
- **适用场景**：
  - 类型推导阶段
  - 运行时表达式
  - 值未知的表达式

### 2. 带有值的构造函数

```cpp
ValueInfo(std::shared_ptr<TypeKind> type,
          std::shared_ptr<ConstValue> value);
```

- **参数**：
  - `type`：值的类型信息
  - `value`：值的具体信息
- **用途**：用于创建包含完整类型和值信息的值信息对象
- **适用场景**：
  - 常量表达式求值完成
  - 字面量表达式
  - 已知的编译时值

## 成员函数

### setValue

```cpp
auto setValue(std::shared_ptr<ConstValue> value) -> void;
```

- **参数**：
  - `value`：要设置的值
- **功能**：
  - 设置值信息
  - 替换现有的值（如果有）
- **特点**：
  - 不进行类型检查，调用者需确保类型一致性
  - 支持将无值对象设置为有值对象

### getType

```cpp
auto getType() const -> std::shared_ptr<TypeKind>;
```

- **参数**：无
- **返回值**：
  - `std::shared_ptr<TypeKind>`：值的类型信息
- **功能**：
  - 获取值的类型信息
  - 返回智能指针，支持共享所有权

### getValue

```cpp
auto getValue() const -> std::shared_ptr<ConstValue>;
```

- **参数**：无
- **返回值**：
  - `std::shared_ptr<ConstValue>`：值的具体信息
- **功能**：
  - 获取值的具体信息
  - 返回智能指针，支持多态和共享所有权
- **特点**：
  - 如果没有值信息，返回空指针

### hasValue

```cpp
auto hasValue() const -> bool;
```

- **参数**：无
- **返回值**：
  - `bool`：是否有值信息
- **功能**：
  - 检查是否有值信息
- **返回值说明**：
  - `true`：有值信息
  - `false`：没有值信息

## 使用示例

### 1. 创建基本类型值信息

```cpp
// 创建类型信息
auto int_type = std::make_shared<TypeKindPath>(int_def);

// 创建只有类型的值信息（运行时表达式）
auto value_info = std::make_shared<ValueInfo>(int_type);

// 创建带有值的值信息（常量表达式）
auto int_value = std::make_shared<ConstValueInt>(42);
auto const_value_info = std::make_shared<ValueInfo>(int_type, int_value);

// 获取类型和值
auto type = value_info->getType();
auto value = const_value_info->getValue();
auto has_value = value_info->hasValue();  // 返回 false
```

### 2. 创建复合类型值信息

```cpp
// 创建结构体类型
auto struct_type = std::make_shared<TypeKindPath>(struct_def);

// 创建结构体值
std::unordered_map<std::string, std::shared_ptr<ConstValue>> fields;
fields["x"] = std::make_shared<ConstValueInt>(10);
fields["y"] = std::make_shared<ConstValueInt>(20);
auto struct_value = std::make_shared<ConstValueStruct>(std::move(fields));

// 创建结构体值信息
auto struct_value_info = std::make_shared<ValueInfo>(struct_type, struct_value);
```

### 3. 动态设置值

```cpp
// 创建只有类型的值信息
auto bool_type = std::make_shared<TypeKindPath>(bool_def);
auto value_info = std::make_shared<ValueInfo>(bool_type);

// 后续设置值
auto bool_value = std::make_shared<ConstValueBool>(true);
value_info->setValue(bool_value);

// 检查是否有值
if (value_info->hasValue()) {
    auto value = value_info->getValue();
}
```

## 设计特点

### 1. 分离设计

`ValueInfo` 将类型信息和值信息分离存储，这种设计带来以下优势：

- **灵活性**：可以单独处理类型和值
- **延迟求值**：支持先确定类型，后计算值
- **运行时支持**：可以表示运行时值（无值信息）

### 2. 智能指针管理

使用 `std::shared_ptr` 管理类型和值信息：

- **内存安全**：自动管理内存，避免泄漏
- **共享所有权**：多个对象可以共享同一个类型或值
- **多态支持**：支持 `TypeKind` 和 `ConstValue` 的多态

### 3. 可选值支持

通过 `value_` 可能为空的设计，支持运行时值：

- **编译时常量**：有值信息
- **运行时表达式**：无值信息
- **部分求值**：支持部分求值的结果

## 与其他组件的关系

### 与 ConstInfo 的关系

`ValueInfo` 与 `ConstInfo` 类似，但有以下区别：

```cpp
// ConstInfo - 仅用于编译时常量
class ConstInfo {
private:
  std::shared_ptr<TypeKind> type_;
  std::shared_ptr<ConstValue> const_value_;  // 必须有值
  
public:
  auto getConstValue() const -> std::shared_ptr<ConstValue>;
};

// ValueInfo - 可用于编译时常量和运行时值
class ValueInfo {
private:
  std::shared_ptr<TypeKind> type_;
  std::shared_ptr<ConstValue> value_;  // 可能为空
  
public:
  auto getValue() const -> std::shared_ptr<ConstValue>;
  auto hasValue() const -> bool;
};
```

### 与 ConstValue 的关系

`ValueInfo` 包含 `ConstValue`，表示具体的值：

```cpp
// ConstValue 的多态类型
class ConstValue {
  virtual auto clone() const -> std::unique_ptr<ConstValue> = 0;
};

// 具体的常量值类型
class ConstValueInt : public ConstValue { /* ... */ };
class ConstValueBool : public ConstValue { /* ... */ };
// ...
```

### 与 TypeKind 的关系

`ValueInfo` 包含 `TypeKind`，表示值的类型：

```cpp
// TypeKind 的多态类型
class TypeKind {
  virtual auto isEqual(const TypeKind *other) const -> bool = 0;
};

// 具体的类型种类
class TypeKindPath : public TypeKind { /* ... */ };
class TypeKindArray : public TypeKind { /* ... */ };
// ...
```

## 使用场景

### 1. 编译时常量表达式

```cpp
// 字面量表达式
auto int_value = std::make_shared<ConstValueInt>(42);
auto int_type = std::make_shared<TypeKindPath>(int_def);
auto value_info = std::make_shared<ValueInfo>(int_type, int_value);

// 常量表达式求值
auto result = evaluator.evaluateExprValue(expr_node);
// result 是 ValueInfo 对象，包含类型和值
```

### 2. 运行时表达式

```cpp
// 变量引用
auto var_type = std::make_shared<TypeKindPath>(int_def);
auto value_info = std::make_shared<ValueInfo>(var_type);
// value_info 没有值信息，表示运行时值

// 函数调用
auto ret_type = std::make_shared<TypeKindPath>(int_def);
auto call_value_info = std::make_shared<ValueInfo>(ret_type);
// call_value_info 没有值信息，表示运行时值
```

### 3. 类型推导

```cpp
// 类型推导阶段
auto inferred_type = type_inferencer.infer(expr_node);
auto value_info = std::make_shared<ValueInfo>(inferred_type);
// 先确定类型，后可能计算值

// 后续求值
if (expr_node->isConstExpr()) {
    auto value = evaluator.evaluateExprValue(expr_node);
    value_info->setValue(value->getValue());
}
```

## 注意事项

1. **类型一致性**：确保 `type_` 和 `value_` 的类型匹配（如果有值）
2. **空指针检查**：使用 `getValue()` 前检查是否有值
3. **内存管理**：正确使用智能指针，避免循环引用
4. **线程安全**：当前的 `ValueInfo` 实现不是线程安全的
5. **与 ConstInfo 的区别**：`ValueInfo` 可以表示运行时值，而 `ConstInfo` 仅表示编译时常量

## 总结

`ValueInfo` 类是 AST 标注系统中用于存储表达式值信息的重要组件。它通过分离类型和值信息，并支持可选值，提供了灵活的值表示方式，支持编译时常量和运行时值。与 `ConstInfo`、`ConstValue` 和 `TypeKind` 等组件紧密协作，为编译器的表达式处理和类型系统提供了坚实的基础。