#include "ParserTotal.hpp"

auto parseItemNode(TokenStream &stream) -> std::unique_ptr<ItemNode> {
  switch (stream.peek().type) {
  case TokenType::MOD:
    return parseItemModuleNode(stream);
  case TokenType::FN:
    return parseItemFnNode(stream);
  case TokenType::CONST:
    if (stream.peekNum(1).type == TokenType::FN) {
      return parseItemFnNode(stream);
    }
    return parseItemConstNode(stream);
  case TokenType::STRUCT:
    return parseItemStructNode(stream);
  case TokenType::ENUM:
    return parseItemEnumNode(stream);
  case TokenType::IMPL:
    return parseItemImplNode(stream);
  default:
    throw std::runtime_error("Unexpected token type for ItemNode");
  }
  return nullptr;
}

auto parseItemModuleNode(TokenStream &stream)
    -> std::unique_ptr<ItemModuleNode> {
  stream.next();
  std::vector<std::unique_ptr<ItemNode>> items;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("The module needs a name.");
  }
  std::string name = stream.next().content;
  if (stream.peek().type != TokenType::LEFT_BRACE) {
    throw std::runtime_error("The module should start with a {.");
  }
  stream.next();
  while (stream.peek().type != TokenType::RIGHT_BRACE) {
    items.push_back(parseItemNode(stream));
  }
  stream.next();
  return std::make_unique<ItemModuleNode>(name, std::move(items));
}

auto parseItemFnNode(TokenStream &stream) -> std::unique_ptr<ItemFnNode> {
  std::string ID;
  std::vector<ItemFnPara> parameters;
  std::unique_ptr<TypeNode> returnType;
  std::unique_ptr<ExprBlockNode> body;
  bool const_flag = false;
  if (stream.peek().type == TokenType::CONST) {
    const_flag = true;
    stream.next();
  }
  stream.next();
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Expected identifier after 'fn'");
  }
  ID = stream.next().content;
  if (stream.peek().type != TokenType::LEFT_PAREN) {
    throw std::runtime_error("Expected '(' after function name");
  }
  stream.next();
  parameters = parseFnParameters(stream);
  if (stream.peek().type != TokenType::RIGHT_PAREN) {
    throw std::runtime_error("Expected ')' after function parameters");
  }
  stream.next();
  if (stream.peek().type == TokenType::ARROW) {
    stream.next();
    returnType = parseTypeNode(stream);
  } else {
    returnType = nullptr;
  }
  if (stream.peek().type == TokenType::LEFT_BRACE) {
    stream.next();
    body = parseExprBlockNode(stream);
  } else if (stream.peek().type == TokenType::SEMICOLON) {
    stream.next();
  } else {
    throw std::runtime_error("Expected '{' after function signature");
  }
  return std::make_unique<ItemFnNode>(ID, std::move(returnType),
                                      std::move(parameters), std::move(body));
}

auto parseFnParameters(TokenStream &stream) -> std::vector<ItemFnPara> {
  std::vector<ItemFnPara> parameters;
  if (stream.peek().type != TokenType::RIGHT_PAREN) {
    parameters.push_back(parseItemFnPara(stream));
    while (stream.peek().type == TokenType::COMMA) {
      stream.next();
      if (stream.peek().type == TokenType::RIGHT_PAREN) {
        break;
      }
      parameters.push_back(parseItemFnPara(stream));
    }
  }
  if (stream.peek().type != TokenType::RIGHT_PAREN) {
    throw std::runtime_error("Unexpected token in function parameters");
  }
  stream.next();
  return parameters;
}

auto parseItemFnPara(TokenStream &stream) -> ItemFnPara {
  ItemFnPara param;
  int32_t checkpoint = stream.getIndex();
  try {
    param.pattern = parsePatternOneNode(stream);
  } catch (std::runtime_error) {
    param.pattern = nullptr;
    stream.restoreIndex(checkpoint);
  }
  if (stream.peek().type != TokenType::COLON) {
    param.pattern = nullptr;
    stream.restoreIndex(checkpoint);
  } else {
    stream.next();
  }
  param.type = parseTypeNode(stream);
  return param;
}

auto parseItemConstNode(TokenStream &stream) -> std::unique_ptr<ItemConstNode> {
  stream.next();
  std::string name;
  std::unique_ptr<TypeNode> type;
  std::unique_ptr<ExprBlockNode> value;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Expected identifier after 'const'");
  }
  name = stream.next().content;
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    type = parseTypeNode(stream);
  } else {
    throw std::runtime_error("Expected ':' after Const name");
  }
  if (stream.peek().type != TokenType::EQUAL) {
    throw std::runtime_error("Expected '=' after Const type");
  }
  stream.next();
  value = parseExprBlockNode(stream);
  return std::make_unique<ItemConstNode>(name, std::move(type),
                                         std::move(value));
}

