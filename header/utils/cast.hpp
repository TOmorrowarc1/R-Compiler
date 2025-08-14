#pragma once
#include <memory>

template <typename Derived, typename Base>
auto dynamic_unique_ptr_cast(std::unique_ptr<Base> &base_ptr)
    -> std::unique_ptr<Derived> {
  if (dynamic_cast<Derived *>(base_ptr.get())) {
    Base *released_raw_ptr = base_ptr.release();
    Derived *derived_raw_ptr = static_cast<Derived *>(released_raw_ptr);
    return std::unique_ptr<Derived>(derived_raw_ptr);
  }
  return nullptr;
}

template <typename Derived, typename Base>
auto is_instance_of(const std::unique_ptr<Base> &base_ptr) -> bool {
  return dynamic_cast<Derived *>(base_ptr.get()) != nullptr;
}