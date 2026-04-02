/**
 ** \file type/record.cc
 ** \brief Implementation for type/record.hh.
 */

#include <ostream>

#include <type/builtin-types.hh>
#include <type/record.hh>
#include <type/visitor.hh>
#include "misc/error.hh"

namespace type
{
  void Record::accept(ConstVisitor& v) const { v(*this); }

  void Record::accept(Visitor& v) { v(*this); }

  // FIXED: Some code was deleted here (Field manipulators).
  const Type* Record::field_type(misc::symbol key) const
  {
    for (auto& field : fields_)
      {
        if (field.name_get() == key)
          return &field.type_get();
      }

    misc::error error_;
    error_ << misc::error::error_type::type;
    error_ << "No field with key: " << key;
    error_.exit();
  }

  int Record::field_index(misc::symbol key) const
  {
    int i = 0;
    for (auto& field : fields_)
      {
        if (field.name_get() == key)
          return i;
        i++;
      }

    misc::error error_;
    error_ << misc::error::error_type::type;
    error_ << "No field with key: " << key;
    error_.exit();
  }

  // FIXED: Some code was deleted here (Special implementation of "compatible_with" for Record).
  bool Record::compatible_with(const Type& other) const
  {
    if (dynamic_cast<const Nil*>(&other.actual()))
      return true;
    return &this->actual() == &other.actual();
  }

} // namespace type
