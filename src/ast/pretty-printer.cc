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
#include "ast/seq-exp.hh"

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
    ostr_ << e.name_get() << " = " << e.var_get();
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

  void PrettyPrinter::operator()(const ArrayExp& e)
  {
    ostr_ << e.type_name_get() << "[";
    e.size_get().accept(*this);

    ostr_ << "] of";
    e.init_get().accept(*this);
  }

  void PrettyPrinter::operator()(const ArrayTy& e)
  {
    ostr_ << "array of ";
    e.base_type_get().accept(*this);
  }

  void PrettyPrinter::operator()(const AssignExp& e)
  {
    e.var_get().accept(*this);
    ostr_ << " := ";

    e.exp_get().accept(*this);
  }

  void PrettyPrinter::operator()(const BreakExp& e)
  {
    ostr_ << "break;";
    (void)e;
  }

  void PrettyPrinter::operator()(const CallExp& e)
  {
    ostr_ << e.name_get();
    auto act = e.args_get();

    if (act.empty())
      return;

    act.at(0)->accept(*this);

    for (size_t i = 1; i < act.size(); i++)
      {
        ostr_ << ", ";
        act.at(i)->accept(*this);
      }
  }

  void PrettyPrinter::operator()(const ChunkList& e)
  {
    for (auto act : e.chunks_get())
      {
        act->accept(*this);
      }
  }

  void PrettyPrinter::operator()(const ClassTy& e)
  {
    e.super_get().accept(*this);
    e.chunks_get().accept(*this);
  }

  void PrettyPrinter::operator()(const Field& e)
  {
    ostr_ << e.name_get();
    e.type_name_get().accept(*this);
  }

  void PrettyPrinter::operator()(const FieldInit& e)
  {
    ostr_ << e.name_get();
    e.init_get().accept(*this);
  }

  void PrettyPrinter::operator()(const ForExp& e)
  {
    ostr_ << "for ";
    e.vardec_get().accept(*this);
    ostr_ << " to ";

    e.hi_get().accept(*this);
    ostr_ << " do ";

    e.body_get().accept(*this);
  }

  void PrettyPrinter::operator()(const FunctionDec& e)
  {
    ostr_ << "function " << e.name_get() << "( ";
    e.formals_get().accept(*this);
    ostr_ << ") ";

    if (e.result_get() != nullptr)
      {
        ostr_ << ": ";
        e.result_get()->accept(*this);
      }

    ostr_ << "= ";
    e.body_get()->accept(*this);
  }

  void PrettyPrinter::operator()(const IfExp& e)
  {
    ostr_ << "if ";

    e.test_get().accept(*this);

    ostr_ << " then ";

    e.thenclause_get().accept(*this);

    const SeqExp* seq = dynamic_cast<const SeqExp*>(&(e.elseclause_get()));
    if (seq == nullptr || !seq->exps_get().empty())
      {
        ostr_ << " else ";
        e.elseclause_get().accept(*this);
      }
  }

  void PrettyPrinter::operator()(const IntExp& e) { ostr_ << e.value_get(); }

  void PrettyPrinter::operator()(const LetExp& e)
  {
    ostr_ << "let ";

    for (auto act : e.chunks_get())
      {
        act->accept(*this);
      }

    ostr_ << " in ";

    e.body_get().accept(*this);

    ostr_ << "end";
  }

  void PrettyPrinter::operator()(const MethodCallExp& e)
  {
    ostr_ << e.name_get() << " (";

    auto act = e.args_get();

    if (!act.empty())
      {
        act.at(0)->accept(*this);

        for (size_t i = 1; i < act.size(); i++)
          {
            ostr_ << ", ";
            act.at(i)->accept(*this);
          }
      }

    ostr_ << ")";
    e.object_get().accept(*this);
  }

  void PrettyPrinter::operator()(const MethodDec& e)
  {
    ostr_ << e.name_get() << " (";
    e.formals_get().accept(*this);
    ostr_ << ") : ";
    e.result_get()->accept(*this);
    e.body_get()->accept(*this);
  }

  void PrettyPrinter::operator()(const NameTy& e) { ostr_ << e.name_get(); }

  void PrettyPrinter::operator()(const NilExp& e)
  {
    ostr_ << "nil";
    (void)e;
  }

  void PrettyPrinter::operator()(const ObjectExp& e)
  {
    ostr_ << e.type_name_get();
  }

  void PrettyPrinter::operator()(const OpExp& e)
  {
    e.left_get().accept(*this);

    switch (e.oper_get())
      {
      case OpExp::Oper::add:
        ostr_ << "+";
        break;
      case OpExp::Oper::sub:
        ostr_ << "-";
        break;
      case OpExp::Oper::mul:
        ostr_ << "*";
        break;
      case OpExp::Oper::div:
        ostr_ << "/";
        break;
      case OpExp::Oper::eq:
        ostr_ << "=";
        break;
      case OpExp::Oper::ne:
        ostr_ << "<>";
        break;
      case OpExp::Oper::lt:
        ostr_ << "<";
        break;
      case OpExp::Oper::le:
        ostr_ << "<=";
        break;
      case OpExp::Oper::gt:
        ostr_ << ">";
        break;
      case OpExp::Oper::ge:
        ostr_ << ">=";
        break;
      }
    e.right_get().accept(*this);
  }

  void PrettyPrinter::operator()(const RecordExp& e)
  {
    ostr_ << e.type_name_get() << " = {";
    for (auto act : e.fields_get())
      {
        act->accept(*this);
      }
    ostr_ << "}";
  }

  void PrettyPrinter::operator()(const RecordTy& e)
  {
    for (auto act : e.fields_get())
      {
        act->accept(*this);
      }
  }

  void PrettyPrinter::operator()(const SeqExp& e)
  {
    for (auto act : e.exps_get())
      {
        act->accept(*this);
      }
  }

  void PrettyPrinter::operator()(const StringExp& e) { ostr_ << e.value_get(); }

  void PrettyPrinter::operator()(const TypeDec& e) { e.ty_get().accept(*this); }

  void PrettyPrinter::operator()(const VarDec& e)
  {
    ostr_ << "var " << e.name_get() << " ";

    if (e.type_name_get() != nullptr)
      {
        ostr_ << ": " << e.type_name_get();
      }

    ostr_ << ":= ";
    if (e.init_get())
      e.init_get()->accept(*this);
  }

  void PrettyPrinter::operator()(const WhileExp& e)
  {
    ostr_ << "while " << e.test_get() << "\n";
    e.body_get().accept(*this);
  }

} // namespace ast
