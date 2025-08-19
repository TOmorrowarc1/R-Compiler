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

template <typename Derived, typename Base>
auto dynamic_shared_ptr_cast(const std::shared_ptr<Base> &base_ptr)
    -> std::shared_ptr<Derived> {
  return std::dynamic_pointer_cast<Derived>(base_ptr);
}

template <typename Derived, typename Base>
auto is_instance_of(const std::shared_ptr<Base> &base_ptr) -> bool {
  return std::dynamic_pointer_cast<Derived>(base_ptr) != nullptr;
}

template <typename Derived, typename Base>
auto is_instance_of(const Base *base_ref) -> bool {
  return dynamic_cast<const Derived *>(base_ref) != nullptr;
}