/**
 ** \file bind/renamer.cc
 ** \brief Implementation of bind::Renamer.
 */

#include <bind/renamer.hh>
#include "ast/all.hh"

namespace bind
{
  using namespace ast;

  // FIXME: Some code was deleted here.
  void Renamer::operator()(ast::VarDec& e)
  {
    visit(e, &e);
    super_type::operator()(e);
  }

  void Renamer::operator()(ast::FunctionDec& e)
  {
    // Skip primitives.
    if (e.body_get() == nullptr)
      return;

    // Skip rename of "_main".
    if (e.name_get() != "_main")
      visit(e, &e);

    super_type::operator()(e);
  }

  void Renamer::operator()(ast::TypeDec& e)
  {
    visit(e, &e);
    super_type::operator()(e);
  }

  void Renamer::operator()(ast::SimpleVar& e)
  {
    visit(e, e.def_get());
    super_type::operator()(e);
  }

  void Renamer::operator()(ast::CallExp& e)
  {
    // Skip primitives.
    if (e.def_get() != nullptr && e.def_get()->body_get() != nullptr)
      visit(e, e.def_get());
    super_type::operator()(e);
  }

  void Renamer::operator()(ast::NameTy& e)
  {
    visit(e, e.def_get());
    super_type::operator()(e);
  }

} // namespace bind
