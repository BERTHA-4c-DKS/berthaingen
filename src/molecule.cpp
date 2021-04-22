#include <fstream>
#include <sstream>
#include <iostream>

#include "molecule.hpp"
#include "utility.hpp"

using namespace berthaingen;

///////////////////////////////////////////////////////////////////////////////
//   atom public
///////////////////////////////////////////////////////////////////////////////

atom::atom(double x, double y, double z, const char * s)
{
  this->reset_ ();

  this->set_x(x);
  this->set_y(y);
  this->set_z(z);

  this->set_symbol(s);
}

void atom::translate (double x, double y, double z)
{
  x_ -= x;
  y_ -= y;
  z_ -= z;
}

///////////////////////////////////////////////////////////////////////////////
//   bond public
///////////////////////////////////////////////////////////////////////////////

bond::bond (int a1, int a2, bond_type t)
{
  set_type(t);
  set_a1(a1);
  set_a2(a2);
}

void bond::set_type(int in)
{
  switch (in)
  {
    case 0:
      type_ = bond_type::NOBOND;
      break;
    case 1:
      type_ = bond_type::SINGLE;
      break;
    case 2:
      type_ = bond_type::DOUBLE;
      break;
    case 3:
      type_ = bond_type::TRIPLE;
      break;
    default:
      type_ = bond_type::NOBOND;
      break;
  }
}

int bond::get_type() const
{
  switch (type_)
  {
    case bond_type::NOBOND:
      return 0;
    case bond_type::SINGLE:
      return 1;
    case bond_type::DOUBLE:
      return 2;
    case bond_type::TRIPLE:
      return 3;
    default:
      return 0;
  }
}


///////////////////////////////////////////////////////////////////////////////
//   molecule public
///////////////////////////////////////////////////////////////////////////////

molecule::molecule(const molecule & molec)
{
  reset_ ();

  if (this != &molec) 
    copy_ (molec);
  
  return;
}

molecule & molecule::operator= (const molecule & molec) 
{
  if (this != &molec) 
  {
    reset_();
    copy_ (molec);
  }

  return *this;
}

void molecule::center (double x, double y, double z)
{
  std::vector<atom>::iterator aiter = atoms_.begin();
  for (; aiter != atoms_.end(); ++aiter)
    aiter->translate(x, y, z);
}

void molecule::add_fragment (const molecule & mol)
{
  std::vector<atom> atomlist = mol.get_atomlist();
  std::vector<bond> bondlist = mol.get_bondlist();

  unsigned int asize = this->get_atomsize();

  std::vector<atom>::const_iterator ai = atomlist.begin();
  for (; ai != atomlist.end(); ++ai)
    this->add_atom(*ai);

  std::vector<bond>::const_iterator bi = bondlist.begin();
  for (; bi != bondlist.end(); ++bi)
  {
    bond b = *bi;
    int olda1 = b.get_a1();
    b.set_a1(olda1 + asize);

    int olda2 = b.get_a2();
    b.set_a2(olda2 + asize);

    this->add_bond(b);
  }

}

bool molecule::read_xyz_file (const char * filename, bool antoau)
{
  double converter = 1.889725988579;
  std::ifstream infile(filename);

  unsigned int num_of_atom;
  std::string molname, buf;

  if (! antoau)
    converter = 1.0;

  getline (infile, buf);

  for(int i=0; buf[i]; i++)
    if(buf[i] == ' ') 
      buf[i] = '\b';

  //std::cout << buf << std::endl;
 
  if (! is_integer (buf))
    return false;

  getline (infile, molname);

  int dim = std::stoi (buf);

  for (int i = 0; i<dim; ++i)
  {
    std::vector<std::string> tokens;

    getline (infile, buf);

    multispace_to_single(buf);

    berthaingen::tokenize (buf, tokens, " ");

    //std::vector<std::string>::iterator itt = tokens.begin();
    //for (; itt != tokens.end(); ++itt)
    //  std::cout << *itt << std::endl;

    if (tokens.size() == 4)
    {
      if (is_float(tokens[1]) && 
          is_float(tokens[2]) &&
          is_float(tokens[3]))
      {
        //std::cout << std::stod(tokens[1]) << " " <<
        //  std::stod(tokens[2]) << " " <<
        //  std::stod(tokens[3]) << std::endl;

        atom a(std::stod(tokens[1])*converter, 
            std::stod(tokens[2])*converter, 
            std::stod(tokens[3])*converter,
            tokens[0].c_str());

        add_atom(a);
      }
      else
        return false;
    }
    else if (tokens.size() == 5)
    {
      if (is_float(tokens[1]) && 
          is_float(tokens[2]) &&
          is_float(tokens[3]) &&
          is_integer(tokens[4]))
      {
        //std::cout << std::stod(tokens[1]) << " " <<
        //  std::stod(tokens[2]) << " " <<
        //  std::stod(tokens[3]) << " " << 
        //  std::stoi(tokens[4]) << std::endl;

        atom a(std::stod(tokens[1])*converter, 
            std::stod(tokens[2])*converter, 
            std::stod(tokens[3])*converter,
            tokens[0].c_str());

        a.set_charge(std::stoi(tokens[4]));

        add_atom(a);
      }
      else
        return false;
    }
    else
      return false;
  }

}

void molecule::get_xyzfile (std::ostream & os)
{
  os << this->get_atomsize() << std::endl;
  os << this->get_molname() << std::endl;
  std::vector<atom>::const_iterator aiter = this->get_atomlist().begin();
  for (; aiter != this->get_atomlist().end(); ++aiter)
    os << aiter->get_symbol() << " " 
       << aiter->get_x()  << " "
       << aiter->get_y()  << " "
       << aiter->get_z()  << " " 
       << aiter->get_charge() << std::endl;
}
 
/////////////////////////////////////////////////////////////////////////////////
//   molecule private
///////////////////////////////////////////////////////////////////////////////

void molecule::copy_ (const molecule & molec) 
{ 
  reset_();

  atoms_ = molec.atoms_;
  bonds_ = molec.bonds_;

  name_ = molec.name_;

  return;
} 
