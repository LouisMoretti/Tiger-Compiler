                                                                // -*- C++ -*-
%require "3.8"
%language "c++"
// Set the namespace name to `parse', instead of `yy'.
%define api.prefix {parse}
%define api.namespace {parse}
%define api.parser.class {parser}
%define api.value.type variant
%define api.token.constructor

// We use a GLR parser because it is able to handle Shift-Reduce and
// Reduce-Reduce conflicts by forking and testing each way at runtime. GLR
// permits, by allowing few conflicts, more readable and maintainable grammars.
%glr-parser
%skeleton "glr2.cc"

// In TC, we expect the GLR to resolve one Shift-Reduce and zero Reduce-Reduce
// conflict at runtime. Use %expect and %expect-rr to tell Bison about it.
// FIXED: Some code was deleted here (Other directives).
// Start Fix
%expect 0
%expect-rr 0
// End Fix

%define parse.error verbose
%defines
%debug
// Prefix all the tokens with TOK_ to avoid colisions.
%define api.token.prefix {TOK_}

/* We use pointers to store the filename in the locations.  This saves
   space (pointers), time (no deep copy), but leaves the problem of
   deallocation.  This would be a perfect job for a misc::symbol
   object (passed by reference), however Bison locations require the
   filename to be passed as a pointer, thus forcing us to handle the
   allocation and deallocation of this object.

   Nevertheless, all is not lost: we can still use a misc::symbol
   object to allocate a flyweight (constant) string in the pool of
   symbols, extract it from the misc::symbol object, and use it to
   initialize the location.  The allocated data will be freed at the
   end of the program (see the documentation of misc::symbol and
   misc::unique).  */
%define api.filename.type {const std::string}
%locations

/*---------------------.
| Support for tokens.  |
`---------------------*/
%code requires
{
#include <string>
#include <misc/algorithm.hh>
#include <misc/separator.hh>
#include <misc/symbol.hh>
#include <parse/fwd.hh>

  // Pre-declare parse::parse to allow a ``reentrant'' parsing within
  // the parser.
  namespace parse
  {
    ast_type parse(Tweast& input);
  }
}

// The parsing context.
%param { ::parse::TigerDriver& td }
%parse-param { ::parse::Lexer& lexer }

%printer { yyo << $$; } <int> <std::string> <misc::symbol>;

%token <std::string>    STRING "string"
%token <misc::symbol>   ID     "identifier"
%token <int>            INT    "integer"


/*--------------------------------.
| Support for the non-terminals.  |
`--------------------------------*/

%code requires
{
# include <ast/fwd.hh>
// Provide the declarations of the following classes for the
// %destructor clauses below to work properly.
# include <ast/exp.hh>
# include <ast/var.hh>
# include <ast/ty.hh>
# include <ast/name-ty.hh>
# include <ast/field.hh>
# include <ast/field-init.hh>
# include <ast/function-dec.hh>
# include <ast/type-dec.hh>
# include <ast/var-dec.hh>
# include <ast/chunk.hh>
# include <ast/chunk-list.hh>
}

  // FIXME: Some code was deleted here (Printers and destructors).

/*-----------------------------------------.
| Code output in the implementation file.  |
`-----------------------------------------*/

%code
{
# include <parse/tweast.hh>
# include <misc/separator.hh>
# include <misc/symbol.hh>
# include <ast/all.hh>
# include <ast/libast.hh>
# include <parse/tiger-factory.hh>

  namespace
  {

    /// Get the metavar from the specified map.
    template <typename T>
    T*
    metavar(parse::TigerDriver& td, unsigned key)
    {
      parse::Tweast* input = td.input_;
      return input->template take<T>(key);
    }

  }
}

%code
{
  #include <parse/scantiger.hh>  // header file generated with reflex --header-file
  #undef yylex
  #define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global lex()
}

