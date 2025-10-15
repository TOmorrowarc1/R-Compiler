# Symbol - 符号系统

## 简介

`Symbol` 类及其派生类构成了 AST 标注系统中的符号表示体系。这个多态类层次结构用于表示程序中的各种符号（变量、函数、类型、常量等），为编译器的符号表管理和语义分析提供了基础支持。

## 基类结构

```cpp
class SymbolInfo {
public:
  virtual ~SymbolInfo() = default;
};
```

### 特点

- **抽象基类**：`SymbolInfo` 是一个抽象基类，定义了所有符号的公共接口
- **多态支持**：通过虚析构函数支持多态
- **扩展性强**：易于添加新的符号类型

## 派生类体系

### 1. SymbolVariableInfo - 变量符号

```cpp
class SymbolVariableInfo : public SymbolInfo {
private:
  std::string name_;                           // 变量名称
  std::shared_ptr<TypeKind> type_;            // 变量类型
  bool is_mutable_;                           // 是否可变

public:
  SymbolVariableInfo(const std::string &name, std::shared_ptr<TypeKind> type,
                     bool is_mutable);
  ~SymbolVariableInfo();
  auto getName() const -> const std::string &;
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto isMutable() const -> bool;
};
```

**成员变量**：
- `name_`：变量名称
- `type_`：变量类型
- `is_mutable_`：是否可变（可修改）

**功能**：
- 表示程序中的变量符号
- 支持类型检查和可变性检查
- 用于变量声明和使用的语义分析

**使用示例**：
```cpp
// 创建整型变量符号
auto int_type = std::make_shared<TypeKindPath>(int_def);
auto var_info = std::make_shared<SymbolVariableInfo>("counter", int_type, true);

// 获取变量信息
auto name = var_info->getName();        // 返回 "counter"
auto type = var_info->getType();        // 返回整型类型
auto is_mutable = var_info->isMutable(); // 返回 true
```

### 2. SymbolFunctionInfo - 函数符号

```cpp
class SymbolFunctionInfo : public SymbolInfo {
public:
  enum class FnType {
    Normal,      // 普通函数
    Method,      // 方法（不可变）
    MutMethod,   // 可变方法
  };

private:
  std::string name_;                                    // 函数名称
  std::shared_ptr<TypeKind> return_type_;               // 返回类型
  std::vector<std::shared_ptr<TypeKind>> parameters_;   // 参数类型列表
  FnType fn_type_;                                      // 函数类型

public:
  SymbolFunctionInfo(const std::string &name,
                     std::shared_ptr<TypeKind> returnType,
                     std::vector<std::shared_ptr<TypeKind>> &&parameters);
  ~SymbolFunctionInfo();
  auto getName() const -> const std::string &;
  auto getReturnType() const -> std::shared_ptr<TypeKind>;
  auto getParametersType() const
      -> const std::vector<std::shared_ptr<TypeKind>> &;
  // 检查两个函数签名是否相同
  auto isEqual(const SymbolFunctionInfo *other) const -> bool;
  auto getFnType() const -> FnType;
  void setFnType(FnType type);
};
```

**成员变量**：
- `name_`：函数名称
- `return_type_`：返回类型
- `parameters_`：参数类型列表
- `fn_type_`：函数类型（普通函数、方法、可变方法）

**功能**：
- 表示程序中的函数符号
- 支持函数签名比较
- 区分普通函数和方法

**使用示例**：
```cpp
// 创建函数符号
std::vector<std::shared_ptr<TypeKind>> params;
params.push_back(int_type);
params.push_back(int_type);
auto func_info = std::make_shared<SymbolFunctionInfo>("add", int_type, std::move(params));

// 设置函数类型
func_info->setFnType(SymbolFunctionInfo::FnType::Normal);

// 获取函数信息
auto name = func_info->getName();           // 返回 "add"
auto ret_type = func_info->getReturnType();  // 返回整型类型
auto params = func_info->getParametersType(); // 返回参数类型列表
```

### 3. SymbolTypeInfo - 类型符号

