# ConstInfo - 常量信息

## 简介

`ConstInfo` 类是 AST 标注系统中用于存储常量类型和值信息的核心类。它封装了常量的类型信息和具体的常量值，为编译器的常量表达式求值和类型检查提供支持。

## 类结构

```cpp
class ConstInfo {
private:
  std::shared_ptr<TypeKind> type_;        // 常量的类型信息
  std::shared_ptr<ConstValue> const_value_; // 常量的值信息

public:
  // 两个构造函数：一个用于表达式，一个用于常量定义
  ConstInfo(std::shared_ptr<TypeKind> type);
  ConstInfo(std::shared_ptr<TypeKind> type,
            std::shared_ptr<ConstValue> const_value);
  ~ConstInfo();
  
  // 设置常量值
  auto setConstValue(const ConstInfo *rhs) -> bool;
  
  // 获取类型和值
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto getConstValue() const -> std::shared_ptr<ConstValue>;
};
```

## 成员变量

### type_

```cpp
std::shared_ptr<TypeKind> type_;
```

- **类型**：`std::shared_ptr<TypeKind>`
- **描述**：存储常量的类型信息
- **作用**：用于类型检查和类型推导
- **特点**：使用智能指针管理，确保内存安全

### const_value_

```cpp
std::shared_ptr<ConstValue> const_value_;
```

- **类型**：`std::shared_ptr<ConstValue>`
- **描述**：存储常量的具体值
- **作用**：保存常量表达式的计算结果
- **特点**：使用智能指针管理，支持多态

## 构造函数

### 1. 表达式构造函数

```cpp
ConstInfo(std::shared_ptr<TypeKind> type);
```

- **参数**：
  - `type`：常量的类型信息
- **用途**：用于创建只有类型信息而没有具体值的常量信息对象
- **适用场景**：
  - 类型推导阶段
  - 常量声明但未初始化的情况
  - 需要延迟求值的表达式

### 2. 常量定义构造函数

```cpp
ConstInfo(std::shared_ptr<TypeKind> type,
          std::shared_ptr<ConstValue> const_value);
```

- **参数**：
  - `type`：常量的类型信息
  - `const_value`：常量的具体值
- **用途**：用于创建包含完整类型和值信息的常量信息对象
- **适用场景**：
  - 常量表达式求值完成
  - 字面量常量
  - 已知的编译时常量

## 成员函数

### setConstValue

```cpp
auto setConstValue(const ConstInfo *rhs) -> bool;
```

- **参数**：
  - `rhs`：源常量信息对象的指针
- **返回值**：
  - `bool`：设置是否成功
- **功能**：
  - 从另一个 `ConstInfo` 对象复制常量值
  - 进行类型兼容性检查
  - 更新当前对象的常量值
- **返回值说明**：
  - `true`：设置成功
  - `false`：设置失败（通常是类型不兼容）

### getType

```cpp
auto getType() const -> std::shared_ptr<TypeKind>;
```

- **参数**：无
- **返回值**：
  - `std::shared_ptr<TypeKind>`：常量的类型信息
- **功能**：
  - 获取常量的类型信息
  - 返回智能指针，支持共享所有权

### getConstValue

```cpp
auto getConstValue() const -> std::shared_ptr<ConstValue>;
```

- **参数**：无
- **返回值**：
  - `std::shared_ptr<ConstValue>`：常量的值信息
- **功能**：
  - 获取常量的具体值
  - 返回智能指针，支持多态和共享所有权

## 使用示例

### 1. 创建基本类型常量

```cpp
// 创建类型信息
auto int_type = std::make_shared<TypeKindPath>(...);

// 创建整数值
auto int_value = std::make_shared<ConstValueInt>(42);

// 创建完整的常量信息
auto const_info = std::make_shared<ConstInfo>(int_type, int_value);

// 获取类型和值
auto type = const_info->getType();
auto value = const_info->getConstValue();
```

### 2. 创建只有类型的常量信息

```cpp
// 创建类型信息
auto bool_type = std::make_shared<TypeKindPath>(...);

// 创建只有类型的常量信息（用于延迟求值）
auto const_info = std::make_shared<ConstInfo>(bool_type);

// 后续设置值
auto bool_value = std::make_shared<ConstValueBool>(true);
auto other_info = std::make_shared<ConstInfo>(bool_type, bool_value);
const_info->setConstValue(other_info.get());
```

### 3. 复杂类型常量

```cpp
// 创建结构体类型
auto struct_type = std::make_shared<TypeKindPath>(struct_def);

// 创建结构体值
std::unordered_map<std::string, std::shared_ptr<ConstValue>> fields;
fields["x"] = std::make_shared<ConstValueInt>(10);
fields["y"] = std::make_shared<ConstValueInt>(20);
auto struct_value = std::make_shared<ConstValueStruct>(std::move(fields));

// 创建结构体常量信息
auto const_info = std::make_shared<ConstInfo>(struct_type, struct_value);
```

## 设计特点

### 1. 分离设计

`ConstInfo` 将类型信息和值信息分离存储，这种设计带来以下优势：

- **灵活性**：可以单独处理类型和值
- **延迟求值**：支持先确定类型，后计算值
- **类型安全**：确保类型和值的匹配

### 2. 智能指针管理

使用 `std::shared_ptr` 管理类型和值信息：

- **内存安全**：自动管理内存，避免泄漏
- **共享所有权**：多个对象可以共享同一个类型或值
- **多态支持**：支持 `TypeKind` 和 `ConstValue` 的多态

### 3. 类型一致性

通过 `setConstValue` 方法确保类型一致性：

- **类型检查**：设置值时检查类型兼容性
- **错误预防**：防止类型不匹配的赋值
- **安全保证**：维护类型系统的完整性

## 与其他组件的关系

### 与 ConstEvaluator 的关系

`ConstInfo` 是 `ConstEvaluator` 求值的结果：

```cpp
// ConstEvaluator 使用 ConstInfo 存储求值结果
auto const_info = evaluator.evaluateExprValue(expr_node);
```

### 与 ConstValue 的关系

`ConstInfo` 包含 `ConstValue`，表示具体的常量值：

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

`ConstInfo` 包含 `TypeKind`，表示常量的类型：

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

## 注意事项

1. **类型一致性**：确保 `type_` 和 `const_value_` 的类型匹配
2. **空指针检查**：使用 `getConstValue()` 前检查是否为空
3. **内存管理**：正确使用智能指针，避免循环引用
4. **线程安全**：当前的 `ConstInfo` 实现不是线程安全的

## 总结

`ConstInfo` 类是 AST 标注系统中用于存储常量信息的重要组件。它通过分离类型和值信息，提供了灵活的常量表示方式，支持延迟求值和类型检查。与 `ConstEvaluator`、`ConstValue` 和 `TypeKind` 等组件紧密协作，为编译器的常量表达式处理和类型系统提供了坚实的基础。