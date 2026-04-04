/**
 ** \file type/type-checker.hxx
 ** \brief Inline methods of type::TypeChecker.
 */

#pragma once

#include <ast/all.hh>
#include <type/pretty-printer.hh>
#include <type/type-checker.hh>
#include <type/types.hh>
#include "type/builtin-types.hh"

namespace type
{

  /*----------------.
  | Setting types.  |
  `----------------*/

  template <typename NodeType>
  void TypeChecker::type_default(NodeType& e, const type::Type* type)
  {
    // FIXED: Some code was deleted here.
    if (e.type_get() == nullptr)
      e.type_set(type);
  }

  template <typename NodeType>
  void TypeChecker::created_type_default(NodeType& e, const type::Type* type)
  {
    // FIXED: Some code was deleted here.
    if (e.created_type_get() == nullptr)
      e.created_type_set(type);

    type_default(e, type);
  }

  template <typename NodeType>
  void TypeChecker::type_set(NodeType& e, const type::Type* type)
  {
    // FIXED: Some code was deleted here (Basically e.type_set(type)).
    e.type_set(type);
  }

  /*-----------------.
  | Error handling.  |
  `-----------------*/

  template <typename T>
  void
  TypeChecker::error(const ast::Ast& ast, const std::string& msg, const T& exp)
  {
    error_ << misc::error::error_type::type << ast.location_get() << ": " << msg
           << ": " << exp << std::endl;
  }

  template <typename T, typename U>
  void
  TypeChecker::error_and_recover(T& loc, const std::string& msg, const U& exp)
  {
    error(loc, msg, exp);

    // const type::Type* nil_error_ = nullptr;
    // FIXED: Some code was deleted here (Get the Nil type).
    // loc.type_set(nil_error_);
    loc.type_set(&Nil::instance());
  }

  template <typename NodeType>
  void TypeChecker::check_type(NodeType& e,
                               const std::string& s,
                               const Type& expected)
  {
    // FIXED: Some code was deleted here.
    // Start Fix
    type(e);
    if (e.type_get()->actual() != expected)
      error_and_recover(e, s, e.type_get()->actual());
    // End Fix
  }

  /*------------.
  | Functions.  |
  `------------*/

  template <typename Routine_Type, typename Routine_Node>
  void TypeChecker::visit_routine_body(Routine_Node& e)
  {
    // FIXED: Some code was deleted here.
    const Routine_Type* rt =
      dynamic_cast<const Routine_Type*>(e.created_type_get());

    if (!rt)
      {
        // unreachable
        error_and_recover(e, "wrong type", e.type_get()->actual());
      }

    check_types(e, "body", *type(*e.body_get()), "return", rt->result_get());
  }

} // namespace type
