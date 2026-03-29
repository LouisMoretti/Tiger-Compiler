/**
 ** \file bind/renamer.hxx
 ** \brief Template methods of bind::Renamer.
 */

#pragma once

#include <bind/renamer.hh>
#include "misc/symbol.hh"

namespace bind
{
  // FIXME: Some code was deleted here.

  template <class E, class Def> void Renamer::visit(E& e, const Def* def)
  {
    // FIXED: Some code was deleted here.
    if (!def)
      return;

    if (unique_names_.find(def) == unique_names_.end())
      unique_names_[def] = misc::symbol::fresh(def->name_get());

    e.name_set(unique_names_[def]);
  }
} // namespace bind
