/**
 ** \file misc/symbol.hxx
 ** \brief Inline implementation of misc::symbol.
 */

#pragma once

#include <misc/symbol.hh>

namespace misc
{
  inline symbol& symbol::operator=(const symbol& rhs)
  {
    // FIXED: Some code was deleted here.
    // Start Fix
    this->obj_ = &rhs.get();
    return *this;
    // End Fix
  }

  inline bool symbol::operator==(const symbol& rhs) const
  {
    // FIXED: Some code was deleted here.
    // Start Fix
    return obj_ == rhs.obj_;
    // End Fix
  }

  inline bool symbol::operator!=(const symbol& rhs) const
  {
    // FIXED: Some code was deleted here.
    // Start Fix
    return obj_ != rhs.obj_;
    // End Fix
  }

  inline std::ostream& operator<<(std::ostream& ostr, const symbol& the)
  {
    // FIXED: Some code was deleted here.
    // Start Fix
    return ostr << the.get();
    // End Fix
  }

} // namespace misc
