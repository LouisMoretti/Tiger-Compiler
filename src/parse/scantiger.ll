                                                            /* -*- C++ -*- */
// %option defines the parameters with which the reflex will be launched
%option noyywrap
// To enable compatibility with bison
%option bison-complete
%option bison-cc-parser=parser
%option bison_cc_namespace=parse
%option bison-locations

%option lex=lex
// Add a param of function lex() generate in Lexer class
%option params="::parse::TigerDriver& td"
%option namespace=parse
// Name of the class generate by reflex
%option lexer=Lexer

%top{

#define YY_EXTERN_C extern "C" // For linkage rule

#include <cerrno>
#include <climits>
#include <regex>
#include <string>
#include <limits.h>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <misc/contract.hh>
  // Using misc::escape is very useful to quote non printable characters.
  // For instance
  //
  //    std::cerr << misc::escape('\n') << '\n';
  //
  // reports about `\n' instead of an actual new-line character.
#include <misc/escape.hh>
#include <misc/symbol.hh>
#include <parse/parsetiger.hh>
#include <parse/tiger-driver.hh>

  // FIXME: Some code was deleted here (Define YY_USER_ACTION to update locations).
// Start Fix
#define YY_USER_ACTION \
    td.location+=size();
// End Fix

#define TOKEN(Type)                             \
  parser::make_ ## Type(td.location_)

#define TOKEN_VAL(Type, Value)                  \
  parser::make_ ## Type(Value, td.location_)

# define CHECK_EXTENSION()                              \
  do {                                                  \
    if (!td.enable_extensions_p_)                       \
      td.error_ << misc::error::error_type::scan        \
                << td.location_                         \
                << ": invalid identifier: `"            \
                << misc::escape(text()) << "'\n";       \
  } while (false)


%}

%x SC_COMMENT SC_STRING

/* Abbreviations.  */
int             [0-9]+
  /* FIXME: Some code was deleted here. */

%class{
  // FIXME: Some code was deleted here (Local variables).
}

%%
/* The rules.  */
{int}         {
                // Start Fix
                long val = 0;
  // FIXED: Some code was deleted here (Decode, and check the value).
                stringstream ss;
                ss << str();
                ss >> val;
                if(val > INT_MAX){
                    td.error_ << misc::error::error_type::scan;
                    td.error_ << "Lexing Error was encountered" at line << td.location_ << "\n";
                }
                // End Fix
                return TOKEN_VAL(INT, val);
              }

  /* FIXME: Some code was deleted here. */
  // Start Fix
  "array"     { return TOKEN(ARRAY); }
  "if"        { return TOKEN(IF); }
  "then"      { return TOKEN(THEN); }
  "else"      { return TOKEN(ELSE); }
  "while"     { return TOKEN(WHILE); }
  "for"     { return TOKEN(FOR); }
  "to"     { return TOKEN(TO); }
  "do"     { return TOKEN(DO); }
  "let"     { return TOKEN(LET); }
  "in"     { return TOKEN(IN); }
  "end"     { return TOKEN(END); }
  "of"     { return TOKEN(OF); }
  "break"     { return TOKEN(BREAK); }
  "nil"     { return TOKEN(NIL); }
  "function"     { return TOKEN(FUNCTION); }
  "var" { return TOKEN(VAR); }
  "type"     { return TOKEN(TYPE); }
  "import"     { return TOKEN(IMPORT); }
  "primitive"     { return TOKEN(PRIMTIVE); }
  // End Fix
%%



