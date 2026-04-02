/**
 ** \file type/pretty-printer.cc
 ** \brief Implementation for type/pretty-printer.hh.
 */

#include <type/libtype.hh>
#include <type/pretty-printer.hh>
#include <type/type.hh>
#include <type/types.hh>

namespace type
{
  namespace
  {
    template <typename Type>
    std::ostream& print_type(std::ostream& ostr, const Type& type)
    {
      PrettyPrinter printer{ostr};
      printer(type);
      return ostr;
    }

    /// How many times did we go through operator()(const Named&)?
    inline long int& indent(std::ostream& o)
    {
      // The slot to store the current indentation level.
      static const int indent_index = std::ios::xalloc();
      return o.iword(indent_index);
    }

  } // namespace

  std::ostream& operator<<(std::ostream& ostr, const Attribute& e)
  {
    return print_type(ostr, e);
  }

  std::ostream& operator<<(std::ostream& ostr, const Field& e)
  {
    return print_type(ostr, e);
  }

  std::ostream& operator<<(std::ostream& ostr, const Type& e)
  {
    return print_type(ostr, e);
  }

  PrettyPrinter::PrettyPrinter(std::ostream& ostr)
    : ostr_{ostr}
  {}

  void PrettyPrinter::operator()(const Nil&)
  {
    // Fixed: Some code was deleted here.
    // Start Fix

    ostr_ << "nil";

    // End Fix
  }

  void PrettyPrinter::operator()(const Void&) { ostr_ << "void"; }

  void PrettyPrinter::operator()(const Int&)
  {
    // FIXED: Some code was deleted here.
    // Start Fix

    ostr_ << "int";

    // End Fix
  }

  void PrettyPrinter::operator()(const String&)
  {
    // FIXED: Some code was deleted here.
    // Start Fix

    ostr_ << "string";

    // End Fix
  }

  void PrettyPrinter::operator()(const Named& e)
  {
    // FIXED: Some code was deleted here.
    // Start Fix

    ostr_ << e.name_get() << " : " << e.type_get();

    // End Fix
  }

  void PrettyPrinter::operator()(const Array& e)
  {
    // FIXME: Some code was deleted here.
  }

  void PrettyPrinter::operator()(const Record& e)
  {
    // FIXED: Some code was deleted here.
    // Start Fix

    auto it = e.begin();
    ostr_ << "{";

    if (it != e.end())
      {
        (*it).type_get().accept(*this);
        ++it;
      }

    while (it != e.end())
      {
        ostr_ << ", ";
        (*it).type_get().accept(*this);

        ++it;
      }

    ostr_ << "}";

    // End Fix
  }

  void PrettyPrinter::operator()(const Class& e)
  {
    // FIXME: Some code was deleted here.
  }

  void PrettyPrinter::operator()(const Function& e)
  {
    // FIXED: Some code was deleted here.
    // Start Fix

    ostr_ << "function_call( ";

    e.formals_get().accept(*this);

    ostr_ << ") : " << e.result_get();

    // End Fix
  }

  void PrettyPrinter::operator()(const Attribute& e)
  {
    ostr_ << e.name_get() << " : " << e.type_get();
  }

  void PrettyPrinter::operator()(const Field& e)
  {
    ostr_ << e.name_get() << " : " << e.type_get();
  }

} // namespace type