```cpp
class SymbolTypeInfo : public SymbolInfo {
private:
  std::string name_;                  // 类型名称
  std::shared_ptr<TypeDef> type_;     // 类型定义

public:
  SymbolTypeInfo(const std::string &name, std::shared_ptr<TypeDef> type);
  ~SymbolTypeInfo();
  auto getName() const -> const std::string &;
  auto getType() const -> std::shared_ptr<TypeDef>;
};
```

**成员变量**：
- `name_`：类型名称
- `type_`：类型定义

**功能**：
- 表示程序中的类型符号
- 支持类型定义的引用
- 用于类型声明和使用的语义分析

**使用示例**：
```cpp
// 创建类型符号
auto struct_def = std::make_shared<StructDef>("Point");
auto type_info = std::make_shared<SymbolTypeInfo>("Point", struct_def);

// 获取类型信息
auto name = type_info->getName();   // 返回 "Point"
auto type_def = type_info->getType(); // 返回结构体定义
```

### 4. SymbolTraitInfo - 特征符号

```cpp
class SymbolTraitInfo : public SymbolInfo {
private:
  std::string name_;                  // 特征名称
  std::shared_ptr<TraitDef> trait_;   // 特征定义

public:
  SymbolTraitInfo(const std::string &name, std::shared_ptr<TraitDef> trait);
  ~SymbolTraitInfo();
  auto getName() const -> const std::string &;
  auto getTrait() const -> std::shared_ptr<TraitDef>;
};
```

**成员变量**：
- `name_`：特征名称
- `trait_`：特征定义

**功能**：
- 表示程序中的特征符号
- 支持特征定义的引用
- 用于特征声明和使用的语义分析

**使用示例**：
```cpp
// 创建特征符号
auto trait_def = std::make_shared<TraitDef>("Drawable");
auto trait_info = std::make_shared<SymbolTraitInfo>("Drawable", trait_def);

// 获取特征信息
auto name = trait_info->getName();   // 返回 "Drawable"
auto trait_def = trait_info->getTrait(); // 返回特征定义
```

### 5. SymbolConstInfo - 常量符号

```cpp
class SymbolConstInfo : public SymbolInfo {
private:
  std::string name_;                  // 常量名称
  std::shared_ptr<ConstInfo> const_info_; // 常量信息

public:
  SymbolConstInfo(const std::string &name);
  ~SymbolConstInfo();
  auto getName() const -> const std::string &;
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto getValue() const -> std::shared_ptr<ConstValue>;
  void setValue(std::shared_ptr<ConstInfo> &&const_info);
};
```

**成员变量**：
- `name_`：常量名称
- `const_info_`：常量信息

**功能**：
- 表示程序中的常量符号
- 支持常量类型和值的存储
- 用于常量声明和使用的语义分析

**使用示例**：
```cpp
// 创建常量符号
auto const_info = std::make_shared<SymbolConstInfo>("MAX_VALUE");

// 设置常量值
auto int_type = std::make_shared<TypeKindPath>(int_def);
auto int_value = std::make_shared<ConstValueInt>(100);
auto const_val = std::make_shared<ConstInfo>(int_type, int_value);
const_info->setValue(std::move(const_val));

// 获取常量信息
auto name = const_info->getName();   // 返回 "MAX_VALUE"
auto type = const_info->getType();   // 返回整型类型
auto value = const_info->getValue(); // 返回整数值 100
```

## 设计特点

### 1. 多态设计

使用继承和多态实现不同类型的符号：

- **基类接口统一**：所有符号都继承自 `SymbolInfo`
- **类型安全**：通过虚函数确保类型安全
- **扩展性强**：易于添加新的符号类型

### 2. 智能指针管理

使用 `std::shared_ptr` 管理符号对象：

- **内存安全**：自动管理内存，避免泄漏
- **共享所有权**：多个对象可以共享同一个符号
- **引用计数**：自动处理对象生命周期

### 3. 类型系统集成

符号系统与类型系统紧密集成：

- **类型引用**：符号可以引用类型定义
- **类型检查**：支持基于符号的类型检查
- **类型推导**：支持基于符号的类型推导

## 使用模式

### 1. 创建符号

