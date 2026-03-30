//
// Created by arnaud.bellicha on 3/25/26.
//
// FIXED: some code is added here
#pragma once

#include "misc/singleton.hh"

namespace misc
{
  template <typename T> const T& misc::Singleton<T>::instance()
  {
    static T singleton_;
    return singleton_;
  }

} // namespace misc