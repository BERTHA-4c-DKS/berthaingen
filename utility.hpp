#ifndef _BERTHAINGEN_UTILITY_INC_
#define _BERTHAINGEN_UTILITY_INC_

#include <string>
#include <vector>

namespace berthaingen
{
  void tokenize (const std::string &, std::vector<std::string> &,
      const std::string & delimiters = " ");
  
  bool is_float (const std::string &);
  
  bool is_integer(const std::string &);

  void multispace_to_single (std::string &);
}

#endif
