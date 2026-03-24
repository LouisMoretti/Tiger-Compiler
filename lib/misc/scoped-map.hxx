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
  void scoped_map<Key, Data>::put(const Key& key, const Data& value)
  {
    if (scoped_map_.empty())
      {
        throw std::logic_error("scoped-map.hxx: l-" + std::to_string(__LINE__)
                               + ": put call with empty scoped map");
      }

    if (*scoped_map_.end().contains(key))
      {
        *scoped_map_.end()[key] = value;
      }
    else
      {
        (*scoped_map_.end()).insert(std::pair<Key, Data>(key, value));
      }
  }

  template <typename Key, typename Data>
  Data scoped_map<Key, Data>::get(const Key& key) const
  {
    if (scoped_map_.empty() || !(*scoped_map_.end()).contains(key))
      {
        throw std::range_error("scoped-map.hxx: l-" + std::to_string(__LINE__)
                               + ": key not found");
      }

    auto& end = scoped_map_.end();
    Data res;
    bool found = false;

    while (!found && end != scoped_map_.begin())
      {
        if (end.contains(key))
          {
            res = end[key];
            found = true;
          }

        end--;
      }

    return res;
  }

  template <typename Key, typename Data>
  void scoped_map<Key, Data>::scope_begin()
  {
    if (scoped_map_.empty())
      {
        scoped_map_.emplace_back(std::map<Key, Data>());
      }
    else
      {
        auto& last_to_copy = *scoped_map_.end();
        scoped_map_.emplace_back(std::map<Key, Data>());
        auto& last = (*scoped_map_.end());
        for (auto& pair : last_to_copy)
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

    scoped_map_.erase(scoped_map_.end());
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
