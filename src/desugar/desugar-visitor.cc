/**
 ** \file desugar/desugar-visitor.cc
 ** \brief Implementation of desugar::DesugarVisitor.
 */

#include <ast/all.hh>
#include <ast/libast.hh>
#include <desugar/desugar-visitor.hh>
#include <misc/algorithm.hh>
#include <misc/symbol.hh>
#include <parse/libparse.hh>
#include <parse/tweast.hh>

namespace desugar
{
  DesugarVisitor::DesugarVisitor(bool desugar_for_p, bool desugar_string_cmp_p)
    : super_type()
    , desugar_for_p_(desugar_for_p)
    , desugar_string_cmp_p_(desugar_string_cmp_p)
  {}

  /*-----------------------------.
  | Desugar string comparisons.  |
  `-----------------------------*/
  void DesugarVisitor::operator()(const ast::OpExp& e)
  {
    // FIXME: Some code was deleted here.
    if (desugar_string_cmp_p_)
      {
        result_ = new ast::CallExp(
          e.location_get(), "streq",
          new ast::exps_type{recurse(e.left_get()), recurse(e.right_get())});
      }
    else
      {
        super_type::operator()(e);
      }
  }

  /*----------------------.
  | Desugar `for' loops.  |
  `----------------------*/

  /*<<-
    Desugar `for' loops as `while' loops:

       for i := lo to hi do
         body

     is transformed as:

       let
         var _lo := lo
         var _hi := hi
         var i := _lo
       in
         if i <= _hi then
           while 1 do
             (
               body;
               if i = _hi then
                 break;
               i := i + 1
             )
       end

     Notice that:

     - a `_hi' variable is introduced so that `hi' is evaluated only
       once;

     - a `_lo' variable is introduced to prevent `i' from being in the
       scope of `_hi';

     - a first test is performed before entering the loop, so that the
       loop condition becomes `i < _hi' (instead of `i <= _hi'); this
       is done to prevent overflows on INT_MAX.
       ->>*/

  void DesugarVisitor::operator()(const ast::ForExp& e)
  {
    // FIXME: Some code was deleted here.
    // // TODO: Check if names are right (Keep old names when necessary)
    // // TODO: Fix compilation...
    // ast::ChunkList* decs = new ast::ChunkList(e.location_get());
    // ast::VarChunk* varChunk = new ast::VarChunk(e.location_get());
    // varChunk->emplace_back(
    //   *(new ast::VarDec(e.location_get(), misc::symbol("_lo"), nullptr,
    //                     e.vardec_get().init_get())));

    // varChunk->emplace_back(*(new ast::VarDec(
    //   e.location_get(), misc::symbol("_hi"), nullptr, e.hi_get())));

    // varChunk->emplace_back(*(new ast::VarDec(
    //   e.location_get(), misc::symbol("i"), nullptr,
    //   new ast::SimpleVar(e.location_get(), misc::symbol("_lo")))));

    // decs->emplace_back(varChunk);
    // result_ = new ast::LetExp(
    //   e.location_get(), decs,
    //   new ast::SeqExp(
    //     e.location_get(),
    //     new ast::exps_type(new ast::IfExp(
    //       e.location_get(),
    //       new ast::OpExp(
    //         e.location_get(),
    //         new ast::SimpleVar(e.location_get(), misc::symbol("i")),
    //         ast::OpExp::Oper::lt,
    //         new ast::SimpleVar(e.location_get(), misc::symbol("_hi"))),
    //       new ast::WhileExp(
    //         e.location_get(), new ast::IntExp(e.location_get(), 1),
    //         new ast::SeqExp(
    //           e.location_get(),
    //           new ast::exps_type(
    //             e.body_get(),
    //             new ast::IfExp(
    //               e.location_get() + e.body_get().location_get(),
    //               new ast::OpExp(
    //                 e.location_get() + e.body_get().location_get(),
    //                 new ast::SimpleVar(e.location_get()
    //                                      + e.body_get().location_get(),
    //                                    misc::symbol("i")),
    //                 ast::OpExp::Oper::eq,
    //                 new ast::SimpleVar(e.location_get()
    //                                      + e.body_get().location_get(),
    //                                    misc::symbol("_hi"))),
    //               new ast::BreakExp(e.location_get()
    //                                 + e.body_get().location_get())),
    //             new ast::AssignExp(
    //               e.location_get() + e.body_get().location_get(),
    //               new ast::SimpleVar(e.location_get()
    //                                    + e.body_get().location_get(),
    //                                  misc::symbol("i")),
    //               new ast::OpExp(
    //                 e.location_get() + e.body_get().location_get(),
    //                 new ast::SimpleVar(e.location_get()
    //                                      + e.body_get().location_get(),
    //                                    misc::symbol("i")),
    //                 ast::OpExp::Oper::add,
    //                 new ast::IntExp(e.location_get()
    //                                   + e.body_get().location_get(),
    //                                 1))))))))));

    if (!desugar_for_p_)
      {
        super_type::operator()(e);
        return;
      }

    parse::Tweast in;
    // parse::location l;
    // ast::Exp* exp = new ast::SeqExp(l, new ast::exps_type);
    // ast::Var* var = new ast::SimpleVar(l, "a");
    // ast::NameTy* namety = new ast::NameTy(l, "int");
    // ast::ChunkList* chunks = new ast::ChunkList(l);

    in << " let "
          " var _lo := "
       << recurse(e.vardec_get().init_get()) << " var _hi := " << recurse(e.hi_get())
       << " var i := _lo "
          " in "
          " if i <= _hi then "
          " while 1 do "
          " ( "
       << recurse(e.body_get())
       << "; if i = _hi then "
          " break; "
          " i := i + 1 "
          " ) "
          " end ";
    // result_ = parse::parse(in);
    result_ = std::get<ast::Exp*>(parse::parse(in));
  }

} // namespace desugar
