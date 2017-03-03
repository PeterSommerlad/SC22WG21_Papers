#include "spanstream.h"

// instantiate templates to sidestep multiple instantiations
namespace std{ namespace experimental{
   template class basic_spanbuf<char>;
   template class basic_ispanstream<char>;
   template class basic_ospanstream<char>;
   template class basic_spanstream<char>;

   template class basic_spanbuf<wchar_t>;
   template class basic_ispanstream<wchar_t>;
   template class basic_ospanstream<wchar_t>;
   template class basic_spanstream<wchar_t>;
}}



