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
  std::cerr << " -b, --basis-set=\"asymbol1:filename1;...;asymbolN:filenameN\"    " << std::endl;
  std::cerr << "                              : mandatory specify basisset filenames and atoms" << std::endl; 
  std::cerr << " -f, --fit-set=\"asymbol1:filename1;...;asymbolN:filenameN\"     " << std::endl;
  std::cerr << "                              : mandatory specify fitset filenames and atoms" << std::endl;  
  std::cerr << " -a, --set-values=\"atom1infile1:atom2infile2:dmin:dmax:dr\"     " << std::endl;
  std::cerr << "                              : set atoms and distances to use" << std::endl;  
  std::cerr << " " << std::endl;
  std::cerr << " -o, --out-inputfname=\"filename\"     " << std::endl;
  std::cerr << "                              : specify bertha input filename (default: input.inp" << std::endl;  
  std::cerr << " -O, --out-fittfname=\"filename\"     " << std::endl;
  std::cerr << "                              : specify bertha fitting input filename (default: fitt2.inp" << std::endl;  
  std::cerr << " -c, --convert-antoau         : convert angstrom coordinate to a.u." << std::endl;
  std::cerr << " -r, --restart-on             : set restart mode on" << std::endl;
  std::cerr << " -t, --usefitt-off            : set usefitt mode off " << std::endl;
 
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
      {"basis-set", 1, NULL, 'b'},
      {"fit-set", 1, NULL, 'f'},
      {"out-inputfname", 1, NULL, 'o'},
      {"out-fittfname", 1, NULL, 'O'},
      {"convert-antoau", 0, NULL, 'c'},
      {"restart-on", 0, NULL, 'r'},
      {"usefitt-off", 0, NULL, 't'},
      {"set-values", 1, NULL, 'a'},
      {0, 0, 0, 0}
    };

    c = getopt_long (argc, argv, "htrcb:f:o:O:a:", long_options, &option_index);
    
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
      case 't':
        berthaopt.usefitt = 0;
        break;
      case 'r':
        berthaopt.restarton = 0;
        break;
      case 'c':
        convert = true;
        break;
      case 'o':
        berthaopt.bertha_in.assign(optarg);
        break;
      case 'O':
        berthaopt.fitt_in.assign(optarg);
        break;
      case 'b':
        inputs.assign(optarg);
        berthaingen::tokenize (inputs, basisset_fname, ";");
        break;
      case 'f':
        inputs.assign(optarg);
        berthaingen::tokenize (inputs, fitset_fname, ";");
        break;
      default:
        usages (argv[0]);
        break;
    }
  }

  if ((argc - optind) != 2) 
    usages (argv[0]);

  int atom1, atom2, nstep;
  double dmin, dmax, dr;

  if (values.size() == 5)
  {
    if ((! berthaingen::is_integer (values[0])) &&
        (! berthaingen::is_integer (values[1])) &&
        (! berthaingen::is_float (values[2])) &&
        (! berthaingen::is_float (values[3])) &&
        (! berthaingen::is_float (values[4])))
    {
      std::cerr << "set-values option error in data type" << std::endl;
      return false;
    }

    atom1 = std::stoi(values[0]);
    atom2 = std::stoi(values[1]);

    dmin = std::stod(values[2]);
    dmax = std::stod(values[3]);
    dr = std::stod(values[4]);

    nstep = (int) ((dmax-dmin)/dr) + 1;
  }
  else
  {
    std::cerr << "set-values option error" << std::endl;
    return EXIT_FAILURE;
  }

  if (basisset_fname.size() == 0)
  {
    std::cerr << "Need to specify the basis set " << std::endl;
    return EXIT_FAILURE; 
  }

  if (fitset_fname.size() == 0)
  {
    std::cerr << "Need to specify the fitting set " << std::endl;
    return EXIT_FAILURE;
  }

  std::map<berthaingen::ptable::element, std::vector<std::string> > 
    basisset_map;
  std::stringstream errmsg;
  if (! split_atom_and_basis (basisset_fname, basisset_map, errmsg))
  {
    std::cerr << errmsg.str() << std::endl;
    return EXIT_FAILURE;
  }

  std::map<berthaingen::ptable::element, std::vector<std::string> > 
    fitset_map;
  if (! split_atom_and_basis (fitset_fname, fitset_map, errmsg))
  {
    std::cerr << errmsg.str() << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename1 = argv[optind];
  std::string filename2 = argv[optind+1];

  berthaingen::molecule mol1, mol2;
  bool f1 = true, f2 = true;

  if ((f1 = mol1.read_xyz_file(filename1.c_str(), convert)) &&
      (f2 = mol2.read_xyz_file(filename2.c_str(), convert)))
  {
    std::stringstream errmsg;

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

      for (int i = 0; i<nstep; ++i)
      {
        std::stringstream prefix;

        prefix << i << "_";

        double val = dmin + (double) i * dr;
        mol2.center (xvers*val, yvers*val, zvers*val);

        std::cout << val << std::endl;

        berthaingen::molecule mol = mol1;

        mol.add_fragment(mol2);

        //std::cout << mol << std::endl;

        std::stringstream errmsg;
        
        if (! writefiles (mol, berthaopt, basisset_map, fitset_map, errmsg,
              prefix.str()))
        {
          std::cerr << errmsg.str() << std::endl;
          return EXIT_FAILURE;
        }

        mol2.center (-1.0*xvers*val, -1.0*yvers*val, -1.0*zvers*val);
      }
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