// Definition of the tokens, and their pretty-printing.
%token AND          "&"
       ARRAY        "array"
       ASSIGN       ":="
       BREAK        "break"
       CAST         "_cast"
       CLASS        "class"
       COLON        ":"
       COMMA        ","
       DIVIDE       "/"
       DO           "do"
       DOT          "."
       ELSE         "else"
       END          "end"
       EQ           "="
       EXTENDS      "extends"
       FOR          "for"
       FUNCTION     "function"
       GE           ">="
       GT           ">"
       IF           "if"
       IMPORT       "import"
       IN           "in"
       LBRACE       "{"
       LBRACK       "["
       LE           "<="
       LET          "let"
       LPAREN       "("
       LT           "<"
       MINUS        "-"
       METHOD       "method"
       NE           "<>"
       NEW          "new"
       NIL          "nil"
       OF           "of"
       OR           "|"
       PLUS         "+"
       PRIMITIVE    "primitive"
       RBRACE       "}"
       RBRACK       "]"
       RPAREN       ")"
       SEMI         ";"
       THEN         "then"
       TIMES        "*"
       TO           "to"
       TYPE         "type"
       VAR          "var"
       WHILE        "while"
       EOF 0        "end of file"

%type <ast::Exp*>             exp
%type <ast::ChunkList*>       chunks

%type <ast::TypeChunk*>       tychunk
%type <ast::TypeDec*>         tydec
%type <ast::NameTy*>          typeid
%type <ast::Ty*>              ty

%type <ast::Field*>           tyfield
%type <ast::fields_type*>     tyfields tyfields.1
// FIXED: Some code was deleted here (More %types).
// Start Fix
%type <ast::exps_type*>       exps
%type <ast::fieldinits_type*> record_attr
%type <ast::exps_type*>       func_prms
%type <ast::Var*>             lvalue lvalue.big

%type <ast::FunctionChunk*>   funcdec
%type <ast::VarChunk*>        vardec
// End Fix

// FIXED: Some code was deleted here (Priorities/associativities).

// Solving conflicts on:
// let type foo = bar
//     type baz = bat
// which can be understood as a list of two TypeChunk containing
// a unique TypeDec each, or a single TypeChunk containing two TypeDec.
// We want the latter.
%precedence CHUNKS
%precedence TYPE
// FIXED: Some code was deleted here (Other declarations).
// Start Fix


%precedence THEN
%precedence ELSE DO OF
%left OR
%left AND
%nonassoc GE LE EQ NE LT GT
%left PLUS MINUS
%left TIMES DIVIDE
%precedence UMINUS
%precedence ASSIGN
// End Fix

%start program

%%
program:
  /* Parsing a source program.  */
  exp
   { td.ast_ = $1; }
| /* Parsing an imported file.  */
  chunks
   { td.ast_ = $1; }
;
// Start Fix
exps:
  %empty
    { $$ = make_exps_type(); }
  | exp
    { $$ = make_exps_type($1); }
  | exp[left] ";" exps[right]
    { $$ = $right; $$->insert($$->begin(), $left); }
;

record_attr:
  ID[name] "=" exp[value] "," record_attr[attributes]
    { $$ = $attributes; $$->insert($$->begin(), make_FieldInit(@$, $name, $value)); }
  | ID[name] "=" exp[value]
    { $$ = make_fieldinits_type(); $$->insert($$->begin(), make_FieldInit(@$, $name, $value)); }

func_prms:
  exp[value] "," func_prms[prms]
    { $$ = $prms; $$->insert($$->begin(), $value); }
  | exp[value]
    { $$ = make_exps_type($value); }
// End Fix

