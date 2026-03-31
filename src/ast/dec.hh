/**
 ** \file ast/dec.hh
 ** \brief Declaration of ast::Dec.
 */

#pragma once

#include <ast/ast.hh>
#include <ast/typable.hh>
#include <misc/symbol.hh>

namespace ast
{
  /// Dec.
  class Dec
    : public Ast
    , public Typable
  {
  public:
    /** \name Ctor & dtor.
     ** \{ */
    /// Construct a Dec node.
    Dec(const Location& location, misc::symbol name);
    Dec(const Dec&) = delete;
    Dec& operator=(const Dec&) = delete;
    /// Destroy a Dec node.
    /** \} */

    /// \name Visitors entry point.
    /// \{ */
    /// Accept a const visitor \a v.
    void accept(ConstVisitor& v) const override = 0;
    /// Accept a non-const visitor \a v.
    void accept(Visitor& v) override = 0;
    /// \}

    /** \name Accessors.
     ** \{ */
    /// Return name of the defined entity.
    misc::symbol name_get() const;
    /// Set name of the defined entity.
    void name_set(misc::symbol);

    // Fixed
    // Start Fix
    void type_set(const type::Type type) override;
    const type::Type type_get() const override;
    // End Fix
    /** \} */

  protected:
    /// Name of the defined entity.
    misc::symbol name_;

    // Fixed
    // Start Fix
    type::Type type_;
    // End Fix
  };
} // namespace ast
#include <ast/dec.hxx>
