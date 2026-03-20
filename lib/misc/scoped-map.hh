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
  template <typename Key, typename Data> class scoped_map
  {
    // FIXME: Some code was deleted here.
    // Start Fix
  public:
    void put(const Key& key, const Data& value);
    Data get(const Key& key) const;
    void scope_begin();
    void scope_end();
    std::ostream& dump(std::ostream& ostr) const;

  private:
    std::vector<std::unordered_map<Key, Data>> scoped_map_;
    // End Fix
  };

  template <typename Key, typename Data>
  std::ostream& operator<<(std::ostream& ostr,
                           const scoped_map<Key, Data>& tbl);

  // FIXME: Some code was deleted here.
  // Start Fix
  template <typename Key, typename Data>
  concept pointer_type_concept = requires(Data a) {
    { a } -> std::same_as<nullptr_t>;
    { *a };
  };
  // End Fix
} // namespace misc

#include <misc/scoped-map.hxx>
