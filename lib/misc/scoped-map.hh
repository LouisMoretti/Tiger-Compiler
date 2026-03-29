/**
 ** \file misc/scoped-map.hh
 ** \brief Declaration of misc::scoped_map.
 **
 ** This implements a stack of dictionnaries.  Each time a scope is
 ** opened, a new dictionnary is added on the top of the stack; the
 ** dictionary is removed when the scope is closed.  Lookup of keys
 ** is done in the last added dictionnary first (LIFO).
 **
 ** In particular this class is used to implement symbol tables.
 **/

#pragma once

#include <map>
#include <unordered_map>
#include <vector>

namespace misc
{
  // FIXME: Some code was deleted here.
  // Start Fix
  template <typename Key, typename Data>
  concept pointer_type_concept = requires(Key k, Data a) {
    { *a };
  };
  // End Fix

  template <typename Key, typename Data> class scoped_map
  {
    // FIXME: Some code was deleted here.
    // Start Fix
  public:
    scoped_map();

    void put(const Key& key, const Data& value);
    Data get(const Key& key) const
      requires(pointer_type_concept<Key, Data>);
    Data get(const Key& key) const
      requires(!pointer_type_concept<Key, Data>);
    void scope_begin();
    void scope_end();
    bool contains(const Key& key);
    std::ostream& dump(std::ostream& ostr) const;

  private:
    std::vector<std::map<Key, Data>> scoped_map_;
    // End Fix
  };

  template <typename Key, typename Data>
  std::ostream& operator<<(std::ostream& ostr,
                           const scoped_map<Key, Data>& tbl);
} // namespace misc

#include <misc/scoped-map.hxx>
