/**
 ** \file bind/binder.cc
 ** \brief Implementation for bind/binder.hh.
 */

#include <stdexcept>

#include <ast/all.hh>
#include <bind/binder.hh>

#include <misc/contract.hh>

#include "misc/error.hh"

namespace bind
{
  /*-----------------.
  | Error handling.  |
  `-----------------*/

  /// The error handler.
  const misc::error& Binder::error_get() const { return error_; }

  // FIXME: Some code was deleted here.

  // Start Fix

  /* Create and Delete scope functions */
  void Binder::scope_begin()
  {
    map_vardec_.scope_begin();
    map_typedec_.scope_begin();
    map_typedec_.scope_begin();
  }

  void Binder::scope_end()
  {
    map_vardec_.scope_end();
    map_typedec_.scope_end();
    map_typedec_.scope_end();
  }

  /* Initializer */
  void Binder::operator()(ast::Ast& e)
  {
    map_typedec_.put("int",
                     nullptr); // Adding primitive types int & string to the map
    map_typedec_.put("string", nullptr);
    super_type::operator()(e);
  }

  /* Populates the Binder */
  void Binder::operator()(ast::VarDec& e)
  {
    map_vardec_.put(e.name_get(), e.init_get());
  }

  void Binder::operator()(ast::FunctionDec& e)
  {
    map_fundec_.put(e.name_get(), e.body_get());
  }
  void Binder::operator()(ast::TypeDec& e)
  {
    map_typedec_.put(e.name_get(), e.ty_get());
  }

  void Binder::operator()(ast::ForExp& e)
  {
    e.def_set(&e);
    in_loop_ = true;
    loops_.push(e);
    scope_begin();
    e.body_get.visit(*this);
    scope_end();
    loops_.pop();
    in_loop_ = false;
  }

  void Binder::operator()(ast::WhileExp& e)
  {
    e.def_set(&e);
    in_loop_ = true;
    loops_.push(e);
    e.body_get.visit(*this);
    loops_.pop();
    in_loop_ = false;
  }

  /* Check the existance in the Binder */
  void Binder::operator()(ast::SimpleVar& e)
  {
    auto ast_obtained = map_vardec_.get(e.name_get());
    if (!ast_obtained)
      {
        error_ << misc::error::error_type::bind;
        error_ << "undeclared variable: " << e.name_get();
        error_.exit();
      }
    e.def_set(ast_obtained);
  }
  void Binder::operator()(ast::CallExp& e)
  {
    auto ast_obtained = map_fundec_.get(e.name_get());
    if (!ast_obtained)
      {
        error_ << misc::error::error_type::bind;
        error_ << "undeclared function: " << e.name_get();
        error_.exit();
      }
    e.def_set(ast_obtained);
  }

  void Binder::operator()(ast::BreakExp& e)
  {
    if (in_loop_)
      {
        if (!(loops_.empty()))
          {
            auto ast_obtained = loops_.top();
            loops_.pop();
            e.def_set(ast_obtained);
          }
        else
          {
            error_ << misc::error::error_type::bind;
            error_ << "break outside loop: ";
            error_.exit();
          }
      }
    else
      {
        error_ << misc::error::error_type::bind;
        error_ << "Invalid break";
        error_.exit();
      }
  }

  void Binder::operator()(ast::NameTy& e)
  {
    auto ast_obtained = map_typedec_.get(e.name_get());
    if (!ast_obtained)
      {
        error_ << misc::error::error_type::bind;
        error_ << "undeclared type: " << e.name_get();
        error_.exit();
      }
    e.def_set(&ast_obtained);
  }

  /* Change the scope, new Binder */
  void Binder::operator()(ast::SeqExp& e)
  {
    for (size_t i; i < e.exps_get().size(); i++)
      {
        scope_begin();
        e.exps_get().at(i)->accept(*this);
        scope_end();
      }
  }
  void Binder::operator()(ast::LetExp& e)
  {
    scope_begin();
    e.chunks_get().accept(
      *this); // TODO fix go through all header then all body
    scope_end();

    scope_begin();
    e.body_get().accept(*this);
    scope_end();
  }

  // End Fix

} // namespace bind
