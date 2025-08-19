#include "TypeDef.hpp"
#include "Symbol.hpp"
#include "TypeKind.hpp"

TypeDef::TypeDef(const std::string &name) : name_(name) {}

TypeDef::TypeDef(const std::string &name,
                 const std::vector<std::string> &member_names,
                 std::vector<std::shared_ptr<TypeKind>> &&member_types)
    : name_(name) {
  for (size_t i = 0; i < member_names.size(); ++i) {
    members_.emplace(std::move(member_names[i]), std::move(member_types[i]));
  }
}

TypeDef::~TypeDef() = default;

auto TypeDef::getName() const -> const std::string & { return name_; }

auto TypeDef::getMember(const std::string &name) const
    -> std::shared_ptr<TypeKind> {
  auto it = members_.find(name);
  if (it != members_.end()) {
    return it->second;
  }
  return nullptr;
}