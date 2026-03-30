/**
 ** \file misc/singleton.hh
 ** \brief Generic singleton
 */

#pragma once

namespace misc
{
  template <typename T> class Singleton
  {
    // FIXME: Some code was deleted here.
  public:
    // Start Fix
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static const T& instance();

  protected:
    Singleton() = default;
    // End Fix
  };

} // namespace misc

#include "misc/singleton.hxx"