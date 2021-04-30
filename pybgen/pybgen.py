import json
import argparse

from mendeleev import element

#################################################################################################

class atom(object): # nuovo stile di classe python subclass di object 
    def __init__(self, at, x, y, z):
        self.__symbol = at
        self.__coordinate = (x, y, z)
        self.__charge = 0
    
    def set_symbol(self, at): 
        self.__symbol = at
    
    def get_symbol(self):
        return self.__symbol

    def set_charge(self, val): 
        self.__charge = val
    
    def get_charge(self):
        return self.__charge
 
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

    def get_num_of_atoms(self):
        return len(self.__list_atoms)
 
    def __repr__ (self):
        str = 'Molecule %s\n' % self.__name
        str = str + 'has %d atoms\n' % len(self.__list_atoms)
      
        for atom in self.__list_atoms:
            str = str + atom.get_str() + '\n'
 
        return str

#################################################################################################

def writeinput (mol, atom2basisset, fout, args):

    fout.write("\'TYPE OF BASIS SET; 1 FOR GEOMETRIC, 2 FOR OPTIMIZED\'\n")
    fout.write("2\n")
    fout.write("\'NUMBER OF CENTERS\'\n" )
    fout.write(str(mol.get_num_of_atoms()) + "\n")

    totalelectrons = 0
    for atom in mol.get_atoms():
        si = element(atom.get_symbol())
        totalelectrons += si.electrons - atom.get_charge()
    
    for i, atom in enumerate(mol.get_atoms()):
        si = element(atom.get_symbol())
        basisset = atom2basisset[atom.get_symbol()]

        fout.write("\'COORDINATES FOR CENTER %5d \'\n"%(i+1)) 
        x = atom.get_coordinates()[0]
        y = atom.get_coordinates()[1]
        z = atom.get_coordinates()[2]
        fout.write("%12.8f %12.8f %12.8f\n"%(x,y,z))
        fout.write("\'Z, N, MAXL AND CHARGE FOR CENTER %5d \'\n"%(i+1)) 
        an = si.atomic_number
        aw = si.atomic_weight
        maxl = basisset["Dim"]
        fout.write("%d,%f,%d,%d\n"%(an, aw, maxl, atom.get_charge()))
        fout.write("\'BASIS SET FOR CENTER %5d %s %s\'\n"%(i+1, atom.get_symbol(),
            basisset["Basisname"]))
        for h, vs in zip(basisset["Header"], basisset["Values"]):
            fout.write(h + "\n")
            for v in vs:
                fout.write(v + "\n")

    fout.write("\'NUMBER OF CLOSED-SHELL ELECTRONS\'"+"\n")
    fout.write(str(totalelectrons) + ",0,0"+"\n")
    fout.write("\'SPECIFY CLOSED AND OPEN SHELLS AND COUPLING\'"+"\n")
    fout.write("0"+"\n")
    fout.write("\'ENTER 1 FOR NEW RUN AND 0 FOR RESTART\'"+"\n")
    fout.write(str(args.restarton) + "\n")
    fout.write("\'LEVEL SHIFT FACTOR IN STAGE 0, 1, AND 2\'"+ "\n")
    fout.write("-2.0,-2.0,-2.0"+ "\n")
    fout.write("\'STARTING STAGE (0-2)\'"+ "\n")
    fout.write("2"+ "\n")
    fout.write("\'PRINT LEVEL FROM 1-2\'"+ "\n")
    fout.write("2"+ "\n")
    fout.write("\'DAMPING FACTOR AND RELATIVE TRESHOLD FOR INITIATION OF DAMPING\'"+ "\n")
    fout.write("0.10D0,1.0D-2"+ "\n")
    fout.write("\'ENTER NCORE, MACTVE,NACTVE\'"+ "\n")
    fout.write(str(totalelectrons) + ",0,0"+ "\n")
    fout.write("\'ENTER GRID QUALITY FROM 1 (COURSE) to 5 (FINE)\'"+ "\n")
    fout.write(str(args.grid)+ "\n")
    fout.write("\'EX-POTENTIAL available: LDA,B88P86,HCTH93,BLYP'"+ "\n")
    fout.write(args.functxc+ "\n")
    fout.write("\'Fitt 2=standard:fitt2 4=solo_poisson:fitt3 6=both:fitt2+fitt3, USEFITT\'"+ "\n")
    fout.write("2 " + str(args.usefitt) + "\n")
    fout.write("\'scalapack\'"+ "\n")
    fout.write("2 2 32 2.0"+ "\n")
    fout.write("\'maxit\'"+ "\n")
    fout.write(str(args.maxit) + "\n")
 

