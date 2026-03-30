/**
 ** \file type/builtin-types.hh
 ** \brief The classes Int, String, Void.
 */
#pragma once

#include <misc/singleton.hh>
#include <type/fwd.hh>
#include <type/type.hh>

namespace type
{
  // FIXED: Some code was deleted here (Builtin types : Int, String, Void, Nil).
  // Start Fix
  class Int
    : public misc::Singleton<Int>
    , public Type
  {
  public:
    Int(const Int&) = delete;
    Int& operator=(const Int&) = delete;
    Int() = default;

    void accept(ConstVisitor& v) const override;
    /// Accept a non-const visitor \a v.
    void accept(Visitor& v) override;
    /// \}
  };

  class Nil
    : public misc::Singleton<Nil>
    , public Type
  {
  public:
    Nil(const Nil&) = delete;
    Nil& operator=(const Nil&) = delete;
    Nil() = default;

    void accept(ConstVisitor& v) const override;
    /// Accept a non-const visitor \a v.
    void accept(Visitor& v) override;
    /// \}
  };

  class String
    : public misc::Singleton<String>
    , public Type
  {
  public:
    String(const String&) = delete;
    String& operator=(const String&) = delete;
    String() = default;

    void accept(ConstVisitor& v) const override;
    /// Accept a non-const visitor \a v.
    void accept(Visitor& v) override;
    /// \}
  };

  class Void
    : public misc::Singleton<Void>
    , public Type
  {
  public:
    Void(const Void&) = delete;
    Void& operator=(const Void&) = delete;
    Void() = default;

    void accept(ConstVisitor& v) const override;
    /// Accept a non-const visitor \a v.
    void accept(Visitor& v) override;
    /// \}
  };
  // End Fix
} // namespace type
