#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <getopt.h>
#include <math.h>

#include "molecule.hpp"
#include "utility.hpp"

#define AUTOAN 0.529177
#define ANTOAU 1.0/AUTOAN

void usages (char * name) 
{
  std::cerr << "usage: " << name << " [options] xyz" << std::endl;
  std::cerr << " -h, --help                   : display this help and exit" << std::endl;
  std::cerr << " " << std::endl;
  std::cerr << " -c, --convert-autoan         : convert a.u. coordinates to angstrom coordinates" << std::endl;
  std::cerr << " -b, --convert-antoau         : convert angstrom coordinates to a.u. coordinates" << std::endl;
  std::cerr << " -r, --center=[atomnum]       : center respect to atomnum" << std::endl;
  std::cerr << " -x, --rotate-x=[angle]       : x-axis rotation of angle (after center)" << std::endl;
  std::cerr << " -y, --rotate-y=[angle]       : y-axis rotation of angle (after center)" << std::endl;
  std::cerr << " -z, --rotate-z=[angle]       : z-axis rotation of angle (after center)" << std::endl;

  exit (1);
}

int main (int argc, char ** argv) 
{
  bool convertautoan = false, convertantoau = false;
  int atomnum = -1;

  double anglex = 0.0, angley = 0.0, anglez = 0.0;

  while (1) 
  {
    std::string inputs;

    int c, option_index;
    static struct option long_options[] = {
      {"help", 0, NULL, 'h'},
      {"convert-autoan", 0, NULL, 'c'},
      {"convert-antoau", 0, NULL, 'b'},
      {"center", 1, NULL, 'r'},
      {"rotate-x", 1, NULL, 'x'},
      {"rotate-y", 1, NULL, 'y'},
      {"rotate-z", 1, NULL, 'z'},
      {0, 0, 0, 0}
    };

    c = getopt_long (argc, argv, "hbcr:x:y:z:", long_options, &option_index);
    
    if (c == -1)
      break;

    switch (c) {
      case 'h':
        usages (argv[0]);
        break;
      case 'r':
        atomnum = atoi(optarg) - 1;
        break;
      case 'c':
        convertautoan = true;
        break;
      case 'b':
        convertantoau = true;
        break;
      case 'x':
        anglex = atof(optarg);
        break;
      default:
        usages (argv[0]);
        break;
    }
  }

  if (optind >= argc) 
    usages (argv[0]);

  if (convertantoau && convertautoan)
    usages (argv[0]);

  std::string filename = argv[optind];

  berthaingen::molecule mol;

  if (mol.read_xyz_file(filename.c_str(), false))
  {
    std::vector<double> xv, yv, zv;
    std::vector<std::string> symbol;
    std::vector<double> charge;

    if (convertantoau)
    {
      std::cout << mol.get_atomsize() <<  std::endl;
      std::cout << mol.get_molname() <<  std::endl;

      std::vector<berthaingen::atom>::const_iterator ai =
       mol.get_atoms_begin(); 
      for (; ai != mol.get_atoms_end(); ++ai)
      {
        double x = ai->get_x() * ANTOAU;
        double y = ai->get_y() * ANTOAU;
        double z = ai->get_z() * ANTOAU;

        xv.push_back(x);
        yv.push_back(y);
        zv.push_back(z);

        symbol.push_back(ai->get_symbol());
        charge.push_back(ai->get_charge());
     }
    }
    else if (convertautoan)
    {
      std::vector<berthaingen::atom>::const_iterator ai =
       mol.get_atoms_begin(); 
      for (; ai != mol.get_atoms_end(); ++ai)
      {
        double x = ai->get_x() * AUTOAN;
        double y = ai->get_y() * AUTOAN;
        double z = ai->get_z() * AUTOAN;

        xv.push_back(x);
        yv.push_back(y);
        zv.push_back(z);

        symbol.push_back(ai->get_symbol());
        charge.push_back(ai->get_charge());
      }
    }

    if (xv.size() == mol.get_atomsize())
    {
      std::cout << mol.get_atomsize() <<  std::endl;
      std::cout << mol.get_molname() <<  std::endl;
      
      double centerx = 0.0, centery = 0.0, centerz =0.0;

      if ((atomnum >= 0) && (atomnum < mol.get_atomsize()))
      {
        centerx = xv[atomnum]; 
        centery = yv[atomnum]; 
        centerz = zv[atomnum];
      }

      anglex = anglex * M_PI / 180.0;

      for (int i=0; i<mol.get_atomsize(); ++i)
      {
        double x = xv[i];
        double y = yv[i];
        double z = zv[i];

        x = x - centerx;
        y = y - centery;
        z = z - centerz;

        /* rotate x */
        y = y * cos(anglex) - z * sin(anglex);
        z = y * sin(anglex) + z * cos(anglex);

        std::cout << symbol[i] << " " 
          << x << " " 
          << y << " " 
          << z << " " 
          << charge[i] << std::endl;
      }
    }

  }
  else
  {
    std::cerr << "Error in parsing " << filename << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
