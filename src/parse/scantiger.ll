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

  // FIXED: Some code was deleted here (Define YY_USER_ACTION to update locations).
  // Start Fix

#define YY_USER_ACTION    \
        td.location_.columns(size());
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

  /* FIXED: Some code was deleted here. */
// Start Fix
whitechar       [ \t]
endofline       (\n\r|\r\n|\r|\n)
id              ([a-zA-Z][0-9a-zA-Z_]*)|_main
// End Fix

%class{
  // FIXED: Some code was deleted here (Local variables).
  std::string grown_string;
  int depth = 0;
}



%%
<SC_STRING> {
  /* rules in sub-lexer */
\"                {
                        start(INITIAL);
                        return TOKEN_VAL(STRING, grown_string);
                  }
\\x[0-9a-fA-F]{2} {
                        grown_string += strtol(text() + 2, 0, 16);
                  }
\\[0-7]{3}        {
                        long val = strtol(text() + 1, 0, 8);
                        if(val > 255)
                        {
                                td.error_ << misc::error::error_type::scan;
                                td.error_ << "Lexing Error was encountered at line" << td.location_ << "\n";
                        }
                        else
                            grown_string += val;
                   }

\\[abfnrtv]       grown_string+=text();
\\                grown_string+=text();
"\\\""            grown_string+=text();
\n                td.location_.lines(1);
.                 grown_string+=text();
}

<SC_COMMENT> {
"/*"    {
            depth+=1;
        }
"*/" {
                depth-=1;
                if(depth==0)
                  start(INITIAL);
      }
<<EOF>>         {
                     td.error_ << misc::error::error_type::scan;
                     td.error_ << "Lexing Error was encountered at line" << td.location_ << " unclosed comment\n";
                     td.error_.exit();
                }
.               {
                  continue;
                }
}

/* The rules.  */
{int}         {
                  long val = 0;
  // FIXED: Some code was deleted here (Decode, and check the value).
                  std::stringstream ss;
                  ss << str();
                  ss >> val;
                  if(val > INT_MAX){
                      td.error_ << misc::error::error_type::scan;
                      td.error_ << "Lexing Error was encountered at line" << td.location_ << "INT_MAX value\n";
                  }
                  // End Fix
                return TOKEN_VAL(INT, val);
              }
  /* FIXED: Some code was deleted here. */
  // Start Fix
"array"           return TOKEN(ARRAY);
"if"              return TOKEN(IF);
"then"            return TOKEN(THEN);
"else"            return TOKEN(ELSE);
"while"           return TOKEN(WHILE);
"for"             return TOKEN(FOR);
"to"              return TOKEN(TO);
"do"              return TOKEN(DO);
"let"             return TOKEN(LET);
"in"              return TOKEN(IN);
"end"             return TOKEN(END);
"of"              return TOKEN(OF);
"break"           return TOKEN(BREAK);
"nil"             return TOKEN(NIL);
"function"        return TOKEN(FUNCTION);
"var"             return TOKEN(VAR);
"type"            return TOKEN(TYPE);
"import"          return TOKEN(IMPORT);
"primitive"       return TOKEN(PRIMITIVE);
","               return TOKEN(COMMA);
":"               return TOKEN(COLON);
";"               return TOKEN(SEMI);
"("               return TOKEN(LPAREN);
")"               return TOKEN(RPAREN);
"["               return TOKEN(LBRACK);
"]"               return TOKEN(RBRACK);
"{"               return TOKEN(LBRACE);
"}"               return TOKEN(RBRACE);
"."               return TOKEN(DOT);
"+"               return TOKEN(PLUS);
"-"               return TOKEN(MINUS);
"*"               return TOKEN(TIMES);
"/"               return TOKEN(DIVIDE);
"="               return TOKEN(EQ);
"<>"              return TOKEN(NE);
"<"               return TOKEN(LT);
"<="              return TOKEN(LE);
">"               return TOKEN(GT);
">="              return TOKEN(GE);
"&"               return TOKEN(AND);
"|"               return TOKEN(OR);
":="              return TOKEN(ASSIGN);
{whitechar} {
                continue;
            }
{endofline} {
                  td.location_.lines(1);
                  td.location_.columns(-size());
            }
"\""        {
                  grown_string.clear();
                  start(SC_STRING);
            }
"/*"    {
                  depth = 1;
                  start(SC_COMMENT);
         }
"chunks_"         return TOKEN(CHUNKS);
"_exp"            return TOKEN(EXP);
"_lvalue"         return TOKEN(LVALUE);
"_namety"         return TOKEN(NAMETY);
"_cast"           return TOKEN(CAST);
{id}              return TOKEN_VAL(ID, text());
<<EOF>>           {
                  return TOKEN(EOF);
                  }
  // End Fix
%%
