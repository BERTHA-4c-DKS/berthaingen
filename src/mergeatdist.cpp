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
  std::cerr << "usage: " << name << " [options] file1.xyz file2.xyz" << std::endl;
  std::cerr << " -h, --help                   : display this help and exit" << std::endl;
  std::cerr << " -a, --set-values=\"atom1infile1:atom2infile2:d\"     " << std::endl;
  std::cerr << "                              : set atoms and distance to use" << std::endl;  
  std::cerr << " -c, --convert-antoau         : convert angstrom coordinate to a.u." << std::endl;
 
  exit (1);
}

int main (int argc, char ** argv) 
{
  std::vector<std::string> basisset_fname, fitset_fname,
    values;
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
      {"set-values", 1, NULL, 'a'},
      {0, 0, 0, 0}
    };

    c = getopt_long (argc, argv, "hca:", long_options, &option_index);
    
    if (c == -1)
      break;

    switch (c) {
      case 'h':
        usages (argv[0]);
        break;
      case 'a':
        inputs.assign(optarg);
        berthaingen::tokenize (inputs, values, ":");
        break;
      case 'c':
        convert = true;
        break;
      default:
        usages (argv[0]);
        break;
    }
  }

  if ((argc - optind) != 2) 
    usages (argv[0]);

  int atom1, atom2;
  double dist;

  if (values.size() == 3)
  {
    if ((! berthaingen::is_integer (values[0])) &&
        (! berthaingen::is_integer (values[1])) &&
        (! berthaingen::is_float (values[2])))
    {
      std::cerr << "set-values option error in data type" << std::endl;
      return false;
    }

    atom1 = std::stoi(values[0]);
    atom2 = std::stoi(values[1]);

    dist = std::stod(values[2]);
  }
  else
  {
    std::cerr << "set-values option error" << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename1 = argv[optind];
  std::string filename2 = argv[optind+1];

  berthaingen::molecule mol, mol1, mol2;
  bool f1 = true, f2 = true;

  if ((f1 = mol1.read_xyz_file(filename1.c_str(), convert)) &&
      (f2 = mol2.read_xyz_file(filename2.c_str(), convert)))
  {
    std::stringstream errmsg;
    
    if (berthaingen::set_mol12_at_dist (mol1, mol2, atom1, atom2, 
            dist, mol))
    {
      mol.get_xyzfile(std::cout);
    }
    else
    {
      std::cerr << "Wrtong atom number" << std::endl;
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
