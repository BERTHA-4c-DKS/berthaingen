#ifndef _BERTHAINGEN_UTILITY_INC_
#define _BERTHAINGEN_UTILITY_INC_

#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "molecule.hpp"

namespace berthaingen
{
  struct bertha_options {
    std::string bertha_in, fitt_in;
    int restarton, usefitt;
  };

  void tokenize (const std::string &, std::vector<std::string> &,
      const std::string & delimiters = " ");
  
  bool is_float (const std::string &);
  
  bool is_integer(const std::string &);

  void multispace_to_single (std::string &);
  
  void rtrim(std::string &, const std::string & delimiters = " \f\n\r\t\v");

  void ltrim(std::string &, const std::string & delimiters = " \f\n\r\t\v");
  
  bool split_atom_and_basis (const std::vector<std::string> &,
      std::map<berthaingen::ptable::element, std::vector<std::string> > &,
      std::stringstream & errmsg);

  bool writefiles (const molecule &, const struct bertha_options &,
      std::map<ptable::element, std::vector<std::string> > &,
      std::map<ptable::element, std::vector<std::string> > &,
      std::stringstream &, const std::string & prefix = "");
  
  bool set_mol12_at_dist (berthaingen::molecule &, 
    berthaingen::molecule &, int, int, 
    double, berthaingen::molecule &);
}
#endif