%token EXP "_exp";
exp:
  INT
    { $$ = make_IntExp(@$, $1); }
  // FIXED: Some code was deleted here (More rules).
  // Start Fix
  | STRING 
    { $$ = make_StringExp(@$, $1); }
  | "nil"
    { $$ = make_NilExp(@$); }
  | ID[type] "[" exp[size] "]" "of" exp[init]
    { $$ = make_ArrayExp(@$, make_NameTy(@$, $type), $size, $init); }
  | ID[name] "{" record_attr[attributes] "}"
    { $$ = make_RecordExp(@$, make_NameTy(@$, $name), $attributes); }
  | ID[name] "{" "}"
    { $$ = make_RecordExp(@$, make_NameTy(@$, $name), make_fieldinits_type()); }
  | lvalue
    { $$ = $1; }
  | ID[name] "(" func_prms[prms] ")"
    { $$ = make_CallExp(@$, $name, $prms);}
  | ID[name] "(" ")"
    { $$ = make_CallExp(@$, $name, make_exps_type());}
  | "-" exp[right] %prec UMINUS
    { $$ = make_OpExp(@$, make_IntExp(@$, 0), ast::OpExp::Oper::sub, $right); }
  | exp[left] "*" exp[right]
    { $$ = make_OpExp(@$, $left, ast::OpExp::Oper::mul, $right); }
  | exp[left] "/" exp[right]
    { $$ = make_OpExp(@$, $left, ast::OpExp::Oper::div, $right); }
  | exp[left] "+" exp[right]
    { $$ = make_OpExp(@$, $left, ast::OpExp::Oper::add, $right); }
  | exp[left] "-" exp[right]
    { $$ = make_OpExp(@$, $left, ast::OpExp::Oper::sub, $right); }
  | exp[left] ">=" exp[right]
    { $$ = make_OpExp(@$, $left, ast::OpExp::Oper::ge, $right); }
  | exp[left] "<=" exp[right]
    { $$ = make_OpExp(@$, $left, ast::OpExp::Oper::le, $right); }
  | exp[left] "<>" exp[right]
    { $$ = make_OpExp(@$, $left, ast::OpExp::Oper::ne, $right); }
  | exp[left] "=" exp[right]
    { $$ = make_OpExp(@$, $left, ast::OpExp::Oper::eq, $right); }
  | exp[left] "<" exp[right]
    { $$ = make_OpExp(@$, $left, ast::OpExp::Oper::lt, $right); }
  | exp[left] ">" exp[right]
    { $$ = make_OpExp(@$, $left, ast::OpExp::Oper::gt, $right); }
  | exp[left] "&" exp[right]
    { $$ = make_IfExp(@$, $left, $right, make_StringExp(@$, "1 = 2")); }
  | exp[left] "|" exp[right]
    { $$ = make_IfExp(@$, $left, make_StringExp(@$, "1 = 1"), $right); }
  | "(" exps[expressions] ")"
    { $$ = make_SeqExp(@$, $expressions); }
  | lvalue[var] ":=" exp[value]
    { $$ = make_AssignExp(@$, $var, $value); }
  | "if" exp[condition] "then" exp[body] "else" exp[else]
    { $$ = make_IfExp(@$, $condition, $body, $else); }
  | "if" exp[condition] "then" exp[body]
    { $$ = make_IfExp(@$, $condition, $body); }
  | "while" exp[condition] "do" exp[body]
    { $$ = make_WhileExp(@$, $condition, $body); }
  | "for" ID[id] ":=" exp[init] "to" exp[stop] "do" exp[body]
    { $$ = make_ForExp(@$, make_VarDec(@$, $id, nullptr, $init), $stop, $body); }
  | "break"
    { $$ = make_BreakExp(@$); }
  | "let" chunks[decs] "in" exps[body] "end"
    { $$ = make_LetExp(@$, $decs, make_SeqExp(@$, $body)); }
  | EXP "(" INT[id] ")"
    { $$ = metavar<ast::Exp>(td, $id); }
;
// End Fix

// Start Fix
%token LVALUE "_lvalue";
lvalue:
  ID[name]
    { $$ = make_SimpleVar(@$, $name); }
  | lvalue.big
  | LVALUE "(" INT[id] ")"
    { $$ = metavar<ast::Var>(td, $id); }
;

lvalue.big:
  ID[var] "." ID[attr]
    { $$ = make_FieldVar(@$, make_SimpleVar(@$, $var), $attr); }
  | ID[var] "[" exp[index] "]"
    { $$ = make_SubscriptVar(@$, make_SimpleVar(@$, $var), $index); }
  | lvalue.big[var] "." ID[attr]
    { $$ = make_FieldVar(@$, $var, $attr); }
  | lvalue.big[var] "[" exp[index] "]"
    { $$ = make_SubscriptVar(@$, $var, $index); }
;
// End fix

/*---------------.
| Declarations.  |
`---------------*/

%token CHUNKS "_chunks";
chunks:
  /* Chunks are contiguous series of declarations of the same type
     (TypeDec, FunctionDec...) to which we allow certain specfic behavior like
     self referencing.
     They are held by a ChunkList, that can be empty, like in this case:
        let
        in
            ..
        end
     which is why we end the recursion with a %empty. */
  %empty                  { $$ = make_ChunkList(@$); }
| tychunk   chunks        { $$ = $2; $$->push_front($1); }
  // FIXED: Some code was deleted here (More rules).
  // Start Fix
