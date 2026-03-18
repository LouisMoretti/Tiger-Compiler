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
    ostr_ << e.name_get() << ".";
    e.var_get().accept(*this);
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
    ostr_ << e.type_name_get() << " [";
    e.size_get().accept(*this);

    ostr_ << "] of ";
    e.init_get().accept(*this);
  }

  void PrettyPrinter::operator()(const ArrayTy& e)
  {
    ostr_ << "array of ";
    e.base_type_get().accept(*this);
    misc::iendl(ostr_);
  }

  void PrettyPrinter::operator()(const AssignExp& e)
  {
    e.var_get().accept(*this);
    ostr_ << " := ";

    e.exp_get().accept(*this);
  }

  void PrettyPrinter::operator()(const BreakExp& e)
  {
    ostr_ << "break";
    (void)e;
  }

  void PrettyPrinter::operator()(const CallExp& e)
  {
    ostr_ << e.name_get();
    auto act = e.args_get();

    if (act.empty())
      {
        ostr_ << "()";
        return;
      }

    ostr_ << "(";
    act.at(0)->accept(*this);

    for (size_t i = 1; i < act.size(); i++)
      {
        ostr_ << ", ";
        act.at(i)->accept(*this);
      }

    ostr_ << ")";
  }

  void PrettyPrinter::operator()(const ChunkList& e)
  {
    bool b = true;

    for (auto act : e.chunks_get())
      {
        if (b)
          {
            b = false;
          }
        else
          {
            misc::iendl(ostr_);
          }

        act->accept(*this);
      }
  }

  void PrettyPrinter::operator()(const ClassTy& e)
  {
    e.super_get().accept(*this);
    e.chunks_get().accept(*this);
    misc::iendl(ostr_);
  }

  void PrettyPrinter::operator()(const Field& e)
  {
    ostr_ << e.name_get() << " : ";
    e.type_name_get().accept(*this);
  }

  void PrettyPrinter::operator()(const FieldInit& e)
  {
    ostr_ << e.name_get() << " = ";
    e.init_get().accept(*this);
  }

  void PrettyPrinter::operator()(const ForExp& e)
  {
    ostr_ << "for " << e.vardec_get().name_get() << " := ";

    e.vardec_get().init_get()->accept(*this);

    ostr_ << " to ";

    e.hi_get().accept(*this);
    ostr_ << " do";
    misc::incindent(ostr_);
    misc::iendl(ostr_);

    e.body_get().accept(*this);
    misc::decindent(ostr_);
  }

  void PrettyPrinter::operator()(const FunctionDec& e)
  {
    if (e.body_get() != nullptr)
      {
        ostr_ << "function ";
      }
    else
      {
        ostr_ << "primitive ";
      }

    ostr_ << e.name_get() << "(";

    if (!e.formals_get().empty())
      {
        auto begin = e.formals_get().begin();
        (*begin)->accept(*this);

        ++begin;
        for (; begin != e.formals_get().end(); ++begin)
          {
            ostr_ << ", ";
            (*begin)->accept(*this);
          }
      }

    ostr_ << ")";

    if (e.result_get() != nullptr)
      {
        ostr_ << " : ";
        e.result_get()->accept(*this);
      }

    if (e.body_get() != nullptr)
      {
        ostr_ << " =";
        misc::incindent(ostr_);
        misc::iendl(ostr_);
        e.body_get()->accept(*this);
      }
    else
      {
        misc::incindent(ostr_);
        misc::iendl(ostr_);
      }

    misc::decindent(ostr_);
  }

  void PrettyPrinter::operator()(const IfExp& e)
  {
    ostr_ << "if ";

    e.test_get().accept(*this);

    ostr_ << " then";
    misc::incindent(ostr_);
    misc::iendl(ostr_);

    e.thenclause_get().accept(*this);

    const SeqExp* seq = dynamic_cast<const SeqExp*>(&(e.elseclause_get()));
    if (seq == nullptr || !seq->exps_get().empty())
      {
        misc::decindent(ostr_);
        misc::iendl(ostr_);
        ostr_ << "else";
        misc::incindent(ostr_);
        misc::iendl(ostr_);

        e.elseclause_get().accept(*this);
        misc::decindent(ostr_);
      }
  }

  void PrettyPrinter::operator()(const IntExp& e) { ostr_ << e.value_get(); }

  void PrettyPrinter::operator()(const LetExp& e)
  {
    ostr_ << "let";
    misc::incindent(ostr_);
    misc::iendl(ostr_);

    e.chunks_get().accept(*this);

    misc::decindent(ostr_);
    misc::iendl(ostr_);
    ostr_ << "in";
    misc::incindent(ostr_);
    misc::iendl(ostr_);

    e.body_get().accept(*this);

    misc::decindent(ostr_);
    misc::iendl(ostr_);
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

    misc::incindent(ostr_);
    misc::iendl(ostr_);
    e.body_get()->accept(*this);
    misc::decindent(ostr_);
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
        ostr_ << " + ";
        break;
      case OpExp::Oper::sub:
        ostr_ << " - ";
        break;
      case OpExp::Oper::mul:
        ostr_ << " * ";
        break;
      case OpExp::Oper::div:
        ostr_ << " / ";
        break;
      case OpExp::Oper::eq:
        ostr_ << " = ";
        break;
      case OpExp::Oper::ne:
        ostr_ << " <> ";
        break;
      case OpExp::Oper::lt:
        ostr_ << " < ";
        break;
      case OpExp::Oper::le:
        ostr_ << " <= ";
        break;
      case OpExp::Oper::gt:
        ostr_ << " > ";
        break;
      case OpExp::Oper::ge:
        ostr_ << " >= ";
        break;
      }
    e.right_get().accept(*this);
  }

  void PrettyPrinter::operator()(const RecordExp& e)
  {
    ostr_ << e.type_name_get() << "{ ";

    if (e.fields_get().empty())
      {
        ostr_ << "}";
        return;
      }

    e.fields_get().at(0)->accept(*this);

    for (size_t i = 1; i < e.fields_get().size(); i++)
      {
        ostr_ << ", ";
        e.fields_get().at(i)->accept(*this);
      }

    ostr_ << " }";
  }

  void PrettyPrinter::operator()(const RecordTy& e)
  {
    ostr_ << "{ ";

    if (e.fields_get().empty())
      {
        return;
      }

    e.fields_get().at(0)->accept(*this);

    for (size_t i = 1; i < e.fields_get().size(); i++)
      {
        ostr_ << ", ";
        e.fields_get().at(i)->accept(*this);
      }

    ostr_ << " }";
    misc::iendl(ostr_);
  }

  void PrettyPrinter::operator()(const SeqExp& e)
  {
    if (e.exps_get().empty())
      {
        ostr_ << "()";
        return;
      }

    ostr_ << "(";
    misc::incindent(ostr_);

    size_t i = 0;

    for (; i < e.exps_get().size() - 1; i++)
      {
        misc::iendl(ostr_);
        e.exps_get().at(i)->accept(*this);
        ostr_ << ";";
      }

    if (i < e.exps_get().size())
      {
        misc::iendl(ostr_);
        e.exps_get().at(i)->accept(*this);
      }

    misc::decindent(ostr_);
    misc::iendl(ostr_);
    ostr_ << ")";
  }

  void PrettyPrinter::operator()(const StringExp& e)
  {
    ostr_ << "\"" << e.value_get() << "\"";
  }

  void PrettyPrinter::operator()(const TypeDec& e)
  {
    ostr_ << "type " << e.name_get() << " = ";
    e.ty_get().accept(*this);
    misc::iendl(ostr_);
  }

  void PrettyPrinter::operator()(const VarDec& e)
  {
    if (e.init_get())
      {
        ostr_ << "var " << e.name_get();
      }
    else
      {
        ostr_ << e.name_get();
      }

    if (e.type_name_get() != nullptr)
      {
        ostr_ << " : ";
        e.type_name_get()->accept(*this);
      }

    if (e.init_get())
      {
        ostr_ << " := ";
        e.init_get()->accept(*this);
      }
  }

  void PrettyPrinter::operator()(const WhileExp& e)
  {
    ostr_ << "while " << e.test_get() << " do";

    misc::incindent(ostr_);
    misc::iendl(ostr_);
    e.body_get().accept(*this);
    misc::decindent(ostr_);
  }

} // namespace ast
