/**
 ** \file ast/ty.hh
 ** \brief Declaration of ast::Ty.
 */

#pragma once

#include <ast/ast.hh>
#include <ast/typable.hh>
#include <ast/type-constructor.hh>

namespace ast
{
  /// Ty.
  class Ty
    : public Ast
    , public Typable
    , public TypeConstructor
  {
  public:
    /** \name Ctor & dtor.
     ** \{ */
    /// Construct a Ty node.
    explicit Ty(const Location& location);
    Ty(const Ty&) = delete;
    Ty& operator=(const Ty&) = delete;
    /// Destroy a Ty node.
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
    // FIXED
    // Start Fix
    void type_set(const type::Type type) override;
    const type::Type type_get() const override;

    void created_type_set(const type::Type* created_type) override;
    const type::Type* created_type_get() const override;
    // End Fix
    /** \} */

  protected:
    // FIXED
    // Start Fix
    type::Type type_;
    type::Type* created_type_ = nullptr;
    // End Fix
  };
} // namespace ast
#include <ast/ty.hxx>
