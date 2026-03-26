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
  // FIXME: Some code was deleted here (Builtin types : Int, String, Void, Nil).
  // Start Fix
  class Int
    : public misc::Singleton<Int>
    , public Type
  {
    Int(const Int&) = delete;
    Int& operator=(const Int&) = delete;

    void accept(ConstVisitor& v) const override;
    /// Accept a non-const visitor \a v.
    void accept(Visitor& v) override;
    /// \}
  };

  class Nil
    : public misc::Singleton<Nil>
    , public Type
  {
    Nil(const Nil&) = delete;
    Nil& operator=(const Nil&) = delete;

    void accept(ConstVisitor& v) const override;
    /// Accept a non-const visitor \a v.
    void accept(Visitor& v) override;
    /// \}
  };

  class String
    : public misc::Singleton<String>
    , public Type
  {
    String(const String&) = delete;
    String& operator=(const String&) = delete;

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

    void accept(ConstVisitor& v) const override;
    /// Accept a non-const visitor \a v.
    void accept(Visitor& v) override;
    /// \}
  };
  // End Fix
} // namespace type