auto parseItemStructNode(TokenStream &stream)
    -> std::unique_ptr<ItemStructNode> {
  stream.next();
  std::string ID;
  std::vector<ItemStructField> fields;
  if (stream.peek().type == TokenType::IDENTIFIER) {
    ID = stream.next().content;
  } else {
    throw std::runtime_error("Struct need identifier.");
  }
  if (stream.peek().type == TokenType::SEMICOLON) {
    stream.next();
  } else if (stream.peek().type == TokenType::LEFT_BRACE) {
    stream.next();
    if (stream.peek().type != TokenType::RIGHT_BRACE) {
      fields.push_back(parseItemStructField(stream));
      while (stream.peek().type == TokenType::COMMA) {
        stream.next();
        if (stream.peek().type == TokenType::RIGHT_PAREN) {
          break;
        }
        fields.push_back(parseItemStructField(stream));
      }
    }
    if (stream.peek().type != TokenType::RIGHT_PAREN) {
      throw std::runtime_error("Unexpected token in function parameters");
    }
    stream.next();
  } else {
    throw std::runtime_error("Unexcepted token in struct.");
  }
  return std::make_unique<ItemStructNode>(ID, std::move(fields));
}

auto parseItemStructField(TokenStream &stream) -> ItemStructField {
  ItemStructField field;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Every struct field needs an ID.");
  }
  field.identifier = stream.next().content;
  if (stream.next().type != TokenType::COLON) {
    throw std::runtime_error("Every struct field needs an :.");
  }
  field.type = parseTypeNode(stream);
  return field;
}

auto parseItemEnumNode(TokenStream &stream) -> std::unique_ptr<ItemEnumNode> {
  stream.next();
  std::string ID;
  std::vector<ItemEnumVariant> variants;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Every enum needs a ID.");
  }
  ID = stream.next().content;
  if (stream.peek().type == TokenType::LEFT_BRACE) {
    stream.next();
    if (stream.peek().type != TokenType::RIGHT_BRACE) {
      variants.push_back(parseItemEnumVariant(stream));
      while (stream.peek().type == TokenType::COMMA) {
        stream.next();
        if (stream.peek().type == TokenType::RIGHT_PAREN) {
          break;
        }
        variants.push_back(parseItemEnumVariant(stream));
      }
    }
    if (stream.peek().type != TokenType::RIGHT_PAREN) {
      throw std::runtime_error("Unexpected token in function parameters");
    }
    stream.next();
  } else {
    throw std::runtime_error("Unexcepted token in struct.");
  }
  return std::make_unique<ItemEnumNode>(ID, std::move(variants));
}

auto parseItemEnumVariant(TokenStream &stream) -> ItemEnumVariant {
  ItemEnumVariant variant;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Enum variant need a ID.");
  }
  variant.ID = stream.next().content;
  if (stream.peek().type == TokenType::LEFT_PAREN) {
    stream.next();
    while (stream.peek().type == TokenType::COMMA) {
      stream.next();
      if (stream.peek().type == TokenType::RIGHT_PAREN) {
        break;
      }
      variant.tuple_fields.push_back(parseItemTupleField(stream));
    }
    if (stream.peek().type != TokenType::RIGHT_PAREN) {
      throw std::runtime_error("Parents in a tuplevar should be in pair.");
    }
    stream.next();
  } else if (stream.peek().type == TokenType::LEFT_BRACE) {
    stream.next();
    while (stream.peek().type == TokenType::COMMA) {
      stream.next();
      if (stream.peek().type == TokenType::RIGHT_BRACE) {
        break;
      }
      variant.struct_fields.push_back(parseItemStructField(stream));
    }
    if (stream.peek().type != TokenType::RIGHT_BRACE) {
      throw std::runtime_error("Braces in a tuplevar should be in pair.");
    }
    stream.next();
  }
  if (stream.peek().type == TokenType::ASSIGN) {
    stream.next();
    variant.discriminant = parseExprNode(stream);
  }
  return variant;
}

auto parseItemTupleField(TokenStream &stream) -> ItemTupleField {
  return {parseTypeNode(stream)};
}

auto parseItemImplNode(TokenStream &stream) -> std::unique_ptr<ItemImplNode> {
  std::unique_ptr<TypeNode> type;
  std::vector<ItemAssociatedItem> items;
  stream.next();
  type = parseTypeNode(stream);
  if (stream.peek().type != TokenType::LEFT_BRACE) {
    throw std::runtime_error("Impl needs { after the type.");
  }
  stream.next();
  while (stream.peek().type != TokenType::RIGHT_BRACE) {
    items.push_back(parseItemAssociatedItem(stream));
  }
  return std::make_unique<ItemImplNode>(std::move(type), std::move(items));
}

auto parseItemAssociatedItem(TokenStream &stream) -> ItemAssociatedItem {
  bool is_fn = false;
  is_fn = stream.peek().type == TokenType::FN ||
          (stream.peek().type == TokenType::CONST &&
           stream.peekNum(1).type == TokenType::FN);
  ItemAssociatedItem item;
  if (is_fn) {
    item.function = parseItemFnNode(stream);
  } else {
    item.constant = parseItemConstNode(stream);
  }
  return item;
}