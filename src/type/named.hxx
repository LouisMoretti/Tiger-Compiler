/**
 ** \file type/named.hxx
 ** \brief Inline methods for type::Named.
 */
#pragma once

#include <misc/contract.hh>
#include <type/named.hh>
#include "misc/error.hh"

namespace type
{
  inline const Type* Named::type_get() const { return type_; }

  inline void Named::type_set(const Type* type) const { type_ = type; }

  inline misc::symbol Named::name_get() const { return name_; }

  inline void Named::name_set(misc::symbol name) { name_ = name; }

  inline const Type& Named::actual() const
  {
    // FIXED: Some code was deleted here.
    if (type_)
      return type_->actual();

    misc::error error_;
    error_ << misc::error::error_type::type;
    error_ << "Uninitialized type in named.";
    error_.exit();
  }

} // namespace type
