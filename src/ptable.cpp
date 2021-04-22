#include "ptable.hpp"

#include <strings.h>

using namespace berthaingen;

namespace 
{
  struct edata {
    int anumber;
    double weight;
    char name[15];
    char symbol[4];
  };

  static const edata elements[ptable::NO_ELEMENT + 1] = {
    {1   , 1.00811       , "Hydrogen",      "H" },
    {2   , 4.002602      , "Helium",        "He"},
    {3   , 6.997         , "Lithium",       "Li"},   
    {4   , 9.0121831     , "Beryllium",     "Be"},   
    {5   , 10.821        , "Boron",         "B" },   
    {6   , 12.0116       , "Carbon",        "C" },   
    {7   , 14.00728      , "Nitrogen",      "N" },   
    {8   , 15.99977      , "Oxygen",        "O" },   
    {9   , 18.998403163  , "Fluorine",      "F" },   
    {10  , 20.1797       , "Neon",          "Ne"},   
    {11  , 22.98976928   , "Sodium",        "Na"},   
    {12  , 24.307        , "Magnesium",     "Mg"},   
    {13  , 26.9815385    , "Aluminum",      "Al"},   
    {14  , 28.086        , "Silicon",       "Si"},   
    {15  , 30.973761998  , "Phosphorus",    "P" },   
    {16  , 32.076        , "Sulfur",        "S" },   
    {17  , 35.457        , "Chlorine",      "Cl"},   
    {18  , 39.948        , "Argon",         "Ar"},   
    {19  , 39.0983       , "Potassium",     "K" },   
    {20  , 40.078        , "Calcium",       "Ca"},   
    {21  , 44.955908     , "Scandium",      "Sc"},   
    {22  , 47.867        , "Titanium",      "Ti"},   
    {23  , 50.9415       , "Vanadium",      "V" },   
    {24  , 51.9961       , "Chromium",      "Cr"},   
    {25  , 54.938044     , "Manganese",     "Mn"},   
    {26  , 55.845        , "Iron",          "Fe"},   
    {27  , 58.933194     , "Cobalt",        "Co"},   
    {28  , 58.6934       , "Nickel",        "Ni"},   
    {29  , 63.546        , "Copper",        "Cu"},   
    {30  , 65.38         , "Zinc",          "Zn"},   
    {31  , 69.723        , "Gallium",       "Ga"}, 
    {32  , 72.630        , "Germanium",     "Ge"}, 
    {33  , 74.921595     , "Arsenic",       "As"}, 
    {34  , 78.971        , "Selenium",      "Se"}, 
    {35  , 79.907        , "Bromine",       "Br"}, 
    {36  , 83.798        , "Krypton ",      "Kr"}, 
    {37  , 85.4678       , "Rubidium",      "Rb"},   
    {38  , 87.62         , "Strontium",     "Sr"},   
    {39  , 88.90584      , "Yttrium",       "Y" },   
    {40  , 91.224        , "Zirconium",     "Zr"},   
    {41  , 92.90637      , "Niobium",       "Nb"},   
    {42  , 95.95         , "Molybdenum",    "Mo"},   
    {43  , 98.0          , "Technetium",    "Tc"},   
    {44  , 101.07        , "Ruthenium",     "Ru"},   
    {45  , 102.90550     , "Rhodium",       "Rh"},   
    {46  , 106.42        , "Palladium",     "Pd"},   
    {47  , 107.8682      , "Silver",        "Ag"},   
    {48  , 112.414       , "Cadmium",       "Cd"},   
    {49  , 114.818       , "Indium",        "In"}, 
    {50  , 118.710       , "Tin",           "Sn"}, 
    {51  , 121.760       , "Antimony",      "Sb"}, 
    {52  , 127.60        , "Tellurium",     "Te"}, 
    {53  , 126.90447     , "Iodine",        "I" }, 
    {54  , 131.293       , "Xenon",         "Xe"}, 
    {55  , 132.90545196  , "Cesium",        "Cs"},   
    {56  , 137.327       , "Barium",        "Ba"},   
    {57  , 138.90547     , "Lanthanum",     "La"},   
    {58  , 140.116       , "Cerium",        "Ce"},
    {59  , 140.90766     , "Praseodymium",  "Pr"},   
    {60  , 144.242       , "Neodymium",     "Nd"},   
    {61  , 145.0         , "Promethium",    "Pm"},   
    {62  , 150.36        , "Samarium",      "Sm"},   
    {63  , 151.964       , "Europium",      "Eu"},   
    {64  , 157.25        , "Gadolinium",    "Gd"},
    {65  , 158.92535     , "Terbium",       "Tb"},   
    {66  , 162.500       , "Dysprosium",    "Dy"},   
    {67  , 164.93033     , "Holmium",       "Ho"},   
    {68  , 167.259       , "Erbium",        "Er"},   
    {69  , 168.93422     , "Thulium",       "Tm"},   
    {70  , 173.054       , "Ytterbium",     "Yb"},   
    {71  , 174.9668      , "Lutetium",      "Lu"},
    {72  , 178.49        , "Hafnium",       "Hf"},
    {73  , 180.94788     , "Tantalum",      "Ta"},
    {74  , 183.84        , "Tungsten",      "W" },
    {75  , 186.207       , "Rhenium",       "Re"},
    {76  , 190.23        , "Osmium",        "Os"},
    {77  , 192.217       , "Iridium",       "Ir"},
    {78  , 195.084       , "Platinum",      "Pt"},
    {79  , 196.966569    , "Gold",          "Au"},
    {80  , 200.592       , "Mercury",       "Hg"},
    {81  , 204.385       , "Thallium",      "Tl"},
    {82  , 207.2         , "Lead",          "Pb"},
    {83  , 208.98040     , "Bismuth",       "Bi"},
    {84  , 209.0         , "Polonium",      "Po"},
    {85  , 210.0         , "Astatine",      "At"},
    {86  , 222.0         , "Radon",         "Rn"},
    {87  , 223.0         , "Francium",      "Fr"},
    {88  , 226.0         , "Radium",        "Ra"},
    {89  , 227.0         , "Actinium",      "Ac"},
    {90  , 232.0377      , "Thorium",       "Th"},      
    {91  , 231.03588     , "Protactinium",  "Pa"},
    {92  , 238.02891     , "Uranium",       "U" },
    {93  , 237.0         , "Neptunium",     "Np"},
    {94  , 244.0         , "Plutonium",     "Pu"},
    {95  , 241.0568293   , "Americium",     "Am"}, 
    {96  , 243.0613893   , "Curium",        "Cm"},
    {97  , 247.0703073   , "Berkelium",     "Bk"},
    {98  , 249.0748539   , "Californium",   "Cf"},
    {99  , 252.082980    , "Einsteinium",   "Es"},
    {100 , 257.0951061   , "Fermium",       "Fm"},
    {101 , 258.0984315   , "Mendelevium",   "Md"},
    {102 , 259.10103     , "Nobelium",      "No"},
    {103 , 262.10961     , "Lawrencium",    "Lr"},
    {104 , 267.12179     , "Rutherfordium", "Rf"},
    {105 , 268.12567     , "Dubnium",       "Db"},
    {106 , 271.13393     , "Seaborgium",    "Sg"},
    {107 , 272.13826     , "Bohrium",       "Bh"},
    {108 , 270.13429     , "Hassium",       "Hs"},
    {109 , 276.15159     , "Meitnerium",    "Mt"},
    {110 , 281.16451     , "Darmstadtium",  "Ds"},
    {111 , 280.16514     , "Roentgenium",   "Rg"},
    {112 , 285.17712     , "Copernicium",   "Cn"},
    {113 , 284.17873     , "Nihonium",      "Nb"},
    {114 , 289.19042     , "Flerovium",     "Fl"},
    {115 , 288.19274     , "Moscovium",     "Mc"},
    {116 , 293.20449     , "Livermorium",   "Lv"},
    {117 , 292.20746     , "Tennessine",    "Ts"},
    {118 , 294.21392     , "Oganesson",     "Og"},  
    { -1 , 0.0           , "Noelement",     "--"}
  };
}

int ptable::atomic_number(ptable::element e)
{
  return elements[e].anumber;
}

double ptable::atomic_weight(ptable::element e)
{
  return elements[e].weight;
}

const char * ptable::atomic_symbol(ptable::element e)
{
  return elements[e].symbol;
}

const char * ptable::element_name(ptable::element e)
{
  return elements[e].name;
}

ptable::element ptable::symbol_to_element (const char * s)
{
  for (int i = ptable::H; i < ptable::NO_ELEMENT; ++i)
    if ( strcasecmp(s, elements[i].symbol) == 0 ) 
      return ptable::element(i);
      
  return ptable::NO_ELEMENT;
}

