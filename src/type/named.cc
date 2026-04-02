/**
 ** \file type/named.cc
 ** \brief Implementation for type/named.hh.
 */

#include <set>

#include <set>
#include <type/named.hh>
#include <type/visitor.hh>

namespace type
{
  Named::Named(misc::symbol name)
    : name_(name)
    , type_(nullptr)
  {}

  Named::Named(misc::symbol name, const Type* type)
    : name_(name)
    , type_(type)
  {}

  // Inherited functions
  void Named::accept(ConstVisitor& v) const { v(*this); }

  void Named::accept(Visitor& v) { v(*this); }

  bool Named::sound() const
  {
    // FIXED: Some code was deleted here (Sound).
    std::set<const Named*> prev;
    auto* curr = this;
    prev.insert(curr);

    while ((curr = dynamic_cast<const Named*>(curr->type_get())) != nullptr)
      {
        if (prev.contains(curr))
          return false;
        prev.insert(curr);
      }
    return true;
  }

  bool Named::compatible_with(const Type& other) const
  {
    // FIXED: Some code was deleted here (Special implementation of "compatible_with" for Named).
    return this->actual().compatible_with(other);
  }

} // namespace type
