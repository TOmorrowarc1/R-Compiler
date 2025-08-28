#include "ScopeBuiltInInit.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"

const std::vector<std::shared_ptr<TypeDef>> BUILTIN = {
    std::make_shared<TypeDef>("i32"),   std::make_shared<TypeDef>("u32"),
    std::make_shared<TypeDef>("isize"), std::make_shared<TypeDef>("usize"),
    std::make_shared<TypeDef>("bool"),  std::make_shared<TypeDef>("char"),
    std::make_shared<TypeDef>("str"),   std::make_shared<TypeDef>("unit"),
};

auto getTypeKind(Scope *global_scope, const std::string &type_name)
    -> std::shared_ptr<TypeKind> {
  auto type_def = global_scope->getType(type_name)->getType();
  if (type_def) {
    return std::make_shared<TypeKindPath>(type_def);
  }
  return nullptr;
}

void scopeBuildInInit(Scope *global_scope) {
  // Add built-in types.
  for (const auto &type : BUILTIN) {
    auto type_symbol = std::make_shared<SymbolTypeInfo>(type->getName(), type);
    global_scope->addType(type->getName(), type_symbol);
  }

  // Add built-in functions.
  {
    // fn print(s: str) -> unit
    std::vector<std::shared_ptr<TypeKind>> params;
    auto return_type = getTypeKind(global_scope, "unit");
    params.push_back(getTypeKind(global_scope, "str"));
    auto func = std::make_shared<SymbolFunctionInfo>("print", return_type,
                                                     std::move(params));
    global_scope->addFunction("print", func);
  }
  {
    // fn println(s: str) -> unit
    std::vector<std::shared_ptr<TypeKind>> params;
    auto return_type = getTypeKind(global_scope, "unit");
    params.push_back(getTypeKind(global_scope, "str"));
    auto func = std::make_shared<SymbolFunctionInfo>("println", return_type,
                                                     std::move(params));
    global_scope->addFunction("println", func);
  }
  {
    // fn printInt(n: i32) -> unit
    std::vector<std::shared_ptr<TypeKind>> params;
    auto return_type = getTypeKind(global_scope, "unit");
    params.push_back(getTypeKind(global_scope, "i32"));
    auto func = std::make_shared<SymbolFunctionInfo>("printInt", return_type,
                                                     std::move(params));
    global_scope->addFunction("printInt", func);
  }
  {
    // fn printlnInt(n: i32) -> unit
    std::vector<std::shared_ptr<TypeKind>> params;
    auto return_type = getTypeKind(global_scope, "unit");
    params.push_back(getTypeKind(global_scope, "i32"));
    auto func = std::make_shared<SymbolFunctionInfo>("printlnInt", return_type,
                                                     std::move(params));
    global_scope->addFunction("printlnInt", func);
  }
  {
    // fn getString() -> str
    std::vector<std::shared_ptr<TypeKind>> params;
    auto return_type = getTypeKind(global_scope, "str");
    auto func = std::make_shared<SymbolFunctionInfo>("getString", return_type,
                                                     std::move(params));
    global_scope->addFunction("getString", func);
  }
  {
    // fn exit(code: i32) -> unit
    std::vector<std::shared_ptr<TypeKind>> params;
    auto return_type = getTypeKind(global_scope, "unit");
    params.push_back(getTypeKind(global_scope, "i32"));
    auto func = std::make_shared<SymbolFunctionInfo>("exit", return_type,
                                                     std::move(params));
    global_scope->addFunction("exit", func);
  }

  // Add built-in methods.
  {
    // impl u32
    auto type_def = global_scope->getType("u32")->getType();
    // fn toString(&self) -> str
    std::vector<std::shared_ptr<TypeKind>> params;
    auto return_type = getTypeKind(global_scope, "str");
    auto func = std::make_shared<SymbolFunctionInfo>("to_string", return_type,
                                                     std::move(params));
    func->setFnType(SymbolFunctionInfo::FnType::Method);
    type_def->addMethod("to_string", func);
  }
  {
    // impl str
    auto type_def = global_scope->getType("str")->getType();
    // fn len(&self) -> i32
    std::vector<std::shared_ptr<TypeKind>> params;
    auto return_type = getTypeKind(global_scope, "u32");
    auto func = std::make_shared<SymbolFunctionInfo>("len", return_type,
                                                     std::move(params));
    func->setFnType(SymbolFunctionInfo::FnType::Method);
    type_def->addMethod("len", func);
    // for reference.
    // fn as_str (&self) -> &str
  }
}