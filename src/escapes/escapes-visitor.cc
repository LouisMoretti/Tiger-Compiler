/**
 ** \file escapes/escapes-visitor.cc
 ** \brief Implementation for escapes/escapes-visitor.hh.
 */

#include <ast/all.hh>
#include <escapes/escapes-visitor.hh>
#include <misc/contract.hh>

namespace escapes
{
  // FIXME: Some code was deleted here.

  void EscapesVisitor::operator()(ast::VarDec& e)
  {
    e.escape_set(false);

    vars_scope_[&e] = scope_;

    super_type::operator()(e);
  }

  void EscapesVisitor::operator()(ast::SimpleVar& e)
  {
    if (vars_scope_.find(e.def_get()) != vars_scope_.end()
        && vars_scope_[e.def_get()] < scope_)
      e.def_get()->escape_set(true);

    super_type::operator()(e);
  }

  void EscapesVisitor::operator()(ast::FunctionDec& e)
  {
    scope_++;
    super_type::operator()(e);
    scope_--;
  }

  void EscapesVisitor::operator()(ast::ForExp& e)
  {
    scope_++;
    super_type::operator()(e);
    scope_--;
  }

  void EscapesVisitor::operator()(ast::SeqExp& e)
  {
    // scope_++;
    super_type::operator()(e);
    // scope_--;
  }

  void EscapesVisitor::operator()(ast::LetExp& e)
  {
    scope_++;
    super_type::operator()(e);
    scope_--;
  }

} // namespace escapes
