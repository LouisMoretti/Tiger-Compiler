/**
 ** \file ast/pretty-printer.cc
 ** \brief Implementation of ast::PrettyPrinter.
 */

#include <ast/all.hh>
#include <ast/libast.hh>
#include <ast/pretty-printer.hh>
#include <misc/escape.hh>
#include <misc/indent.hh>
#include <misc/separator.hh>

namespace ast
{
  // Anonymous namespace: these functions are private to this file.
  namespace
  {

    /// \brief Output \a e on \a ostr.
    ///
    /// Used to factor the output of the name declared,
    /// and its possible additional attributes.
    inline std::ostream& operator<<(std::ostream& ostr, const Dec& e)
    {
      ostr << e.name_get();
      return ostr;
    }
  } // namespace

  PrettyPrinter::PrettyPrinter(std::ostream& ostr)
    : ostr_(ostr)
  {}

  void PrettyPrinter::operator()(const SimpleVar& e) { ostr_ << e.name_get(); }

  void PrettyPrinter::operator()(const FieldVar& e)
  {
    // FIXED: Some code was deleted here.
    ostr_ << e.name_get << " = " << e.var_get();
  }

  /* Foo[10]. */
  void PrettyPrinter::operator()(const SubscriptVar& e)
  {
    ostr_ << e.var_get() << '[' << misc::incindent << e.index_get()
          << misc::decindent << ']';
  }

  void PrettyPrinter::operator()(const CastExp& e)
  {
    ostr_ << "_cast(" << e.exp_get() << ", " << e.ty_get() << ')';
  }

  // FIXED: Some code was deleted here.

  void operator()(const ArrayExp& e) {}

  void operator()(const ArrayTy& e) {}

  void operator()(const AssignExp& e) {}

  void operator()(const Ast& e) {}

  void operator()(const BreakExp& e) {}

  void operator()(const CallExp& e) {}

  void operator()(const ChunkList& e) {}

  void operator()(const ClassTy& e) {}

  void operator()(const Dec& e) {}

  void operator()(const Exp& e) {}

  void operator()(const Field& e) {}

  void operator()(const FieldInit& e) {}

  void operator()(const ForExp& e) {}

  void operator()(const FunctionDec& e) {}

  void operator()(const IfExp& e) {}

  void operator()(const IntExp& e) {}

  void operator()(const LetExp& e) {}

  void operator()(const MethodCallExp& e) {}

  void operator()(const MethodDec& e) {}

  void operator()(const NameTy& e) {}

  void operator()(const NilExp& e) {}

  void operator()(const ObjectExp& e) {}

  void operator()(const OpExp& e) {}

  void operator()(const RecordExp& e) {}

  void operator()(const RecordTy& e) {}

  void operator()(const SeqExp& e) {}

  void operator()(const StringExp& e) {}

  void operator()(const Ty& e) {}

  void operator()(const TypeDec& e) {}

  void operator()(const Var& e) {}

  void operator()(const VarDec& e) {}

  void operator()(const WhileExp& e)
  {
    ostr_ << "while " << e.test_get() << "\n";
    this(e.body_get());
  }

} // namespace ast
