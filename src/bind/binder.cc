/**
 ** \file bind/binder.cc
 ** \brief Implementation for bind/binder.hh.
 */

#include <stdexcept>

#include <ast/all.hh>
#include <bind/binder.hh>

#include <set>
#include <misc/contract.hh>

#include "ast/exp.hh"
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
    map_fundec_.scope_begin();
  }

  void Binder::scope_end()
  {
    map_vardec_.scope_end();
    map_typedec_.scope_end();
    map_fundec_.scope_end();
  }

  /* Populates the Binder */
  void Binder::operator()(ast::VarDec& e) { map_vardec_.put(e.name_get(), &e); }

  void Binder::operator()(ast::VarChunk& e)
  {
    std::set<misc::symbol> vars;
    for (const auto& dec : e)
      {
        if (vars.find(dec->name_get()) != vars.end())
          {
            error_ << misc::error::error_type::bind;
            error_ << "duplicated parameter: " << dec->name_get();
            error_.exit();
          }
        vars.insert(dec->name_get());
      }
    for (const auto& dec : e)
      {
        dec->accept(*this);
      }
  }

  void Binder::operator()(ast::TypeChunk& e)
  {
    for (const auto& dec : e)
      {
        dec->ty_get().accept(*this);
        map_typedec_.put(dec->name_get(), dec);
      }
    for (const auto& dec : e)
      {
        dec->accept(*this);
      }
  }

  void Binder::operator()(ast::FunctionChunk& e)
  {
    for (const auto& dec : e)
      {
        scope_begin();
        dec->formals_get().accept(*this);
        scope_end();
        if (map_fundec_.contains(dec->name_get()))
          {
            error_ << misc::error::error_type::bind;
            error_ << "duplicated function: " << dec->name_get();
            error_.exit();
          }
        map_fundec_.put(dec->name_get(), dec);
      }
    for (const auto& dec : e)
      {
        dec->accept(*this);
      }
  }

  void Binder::operator()(ast::FunctionDec& e)
  {
    if (e.body_get())
      e.body_get()->accept(*this);
  }
  void Binder::operator()(ast::TypeDec& e)
  {
    // TODO check if its string or int
    if (e.name_get() != "string" && e.name_get() != "int")
      map_typedec_.put(e.name_get(), &e);
  }

  void Binder::operator()(ast::ForExp& e)
  {
    in_loop_ = true;
    loops_.push(&e);
    scope_begin();
    e.vardec_get().accept(*this);
    e.body_get().accept(*this);
    scope_end();
    loops_.pop();
    in_loop_ = false;
  }

  void Binder::operator()(ast::WhileExp& e)
  {
    in_loop_ = true;
    loops_.push(&e);
    e.body_get().accept(*this);
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
        error_ << "undeclared/undefined function: " << e.name_get();
        error_.exit();
      }
    e.def_set(ast_obtained);
    for (const auto& dec : e.args_get())
      {
        dec->accept(*this);
      }
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
    e.def_set(ast_obtained);
  }

  /* Change the scope, new Binder */
  void Binder::operator()(ast::SeqExp& e)
  {
    for (size_t i = 0; i < e.exps_get().size(); i++)
      {
        scope_begin();
        e.exps_get().at(i)->accept(*this);
        scope_end();
      }
  }
  void Binder::operator()(ast::LetExp& e)
  {
    scope_begin();
    e.chunks_get().accept(*this);

    e.body_get().accept(*this);
    scope_end();
  }

  // End Fix

} // namespace bind
