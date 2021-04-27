import json
import argparse

class atom(object): # nuovo stile di classe python subclass di object 
    def __init__(self, at, x, y, z):
        self.__symbol = at
        self.__coordinate = (x, y, z)
    
    def set_symbol(self, at): 
        self.__symbol = at
    
    def get_symbol(self):
        return self.__symbol
 
    def set_coordinates(self, x, y, z):
        self.__coordinate = (x, y, z)
 
    def get_coordinates(self):
        return self.__coordinate
 
    def get_str(self):
        return '%s %10.4f %10.4f %10.4f' % (self.__symbol, self.__coordinate[0], \
          self.__coordinate[1], self.__coordinate[2])
 
    def __repr__(self): # overloads printing
        return self.get_str()

#################################################################################################

class molecule(object):
    def __init__ (self, nome = "noname"):
        self.__name = nome
        self.__list_atoms = []
 
    def add_atom (self, atom):
        self.__list_atoms.append(atom)
 
    def get_atoms (self):
        return self.__list_atoms
 
    def __repr__ (self):
        str = 'Molecule %s\n' % self.__name
        str = str + 'has %d atoms\n' % len(self.__list_atoms)
      
        for atom in self.__list_atoms:
            str = str + atom.get_str() + '\n'
 
        return str

#################################################################################################

def writeinput (mol, atom2basisset, fout):

    fout.write("\'TYPE OF BASIS SET; 1 FOR GEOMETRIC, 2 FOR OPTIMIZED\'\n")
    fout.write("2\n")
    fout.write("\'NUMBER OF CENTERS\'\n" )

#################################################################################################

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-f","--inputfile", help="Specify XYX input file", required=True, \
        type=str, default="")
    parser.add_argument("-j","--jsonbasisfile", \
        help="Specify BERTHA JSON file for fitting and basis (default: fullsets.json)", \
        required=False, type=str, default="fullsets.json")
    parser.add_argument("-b","--basisset", \
        help="Specify BERTHA basisset \"atomname1:basisset1,atomname2:basisset2,...\"", \
        required=True, type=str, default="")
    parser.add_argument("-t","--fittset", \
        help="Specify BERTHA fitting set \"atomname1:fittset1,atomname2:fittset2,...\"", \
        required=True, type=str, default="")

    args = parser.parse_args()
    
    basisdata = None
    with open(args.jsonbasisfile) as f:
        basisdata = json.load(f)

    mol = molecule()

    jsonkey = "BasisFittSetBertha"

    atoms = set()

    with open(args.inputfile) as fp:
        dim = int(fp.readline())
        header  = fp.readline()
        for i in range(dim):
            l = fp.readline()
            sl = l.split()

            if len(sl) != 4:
                print("Error at line "+ l)
                exit(1)

            atoms.update([sl[0]])

            a = atom(sl[0], float(sl[1]), \
                float(sl[2]), float(sl[3]))
            mol.add_atom(a)

    atomtobasisset = {}
    atomtofittset = {}

    if len(args.fittset.split(",")) == len(atoms):
        for ab in args.fittset.split(","):
            sab = ab.split(":")

            if len(sab) != 2:
                print("Error in option ", args.basis)
                exit(1)

            atomname = sab[0]
            basisname = sab[1]

            atomtofittset[atomname] = basisname

    if len(args.basisset.split(",")) == len(atoms):
        for ab in args.basisset.split(","):
            sab = ab.split(":")

            if len(sab) != 2:
                print("Error in option ", args.basis)
                exit(1)

            atomname = sab[0]
            basisname = sab[1]

            atomtobasisset[atomname] = basisname

    for an in atoms:
        if not an in atomtobasisset or \
            not an in atomtofittset:
            print("Error basis or fitting set not defined for ", an)
            exit(1) 

    atom2fittsetvalues = {}
    atom2basissetvalues = {}

    for ad in basisdata[jsonkey]:
        for k in ad:
            sk = k.split("/")

            if len(sk) != 3:
                print("Error in basis file ", sk)
                exit(1)

            atom = sk[0]
            basisname = sk[1]
            basistype = sk[2]

            if basistype == "basisset":
                if atom in atomtobasisset:
                    if basisname == atomtobasisset[atom]:
                        atom2basissetvalues[atom] = ad[k]
                        #print(ad[k]["Dim"])
                        #print(ad[k]["Header"])
                        #print(ad[k]["Values"])
            elif  basistype == "fittset":
                if atom in atomtofittset:
                    if basisname == atomtofittset[atom]:
                        atom2fittsetvalues[atom] = ad[k]
                        #print(ad[k]["Dim"])
                        #print(ad[k]["Values"])
    for an in atoms:
        if not an in atom2basissetvalues or \
            not an in atom2fittsetvalues:
            print("Error basis or fitting set not foud for ", an)
            exit(1) 

    with open("input.inp", "w") as fp:
        writeinput(mol, atom2basissetvalues, fp)
    # ready to dump input and fitt files

