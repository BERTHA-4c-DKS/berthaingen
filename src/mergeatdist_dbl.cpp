#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <cmath>

#include <getopt.h>

#include "molecule.hpp"
#include "utility.hpp"

void usages (char * name) 
{
  std::cerr << "usage: " << name << " [options] file1.xyz file2.xyz file3.xyz" << std::endl;
  std::cerr << " -h, --help                   : display this help and exit" << std::endl;
  std::cerr << " -a, --set-values12=\"atom1infile1:atom2infile2:d\"     " << std::endl;
  std::cerr << "                              : set atoms and distance to use" << std::endl; 
  std::cerr << " -b, --set-values23=\"atom1infile2:atom2infile3:d\"     " << std::endl;
  std::cerr << "                              : set atoms and distance to use" << std::endl; 
  std::cerr << " -c, --convert-antoau         : convert angstrom coordinate to a.u." << std::endl;
 
  exit (1);
}

int main (int argc, char ** argv) 
{
  std::vector<std::string> basisset_fname, fitset_fname,
    values12, values23;
  bool convert = false;

  struct berthaingen::bertha_options berthaopt;

  berthaopt.bertha_in = "input.inp";
  berthaopt.fitt_in = "fitt2.inp";
  berthaopt.restarton = 1;
  berthaopt.usefitt = 1;

  while (1) 
  {
    std::string inputs;

    int c, option_index;
    static struct option long_options[] = {
      {"help", 0, NULL, 'h'},
      {"set-values12", 1, NULL, 'a'},
      {"set-values23", 1, NULL, 'b'},
      {0, 0, 0, 0}
    };

    c = getopt_long (argc, argv, "hca:b:", long_options, &option_index);
    
    if (c == -1)
      break;

    switch (c) {
      case 'h':
        usages (argv[0]);
        break;
      case 'a':
        inputs.assign(optarg);
        berthaingen::tokenize (inputs, values12, ":");
        break;
      case 'b':
        inputs.assign(optarg);
        berthaingen::tokenize (inputs, values23, ":");
        break;
      case 'c':
        convert = true;
        break;
      default:
        usages (argv[0]);
        break;
    }
  }

  if ((argc - optind) != 3) 
    usages (argv[0]);

  int atom1of1, atom2of2, atom1of2, atom2of3;
  double dist12, dist23;

  if ((values12.size() == 3) && (values23.size()))
  {
    if ((! berthaingen::is_integer (values12[0])) &&
        (! berthaingen::is_integer (values12[1])) &&
        (! berthaingen::is_float (values12[2])))
    {
      std::cerr << "set-values option error in data type" << std::endl;
      return false;
    }

    if ((! berthaingen::is_integer (values23[0])) &&
        (! berthaingen::is_integer (values23[1])) &&
        (! berthaingen::is_float (values23[2])))
    {
      std::cerr << "set-values option error in data type" << std::endl;
      return false;
    }

    atom1of1 = std::stoi(values12[0]);
    atom2of2 = std::stoi(values12[1]);

    dist12 = std::stod(values12[2]);

    atom1of2 = std::stoi(values23[0]);
    atom2of3 = std::stoi(values23[1]);

    dist23 = std::stod(values23[2]);
 
  }
  else
  {
    std::cerr << "set-values option error" << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename1 = argv[optind];
  std::string filename2 = argv[optind+1];
  std::string filename3 = argv[optind+2];

  berthaingen::molecule mol1, mol2, mol3;
  bool f1 = true, f2 = true, f3 = true;

  if ((f1 = mol1.read_xyz_file(filename1.c_str(), convert)) &&
      (f2 = mol2.read_xyz_file(filename2.c_str(), convert)) &&
      (f3 = mol3.read_xyz_file(filename3.c_str(), convert)))
  {
    std::stringstream errmsg;

    /*
    std::cout << atom1of1 << " " << mol1.get_atomsize() << std::endl;
    std::cout << atom2of2 << " " << mol2.get_atomsize() << std::endl;
    std::cout << atom1of2 << " " << mol2.get_atomsize() << std::endl;
    std::cout << atom2of3 << " " << mol3.get_atomsize() << std::endl;
    */

    if ((atom1of1 <= mol1.get_atomsize()) && 
        (atom2of2 <= mol2.get_atomsize()) &&
        (atom1of2 <= mol2.get_atomsize()) &&
        (atom2of3 <= mol3.get_atomsize()))
    {    

      berthaingen::molecule mol12, mol;
      
      if (berthaingen::set_mol12_at_dist (mol1, mol2, atom1of1, atom2of2, 
            dist12, mol12))
      {
        if (berthaingen::set_mol12_at_dist (mol12, mol3, atom1of2 + mol1.get_atomsize(), 
              atom2of3, dist23, mol))
        {
          mol.get_xyzfile(std::cout);
        }
        else
        {
          std::cerr << "Error in set_mol 2" << std::endl;
          return EXIT_FAILURE;
        }
      }
      else
      {
        std::cerr << "Error in set_mol 1" << std::endl;
        return EXIT_FAILURE;
      }
    }
    else
    {
      std::cerr << "Wrong atom number" << std::endl;
      return EXIT_FAILURE;
    }
  }
  else
  {
    if (!f1)
      std::cerr << "Error in parsing " << filename1 << std::endl;

    if (!f2)
      std::cerr << "Error in parsing " << filename2 << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
