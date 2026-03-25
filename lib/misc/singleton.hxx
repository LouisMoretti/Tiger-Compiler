//
// Created by arnaud.bellicha on 3/25/26.
//

#pragma once

#include "misc/singleton.hh"

namespace misc
{

  template <typename T> const Singleton<T>& misc::Singleton<T>::instance()
  {
    static Singleton instance;
    return instance; /* This seems familiar? */
  }

} // namespace misc