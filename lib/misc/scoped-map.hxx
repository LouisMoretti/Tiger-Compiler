/** \file misc/scoped-map.hxx
 ** \brief Implementation of misc::scoped_map.
 */

#pragma once

#include <sstream>
#include <stdexcept>
#include <type_traits>

#include <ranges>
#include <misc/algorithm.hh>
#include <misc/contract.hh>
#include <misc/indent.hh>

namespace misc
{
  // FIXME: Some code was deleted here.
  // Start Fix

  template <typename Key, typename Data>
  scoped_map<Key, Data>::scoped_map()
    : scoped_map_()
  {
    scope_begin();
  }

  template <typename Key, typename Data>
  void scoped_map<Key, Data>::put(const Key& key, const Data& value)
  {
    if (scoped_map_.empty())
      {
        throw std::logic_error("scoped-map.hxx: l-" + std::to_string(__LINE__)
                               + ": put call with empty scoped map");
      }

    if (scoped_map_.back().contains(key))
      {
        scoped_map_.back()[key] = value;
      }
    else
      {
        scoped_map_.back().insert(std::pair<Key, Data>(key, value));
      }
  }

  template <typename Key, typename Data>
  Data scoped_map<Key, Data>::get(const Key& key) const
    requires(pointer_type_concept<Key, Data>)
  {
    if (scoped_map_.empty() || !scoped_map_.back().contains(key))
      {
        return nullptr;
      }

    return scoped_map_.back().at(key);
  }

  template <typename Key, typename Data>
  Data scoped_map<Key, Data>::get(const Key& key) const
    requires(!pointer_type_concept<Key, Data>)
  {
    if (scoped_map_.empty() || !scoped_map_.back().contains(key))
      {
        //TODO: need to use the error type
        throw std::range_error("scoped-map.hxx: l-" + std::to_string(__LINE__)
                               + ": key not found");
      }

    return scoped_map_.back().at(key);
  }

  template <typename Key, typename Data>
  void scoped_map<Key, Data>::scope_begin()
  {
    if (scoped_map_.empty())
      {
        scoped_map_.emplace_back();
      }
    else
      {
        auto& last_to_copy = scoped_map_.back();
        scoped_map_.emplace_back();
        auto& last = scoped_map_.back();
        for (auto pair : last_to_copy)
          {
            last.insert(pair);
          }
      }
  }

  template <typename Key, typename Data> void scoped_map<Key, Data>::scope_end()
  {
    if (scoped_map_.empty())
      {
        throw std::logic_error("scoped-map.hxx: l-" + std::to_string(__LINE__)
                               + ": invalid end scope");
      }

    scoped_map_.pop_back();
  }

  template <typename Key, typename Data>
  std::ostream& scoped_map<Key, Data>::dump(std::ostream& ostr) const
  {
    int i = 1;

    for (auto level : scoped_map_)
      {
        ostr << "level : " << i << "\n";

        for (auto act : level)
          {
            ostr << "key : " << act.key << "\t\t| value : " << act.value
                 << "\n";
          }

        i++;
      }

    return ostr;
  }

  // End Fix

  template <typename Key, typename Data>
  inline std::ostream& operator<<(std::ostream& ostr,
                                  const scoped_map<Key, Data>& tbl)
  {
    return tbl.dump(ostr);
  }

} // namespace misc
