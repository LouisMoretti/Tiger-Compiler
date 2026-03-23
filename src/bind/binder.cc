/**
 ** \file bind/binder.cc
 ** \brief Implementation for bind/binder.hh.
 */

#include <stdexcept>

#include <ast/all.hh>
#include <bind/binder.hh>

#include <misc/contract.hh>

#include "../../../../../../../../../afs/cri.epita.fr/user/a/ar/arnaud.bellicha/u/ing1/S6/tiger/Tiger-Compiler/lib/misc/error.hh"
#include "../ast/name-ty.hh"

namespace bind
{
  /*-----------------.
  | Error handling.  |
  `-----------------*/

  /// The error handler.
  const misc::error& Binder::error_get() const { return error_; }

  // FIXME: Some code was deleted here.

  // Start Fix

  /* Initializer */
  void Binder::operator()(ast::Ast& e)
  {
    map_.put("int", nullptr); // Adding primitive types int & string to the map
    map_.put("string", nullptr);
    super_type::operator()(e);
  }

  /* Populates the Binder */
  void Binder::operator()(ast::VarDec& e)
  {
    map_.put(e.name_get(), e.init_get());
  }

  void Binder::operator()(ast::FunctionDec& e)
  {
    map_.put(e.name_get(), e.body_get());
  }
  void Binder::operator()(ast::TypeDec& e)
  {
    map_.put(e.name_get(), e.ty_get());
  }

  void Binder::operator()(ForExp& e)
  {
    in_loop_ = true;
    loops_.push(e);
    map_.begin_scope();
    e.body_get.visit(*this);
    map_.end_scope();
    loops_.pop();
  }

  void Binder::operator()(WhileExp& e)
  {
    in_loop_ = true;
    loops_.push(e);
    map_.begin_scope();
    e.body_get.visit(*this);
    map_.end_scope();
    loops_.pop();
  }

  /* Check the existance in the Binder */
  void Binder::operator()(ast::SimpleVar& e)
  {
    try
      {
        auto ast_obtained = map_.get(e.name_get());
        e.def_set(ast_obtained);
      }
    catch (std::range_error error)
      {
        error_ << misc::error::error_type::bind;
        error_ << "undeclared variable: " << e.name_get();
        error_.exit();
      }
  }
  void Binder::operator()(ast::CallExp& e)
  {
    try
      {
        auto ast_obtained = map_.get(e.name_get());
        e.def_set(ast_obtained);
      }
    catch (std::range_error error)
      {
        error_ << misc::error::error_type::bind;
        error_ << "undeclared function: " << e.name_get();
        error_.exit();
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
        // Error Case break without loop
      }
    // Error Case break inside scope inside loop
  }

  void Binder::operator()(ast::NameTy& e)
  {
    try
      {
        auto ast_obtained = map_.get(e.name_get());
        e.def_set(ast_obtained);
      }
    catch (std::range_error error)
      {
        error_ << misc::error::error_type::bind;
        error_ << "undeclared type: " << e.name_get();
        error_.exit();
      }
  }

  /* Change the scope, new Binder */
  void Binder::operator()(ast::SeqExp& e)
  {
    for (size_t i; i < e.exps_get().size(); i++)
      {
        map_.begin_scope();
        e.exps_get().at(i)->accept(*this);
        map_.end_scope();
      }
  }
  void Binder::operator()(ast::LetExp& e)
  {
    map_.begin_scope();
    e.chunks_get().accept(*this);
    map_.end_scope();

    map_.begin_scope();
    e.body_get().accept(*this);
    map_.end_scope();
  }

  // End Fix

} // namespace bind
