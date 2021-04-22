#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "utility.hpp"

bool berthaingen::writefiles (const berthaingen::molecule & mol,
    const struct berthaingen::bertha_options & berthaopt,
    std::map<berthaingen::ptable::element, 
          std::vector<std::string> > & basisset_map,
    std::map<berthaingen::ptable::element, 
          std::vector<std::string> > & fitset_map, 
    std::stringstream & errmsg,
    const std::string & prefix)
{ 

  std::ofstream fout((prefix + berthaopt.bertha_in).c_str(), 
      std::ios::out | std::ios::trunc);
  
  if (fout) 
  {
    fout << "\'TYPE OF BASIS SET; 1 FOR GEOMETRIC, 2 FOR OPTIMIZED\'" << std::endl;
    fout << "2" << std::endl;
    fout << "\'NUMBER OF CENTERS\'" << std::endl;
    fout << mol.get_atomsize() << std::endl;
    
    std::vector<berthaingen::atom> atoms;
    atoms = mol.get_atomlist();
    
    unsigned int totelectron = 0;
    std::vector<berthaingen::atom>::iterator it = atoms.begin();
    for (; it != atoms.end(); ++it)
    {
      berthaingen::ptable::element e = it->get_element();
      totelectron += berthaingen::ptable::atomic_number(e) - 
        it->get_charge();
    }
    
    for (int i = 0; i < mol.get_atomsize(); ++i)
    {
      fout << "\'COORDINATES FOR CENTER " << i+1 << "\'" << std::endl;
      fout << atoms[i].get_x() << "," 
                << atoms[i].get_y() << ","
                << atoms[i].get_z() << std::endl;
      fout << "\'Z, N, MAXL AND CHARGE FOR CENTER " << i+1 << "\'" << std::endl;
      berthaingen::ptable::element e = atoms[i].get_element();
      if (basisset_map.find(e) != basisset_map.end())
      {
        std::vector<std::string>::iterator it = basisset_map[e].begin();
        fout << berthaingen::ptable::atomic_number(e) << ","
                  << berthaingen::ptable::atomic_weight(e) << ","
                  << *it << "," // MAXL
                  << atoms[i].get_charge() << std::endl;
        fout << "\'BASIS SET FOR CENTER " << i+1 << "\'" << std::endl;
        it++;
        for (; it != basisset_map[e].end(); ++it)
          fout << *it  << std::endl;
      }
      else
      {
        errmsg << "Cannot find a basis set for element : " << 
          ptable::atomic_symbol(atoms[i].get_element());
        return false;
      }
    }
    
    fout << "\'NUMBER OF CLOSED-SHELL ELECTRONS\'" << std::endl;
    fout << totelectron << ",0,0" << std::endl; 
    fout << "\'SPECIFY CLOSED AND OPEN SHELLS AND COUPLING\'" << std::endl;
    fout << "0" << std::endl;
    fout << "\'ENTER 1 FOR NEW RUN AND 0 FOR RESTART\'" << std::endl;
    fout << berthaopt.restarton << std::endl;
    fout << "\'LEVEL SHIFT FACTOR IN STAGE 0, 1, AND 2\'" << std::endl;
    fout << "-2.0,-2.0,-2.0" << std::endl;
    fout << "\'STARTING STAGE (0-2)\'" << std::endl;
    fout << "2" << std::endl;
    fout << "\'PRINT LEVEL FROM 1-2\'" << std::endl;
    fout << "2" << std::endl;
    fout << "\'DAMPING FACTOR AND RELATIVE TRESHOLD FOR INITIATION OF DAMPING\'" << std::endl;
    fout << "0.10D0,1.0D-2" << std::endl;
    fout << "\'ENTER NCORE, MACTVE,NACTVE\'" << std::endl;
    fout << totelectron << ",0,0" << std::endl;                                                
    fout << "\'ENTER GRID QUALITY FROM 1 (COURSE) to 5 (FINE)\'" << std::endl;
    fout << "5" << std::endl;
    fout << "\'EX-POTENTIAL available: LDA, B88P86,HCTH93\'" << std::endl;
    fout << "BLYP" << std::endl;
    fout << "\'Fitt\' USEFITT" << std::endl;
    fout << "2 " << berthaopt.usefitt << std::endl;
    fout << "\'scalapack\'" << std::endl;
    fout << "2 2 32 2.0" << std::endl;
    fout << "\'maxit\'" << std::endl;
    fout << "100" << std::endl;
  }
  else 
  {
    errmsg << "Error while opening file " << prefix + berthaopt.bertha_in;
    return false;
  }

  fout.close();

  std::ofstream foutf((prefix + berthaopt.fitt_in).c_str(), 
      std::ios::out | std::ios::trunc);
  
  if (foutf) 
  {
    foutf << mol.get_atomsize() << std::endl;
    
    std::vector<berthaingen::atom> atoms;
    atoms = mol.get_atomlist();
    
    for (int i = 0; i < mol.get_atomsize(); ++i)
    {
      foutf << atoms[i].get_x() << " " 
                << atoms[i].get_y() << " "
                << atoms[i].get_z() << std::endl;
      berthaingen::ptable::element e = atoms[i].get_element();
      if (fitset_map.find(e) != fitset_map.end())
      {
        std::vector<std::string>::iterator it = fitset_map[e].begin();
        for (; it != fitset_map[e].end(); ++it)
          foutf << *it << std::endl;
      }
      else
      {
        errmsg << "Cannot find a fitting set for element : " << 
          atoms[i].get_element();
        return false;
      }
    }
  }
  else 
  {
    errmsg << "Error while opening file " << prefix + berthaopt.fitt_in;
    return false;
  }

 

  foutf.close();

  return true;
}
