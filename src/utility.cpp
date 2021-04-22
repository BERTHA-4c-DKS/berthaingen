#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

//#include <boost/regex.hpp>

#include "utility.hpp"

namespace 
{
  bool both_are_spaces (char lhs, char rhs) 
  { 
    return (lhs == rhs) && (lhs == ' '); 
  }
}

bool berthaingen::set_mol12_at_dist (berthaingen::molecule & mol1, 
    berthaingen::molecule & mol2, int atom1, int atom2, 
    double dist, berthaingen::molecule & mol)
{
  if ((atom1 <= mol1.get_atomsize()) && 
      (atom2 <= mol2.get_atomsize()))
  {    
    berthaingen::atom a1 = mol1.get_atom(atom1 - 1);
    berthaingen::atom a2 = mol2.get_atom(atom2 - 1);

    // Maybe a 3D point clss ... is needed 
    double len = pow((a1.get_x() - a2.get_x()), 2.0) + 
       pow((a1.get_y() - a2.get_y()), 2.0) +
       pow((a1.get_z() - a2.get_z()), 2.0);
    len = sqrt(len);

    double xvers = (a1.get_x() - a2.get_x())/len;
    double yvers = (a1.get_y() - a2.get_y())/len;
    double zvers = (a1.get_z() - a2.get_z())/len;

    mol1.center (a1.get_x(), a1.get_y(), a1.get_z());
    mol2.center (a2.get_x(), a2.get_y(), a2.get_z());

    mol2.center (xvers*dist, yvers*dist, zvers*dist);

    mol = mol1;

    mol.add_fragment(mol2);

    return true;
  }

  return false;
}

bool berthaingen::is_float (const std::string & s) 
{
  std::istringstream iss(s);
  float f;
    
  iss >> std::noskipws >> f; 
                
  return iss.eof() && !iss.fail(); 
}

bool berthaingen::is_integer(const std::string & s)
{
  if(s.empty() || ((!isdigit(s[0])) && 
        (s[0] != '-') && (s[0] != '+'))) 
    return false ;

  char * p ;
  strtol(s.c_str(), &p, 10) ;

  return (*p == 0) ;
}

void berthaingen::tokenize (const std::string & str, 
    std::vector<std::string> & tokens,
    const std::string & delimiters)
{
  // Skip delimiters at beginning.
  std::string::size_type lastPos = 
    str.find_first_not_of(delimiters, 0);
  std::string::size_type pos = 
    str.find_first_of(delimiters, lastPos);
  
  while (std::string::npos != pos || 
      std::string::npos != lastPos)
  {
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    lastPos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, lastPos);
  }
}

void berthaingen::multispace_to_single (std::string & str)
{
  std::string::iterator new_end = 
    std::unique(str.begin(), str.end(), both_are_spaces);
  str.erase(new_end, str.end());   

  //boost::regex_replace(str, boost::regex("[' ']{2,}"), " ");
}

void berthaingen::rtrim(std::string & s, const std::string & delimiters)
{
  s.erase(s.find_last_not_of( delimiters ) + 1 );
}
 
void berthaingen::ltrim(std::string & s, const std::string & delimiters)
{
  s.erase(0, s.find_first_not_of( delimiters ) );
}

bool berthaingen::split_atom_and_basis (
    const std::vector<std::string> & setfname,
    std::map<berthaingen::ptable::element, std::vector<std::string> > & 
    set_map, std::stringstream & errmsg)
{
  std::vector<std::string>::const_iterator iter = setfname.begin();
  for (; iter != setfname.end(); ++iter)
  {
    std::vector<std::string> vctstring;
    berthaingen::tokenize (*iter, vctstring, ":");
    if (vctstring.size() == 2)
    {
      berthaingen::ptable::element e = 
        berthaingen::ptable::symbol_to_element(vctstring[0].c_str());

      if (e == berthaingen::ptable::NO_ELEMENT)
      {
        errmsg << "Error in element: " << vctstring[0];
        return false;
      }

      std::vector<std::string> lines;
      std::ifstream fp;
      fp.open(vctstring[1].c_str());
      if (fp.is_open()) 
      {
        while (!fp.eof()) 
        {
          std::string line;
          std::getline (fp, line);
          if (line.find_first_not_of("\t\n ") != std::string::npos)
          {
            multispace_to_single (line);
            ltrim(line, " \t");
            rtrim(line, " \t");

            lines.push_back(line);
          }
        }
      }
      fp.close();

      if (lines.size() == 0)
      {
        errmsg << "Error in element " << vctstring[0] <<
          " file " << vctstring[1] << "is empty ";
        return false;
      }

      if (set_map.find(e) == set_map.end())
      {
        set_map[e] = lines;
      }
      else
      {
        errmsg << "Element " << vctstring[0] << " many times present ";
        return false;
      }
    }
    else
    {
      errmsg << "Error in set: " << *iter;
      return false;
    }
  }

  return true;
}