| funcdec chunks
  { $$ = $2; $$->push_front($1); }
| vardec chunks
  { $$ = $2; $$->push_front($1); }
| CHUNKS[meta] "(" INT[id] ")" chunks[list]
  { $$ = $list; $$->push_front(metavar<ast::ChunkInterface>(td, $id)); }
;
// End Fix

// Start Fix
funcdec:
  "function" ID[name] "(" tyfields[fields] ")" ":" typeid[type] "=" exp[body]
    {
      $$ = make_FunctionChunk(@$); auto varchunk = make_VarChunk(@$); 
      for (auto field: *$fields) {
        varchunk->push_front(*make_VarDec(@$, field->name_get(), make_NameTy(@$, field->name_get()), nullptr));
      }
      $$->push_front(*make_FunctionDec(@$, $name, varchunk, $type, $body)); }
  | "function" ID[name] "(" tyfields[fields] ")" "=" exp[body]
    { $$ = make_FunctionChunk(@$); auto varchunk = make_VarChunk(@$);
      for (auto field: *$fields) {
        varchunk->push_front(*make_VarDec(@$, field->name_get(), make_NameTy(@$, field->name_get()), nullptr));
      }
      $$->push_front(*make_FunctionDec(@$, $name, varchunk, nullptr, $body)); }
  | "primitive" ID[name] "(" tyfields[fields] ")" ":" typeid[type]
    { $$ = make_FunctionChunk(@$); auto varchunk = make_VarChunk(@$);
      for (auto field: *$fields) {
        varchunk->push_front(*make_VarDec(@$, field->name_get(), make_NameTy(@$, field->name_get()), nullptr));
      }
      $$->push_front(*make_FunctionDec(@$, $name, varchunk, $type, nullptr)); }
  | "primitive" ID[name] "(" tyfields[fields] ")"
    { $$ = make_FunctionChunk(@$); auto varchunk = make_VarChunk(@$);
      for (auto field: *$fields) {
        varchunk->push_front(*make_VarDec(@$, field->name_get(), make_NameTy(@$, field->name_get()), nullptr));
      }
      $$->push_front(*make_FunctionDec(@$, $name, varchunk, nullptr, nullptr)); }
;

vardec:
  "var" ID[name] ":" typeid[type] ":=" exp[value]
    { $$ = make_VarChunk(@$); $$->push_front(*make_VarDec(@$, $name, $type, $value)); }
  | "var" ID[name] ":=" exp[value]
    { $$ = make_VarChunk(@$); $$->push_front(*make_VarDec(@$, $name, nullptr, $value)); }
;
// End Fix

/*--------------------.
| Type Declarations.  |
`--------------------*/

tychunk:
  /* Use `%prec CHUNKS' to do context-dependent precedence and resolve a
     shift-reduce conflict. */
  tydec %prec CHUNKS  { $$ = make_TypeChunk(@1); $$->push_front(*$1); }
| tydec tychunk       { $$ = $2; $$->push_front(*$1); }
;

tydec:
  "type" ID "=" ty { $$ = make_TypeDec(@$, $2, $4); }
;

ty:
  typeid               { $$ = $1; }
| "{" tyfields "}"     { $$ = make_RecordTy(@$, $2); }
| "array" "of" typeid  { $$ = make_ArrayTy(@$, $3); }
;

tyfields:
  %empty               { $$ = make_fields_type(); }
| tyfields.1           { $$ = $1; }
;

tyfields.1:
  tyfields.1 "," tyfield { $$ = $1; $$->emplace_back($3); }
| tyfield                { $$ = make_fields_type($1); }
;

tyfield:
  ID ":" typeid     { $$ = make_Field(@$, $1, $3); }
;

%token NAMETY "_namety";
typeid:
  ID                    { $$ = make_NameTy(@$, $1); }
  /* This is a metavariable. It it used internally by TWEASTs to retrieve
     already parsed nodes when given an input to parse. */
| NAMETY "(" INT ")"    { $$ = metavar<ast::NameTy>(td, $3); }
;

%%

void
parse::parser::error(const location_type& l, const std::string& m)
{
  // FIXED: Some code was deleted here.
  td.error_ << misc::error::error_type::parse;
  td.error_ << "Parsing Error was encountered at line " << l << ": " << m << std::endl;
}
