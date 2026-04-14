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
    result_ = new ast::CallExp(e.location_get(), "streq",
                               new ast::exps_type(e.left_get(), e.right_get()));
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
    // TODO: Check if names are right (Keep old names when necessary)
    // TODO: Fix compilation...
    ast::ChunkList* decs = new ast::ChunkList(e.location_get() + 1);
    ast::VarChunk* varChunk = new ast::VarChunk(e.location_get());
    varChunk->emplace_back(
      *(new ast::VarDec(e.location_get() + 1, misc::symbol("_lo"), nullptr,
                        e.vardec_get().init_get())));
    varChunk->emplace_back(*(new ast::VarDec(
      e.location_get() + 2, misc::symbol("_hi"), nullptr, e.hi_get())));
    varChunk->emplace_back(*(new ast::VarDec(
      e.location_get() + 3, misc::symbol("i"), nullptr,
      new ast::SimpleVar(e.location_get() + 3, misc::symbol("_lo")))));
    decs->emplace_back(varChunk);
    result_ = new ast::LetExp(
      e.location_get(), decs,
      new ast::SeqExp(
        e.location_get() + 5,
        new ast::exps_type(new ast::IfExp(
          e.location_get() + 6,
          new ast::OpExp(
            e.location_get() + 6,
            new ast::SimpleVar(e.location_get() + 6, misc::symbol("i")),
            ast::OpExp::Oper::lt,
            new ast::SimpleVar(e.location_get() + 6, misc::symbol("_hi"))),
          new ast::WhileExp(
            e.location_get() + 7, new ast::IntExp(e.location_get() + 7, 1),
            new ast::SeqExp(
              e.location_get() + 8,
              new ast::exps_type(
                e.body_get(),
                new ast::IfExp(
                  e.location_get() + 8 + e.body_get().location_get(),
                  new ast::OpExp(
                    e.location_get() + 8 + e.body_get().location_get(),
                    new ast::SimpleVar(e.location_get() + 8
                                         + e.body_get().location_get(),
                                       misc::symbol("i")),
                    ast::OpExp::Oper::eq,
                    new ast::SimpleVar(e.location_get() + 8
                                         + e.body_get().location_get(),
                                       misc::symbol("_hi"))),
                  new ast::BreakExp(e.location_get() + 9
                                    + e.body_get().location_get())),
                new ast::AssignExp(
                  e.location_get() + 10 + e.body_get().location_get(),
                  new ast::SimpleVar(e.location_get() + 10
                                       + e.body_get().location_get(),
                                     misc::symbol("i")),
                  new ast::OpExp(
                    e.location_get() + 10 + e.body_get().location_get(),
                    new ast::SimpleVar(e.location_get() + 10
                                         + e.body_get().location_get(),
                                       misc::symbol("i")),
                    ast::OpExp::Oper::add,
                    new ast::IntExp(e.location_get() + 10
                                      + e.body_get().location_get(),
                                    1))))))))));
  }

} // namespace desugar