```cpp
// 创建变量符号
auto var_info = std::make_shared<SymbolVariableInfo>("x", int_type, true);

// 创建函数符号
std::vector<std::shared_ptr<TypeKind>> params;
params.push_back(int_type);
auto func_info = std::make_shared<SymbolFunctionInfo>("foo", int_type, std::move(params));

// 创建类型符号
auto type_info = std::make_shared<SymbolTypeInfo>("MyType", type_def);

// 创建特征符号
auto trait_info = std::make_shared<SymbolTraitInfo>("MyTrait", trait_def);

// 创建常量符号
auto const_info = std::make_shared<SymbolConstInfo>("MY_CONST");
```

### 2. 类型检查和转换

```cpp
// 检查符号类型
if (auto var_info = std::dynamic_pointer_cast<SymbolVariableInfo>(symbol)) {
    // 处理变量符号
    auto is_mutable = var_info->isMutable();
} else if (auto func_info = std::dynamic_pointer_cast<SymbolFunctionInfo>(symbol)) {
    // 处理函数符号
    auto ret_type = func_info->getReturnType();
}
// ... 其他类型检查
```

### 3. 符号比较

```cpp
// 比较函数签名
auto func1 = std::make_shared<SymbolFunctionInfo>("foo", int_type, std::move(params1));
auto func2 = std::make_shared<SymbolFunctionInfo>("bar", int_type, std::move(params2));
if (func1->isEqual(func2.get())) {
    // 函数签名相同
}
```

## 与其他组件的关系

### 与 Scope 的关系

`Scope` 管理各种 `SymbolInfo` 派生类对象：

```cpp
class Scope {
private:
  std::unordered_map<std::string, std::shared_ptr<SymbolInfo>> symbols_;
  std::unordered_map<std::string, std::shared_ptr<SymbolTypeInfo>> types_;
  std::unordered_map<std::string, std::shared_ptr<SymbolTraitInfo>> traits_;
  std::unordered_map<std::string, std::shared_ptr<SymbolConstInfo>> consts_;
  
public:
  auto addVarible(const std::string &name,
                  std::shared_ptr<SymbolVariableInfo> symbol) -> bool;
  auto addFunction(const std::string &name,
                   std::shared_ptr<SymbolFunctionInfo> function) -> bool;
  // ...
};
```

### 与 TypeDef 的关系

`SymbolTypeInfo` 引用 `TypeDef` 对象：

```cpp
class SymbolTypeInfo {
private:
  std::shared_ptr<TypeDef> type_;  // 引用类型定义
  
public:
  auto getType() const -> std::shared_ptr<TypeDef>;
};
```

### 与 TraitDef 的关系

`SymbolTraitInfo` 引用 `TraitDef` 对象：

```cpp
class SymbolTraitInfo {
private:
  std::shared_ptr<TraitDef> trait_;  // 引用特征定义
  
public:
  auto getTrait() const -> std::shared_ptr<TraitDef>;
};
```

### 与 ConstInfo 的关系

`SymbolConstInfo` 包含 `ConstInfo` 对象：

```cpp
class SymbolConstInfo {
private:
  std::shared_ptr<ConstInfo> const_info_;  // 包含常量信息
  
public:
  auto getType() const -> std::shared_ptr<TypeKind>;
  auto getValue() const -> std::shared_ptr<ConstValue>;
};
```

## 注意事项

1. **名称唯一性**：在同一作用域中，符号名称必须唯一
2. **生命周期**：确保符号对象的生命周期长于使用它的对象
3. **内存管理**：正确使用智能指针，避免循环引用
4. **线程安全**：当前的符号系统实现不是线程安全的
5. **类型一致性**：确保符号的类型信息与实际使用一致

## 总结

`Symbol` 类及其派生类构成了一个完整的符号表示体系，支持编译器中的各种符号管理和语义分析需求。通过多态设计和智能指针管理，提供了类型安全、内存安全和扩展性强的符号处理机制。与 `Scope`、`TypeDef`、`TraitDef` 和 `ConstInfo` 等组件紧密协作，为编译器的语义分析阶段提供了坚实的基础。