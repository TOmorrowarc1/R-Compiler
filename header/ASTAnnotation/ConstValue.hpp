#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

class ConstValue {
public:
  ConstValue() = default;
  virtual ~ConstValue() = default;
  virtual auto clone() const -> std::unique_ptr<ConstValue> = 0;
};

class ConstValueInt : public ConstValue {
private:
  int32_t value_;

public:
  ConstValueInt(int32_t value);
  ~ConstValueInt() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getValue() const -> int32_t;
};

class ConstValueBool : public ConstValue {
private:
  bool value_;

public:
  ConstValueBool(bool value);
  ~ConstValueBool() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getValue() const -> bool;
};

class ConstValueChar : public ConstValue {
private:
  char value_;

public:
  ConstValueChar(char value);
  ~ConstValueChar() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getValue() const -> char;
};

class ConstValueString : public ConstValue {
private:
  std::string value_;

public:
  ConstValueString(const std::string &value);
  ~ConstValueString() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getValue() const -> const std::string &;
};

class ConstValueStruct : public ConstValue {
private:
  std::unordered_map<std::string, std::shared_ptr<ConstValue>> fields_;

public:
  ConstValueStruct(
      std::unordered_map<std::string, std::shared_ptr<ConstValue>> &&fields);
  ~ConstValueStruct() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getField(const std::string &name) const -> const std::shared_ptr<ConstValue>;
};

class ConstValueEnum: public ConstValue {
private:
  std::string variant_;
public:
  ConstValueEnum(const std::string &variant);
  ~ConstValueEnum() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getVariant() const -> const std::string &;
};

class ConstValueArray : public ConstValue {
private:
  std::vector<std::shared_ptr<ConstValue>> elements_;
public:
  ConstValueArray(std::vector<std::shared_ptr<ConstValue>> &&elements);
  ~ConstValueArray() override;
  auto clone() const -> std::unique_ptr<ConstValue> override;
  auto getElement(int32_t index) const -> const std::shared_ptr<ConstValue>;
};