#################################################################################################

def writefitt (mol, atom2fittset, fout, args):

    fout.write(str(mol.get_num_of_atoms()) + "\n")

    for i, atom in enumerate(mol.get_atoms()):
        basisset = atom2fittset[atom.get_symbol()]

        x = atom.get_coordinates()[0]
        y = atom.get_coordinates()[1]
        z = atom.get_coordinates()[2]
        fout.write("%12.8f %12.8f %12.8f\n"%(x,y,z))
        fout.write("%d\n"%(basisset["Dim"]))
        for vs in basisset["Values"]:
            for v in vs:
                fout.write(v + "\n")

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
    parser.add_argument("--restarton", help="ENTER 1 FOR NEW RUN AND 0 FOR RESTART (default=1)", \
        type=int, default=1)
    parser.add_argument("--grid", help="ENTER GRID QUALITY FROM 1 (COURSE) to 5 (FINE) (default=5)", \
        type=int, default=5)
    parser.add_argument("--functxc", help="EX-POTENTIAL available: LDA,B88P86,HCTH93,BLYP (default=BLYP)", \
        type=str, default="BLYP")
    parser.add_argument("--maxit", help="Maximum number of iterations (default=100)", \
        type=int, default=100)
    parser.add_argument("--usefitt", help="USEFITT 0 OR 1 (default=0)", \
        type=int, default=0)
    parser.add_argument("--berthainfname", help="Specify Bertha input filename (default=input.inp)", \
        type=str, default="input.inp")
    parser.add_argument("--berthafittfname", help="Specify Bertha fitting filename (default=fitt2.inp)", \
        type=str, default="fitt2.inp")

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

            if len(sl) != 4 and len(sl) != 5:
                print("Error at line "+ l)
                exit(1)

            atoms.update([sl[0]])

            a = atom(sl[0], float(sl[1]), \
                float(sl[2]), float(sl[3]))

            if len(sl) == 5:
                a.set_charge(int(sl[4])) 

            mol.add_atom(a)

    atomtobasisset = {}
    atomtofittset = {}

    if len(args.fittset.split(",")) == len(atoms):
        for ab in args.fittset.split(","):
            sab = ab.split(":")

            if len(sab) != 2:
                print("Error in option ", args.fittset)
                exit(1)

            atomname = sab[0]
            basisname = sab[1]

            atomtofittset[atomname] = basisname

    if len(args.basisset.split(",")) == len(atoms):
        for ab in args.basisset.split(","):
            sab = ab.split(":")

            if len(sab) != 2:
                print("Error in option ", args.basisset)
                exit(1)

            atomname = sab[0]
            basisname = sab[1]

            atomtobasisset[atomname] = basisname

    for an in atoms:
        if not an in atomtobasisset:
            print("Error basisset not defined for ", an)
            exit(1) 
             
        if not an in atomtofittset:
            print("Error fittingset not defined for ", an)
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
        if not an in atom2basissetvalues:
            print("Error basis set not foud for ", an)
            exit(1) 

        if not an in atom2fittsetvalues:
            print("Error fitting set not foud for ", an)
            exit(1) 

    with open(args.berthainfname, "w") as fp:
        writeinput(mol, atom2basissetvalues, fp, args)

    with open(args.berthafittfname, "w") as fp:
        writefitt(mol, atom2fittsetvalues, fp, args)
