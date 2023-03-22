import json
import argparse

from mendeleev import element
from dataclasses import dataclass

@dataclass
class berthainputoption:
    inputfile: str = ""
    jsonbasisfile: str ="fullsets.json"
    fittset: str = ""
    basisset: str = ""
    restarton: int = 1
    grid: int  = 5
    functxc: str = "BLYP"
    maxit: int = 100
    usefitt: int = 0
    convertlengthunit: float = 1.0
    berthainfname: str = "input.inp"
    berthafittfname: str = "fitt2.inp"

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
def showdataforatom (boption, atom):

    basisdata = None
    with open(boption.jsonbasisfile) as f:
        basisdata = json.load(f)

    jsonkey = "BasisFittSetBertha"

    basissetlist = []
    fittsetlist = []

    for ad in basisdata[jsonkey]:
        for k in ad:
            sk = k.split("/")

            if len(sk) != 3:
                print("Error in basis file ", sk)
                exit(1)

            a = sk[0]
            basisname = sk[1]
            basistype = sk[2]

            if basistype == "basisset":
                if a == atom:
                    basissetlist.append(basisname)
                    #print(basisname)
            elif  basistype == "fittset":
                if a == atom:
                    fittsetlist.append(basisname)
                    #print(basisname)

    return basissetlist, fittsetlist

#################################################################################################

def writeinput (mol, atom2basisset, fout, boption):

    fout.write("\'TYPE OF BASIS SET; 1 FOR GEOMETRIC, 2 FOR OPTIMIZED\'\n")
    fout.write("2\n")
    fout.write("\'NUMBER OF CENTERS\'\n" )
    fout.write(str(mol.get_num_of_atoms()) + "\n")

    totalelectrons = 0
    for a in mol.get_atoms():
        si = element(a.get_symbol())
        totalelectrons += si.electrons - a.get_charge()
    
    for i, a in enumerate(mol.get_atoms()):
        si = element(a.get_symbol())
        basisset = atom2basisset[a.get_symbol()]

        fout.write("\'COORDINATES FOR CENTER %5d \'\n"%(i+1)) 
        x = a.get_coordinates()[0] * boption.convertlengthunit
        y = a.get_coordinates()[1] * boption.convertlengthunit
        z = a.get_coordinates()[2] * boption.convertlengthunit
        fout.write("%12.8f %12.8f %12.8f\n"%(x,y,z))
        fout.write("\'Z, N, MAXL AND CHARGE FOR CENTER %5d \'\n"%(i+1)) 
        an = si.atomic_number
        aw = si.atomic_weight
        maxl = basisset["Dim"]
        fout.write("%d,%f,%d,%d\n"%(an, aw, maxl, a.get_charge()))
        fout.write("\'BASIS SET FOR CENTER %5d %s %s\'\n"%(i+1, a.get_symbol(),
            basisset["Basisname"]))
        for h, vs in zip(basisset["Header"], basisset["Values"]):
            fout.write(h + "\n")
            for v in vs:
                fout.write(v + "\n")

    netcharge=boption.totalcharge

    fout.write("\'NUMBER OF CLOSED-SHELL ELECTRONS\'"+"\n")
    fout.write(str(int(totalelectrons-netcharge)) + ",0,0"+"\n")
    fout.write("\'SPECIFY CLOSED AND OPEN SHELLS AND COUPLING\'"+"\n")
    fout.write("0"+"\n")
    fout.write("\'ENTER 1 FOR NEW RUN AND 0 FOR RESTART\'"+"\n")
    fout.write(str(boption.restarton) + "\n")
    fout.write("\'LEVEL SHIFT FACTOR IN STAGE 0, 1, AND 2\'"+ "\n")
    fout.write("-2.0,-2.0,-2.0"+ "\n")
    fout.write("\'STARTING STAGE (0-2)\'"+ "\n")
    fout.write("2"+ "\n")
    fout.write("\'PRINT LEVEL FROM 1-2\'"+ "\n")
    fout.write("2"+ "\n")
    fout.write("\'DAMPING FACTOR AND RELATIVE TRESHOLD FOR INITIATION OF DAMPING\'"+ "\n")
    fout.write("0.10D0,1.0D-2"+ "\n")
    fout.write("\'ENTER NCORE, MACTVE,NACTVE\'"+ "\n")
    fout.write(str(int(totalelectrons-netcharge)) + ",0,0"+ "\n")
    fout.write("\'ENTER GRID QUALITY FROM 1 (COURSE) to 5 (FINE)\'"+ "\n")
    fout.write(str(boption.grid)+ "\n")
    fout.write("\'EX-POTENTIAL available: LDA,B88P86,HCTH93,BLYP'"+ "\n")
    fout.write(boption.functxc+ "\n")
    fout.write("\'Fitt 2=standard:fitt2 4=solo_poisson:fitt3 6=both:fitt2+fitt3, USEFITT\'"+ "\n")
    fout.write("2 " + str(boption.usefitt) + "\n")
    fout.write("\'scalapack\'"+ "\n")
    fout.write("2 2 32 2.0"+ "\n")
    fout.write("\'maxit\'"+ "\n")
    fout.write(str(boption.maxit) + "\n")
 

#################################################################################################

def writefitt (mol, atom2fittset, fout, boption):

    fout.write(str(mol.get_num_of_atoms()) + "\n")

    for i, a in enumerate(mol.get_atoms()):
        basisset = atom2fittset[a.get_symbol()]

        x = a.get_coordinates()[0] * boption.convertlengthunit
        y = a.get_coordinates()[1] * boption.convertlengthunit
        z = a.get_coordinates()[2] * boption.convertlengthunit
        fout.write("%12.8f %12.8f %12.8f\n"%(x,y,z))
        fout.write("%d\n"%(basisset["Dim"]))
        for vs in basisset["Values"]:
            for v in vs:
                fout.write(v + "\n")

#################################################################################################

def generateinputfiles (boption):

    basisdata = None
    with open(boption.jsonbasisfile) as f:
        basisdata = json.load(f)

    mol = molecule()

    jsonkey = "BasisFittSetBertha"

    atoms = set()

    with open(boption.inputfile) as fp:
        dim = int(fp.readline())
        header  = fp.readline()
        for i in range(dim):
            l = fp.readline()
            sl = l.split()

            if len(sl) != 4 and len(sl) != 5:
                print("Error at line "+ l)
                exit(1)

            atoms.update([sl[0]])

            a = atom (sl[0], float(sl[1]), \
                float(sl[2]), float(sl[3]))

            if len(sl) == 5:
                a.set_charge(int(sl[4])) 

            mol.add_atom(a)

    atomtobasisset = {}
    atomtofittset = {}

    #print(boption.fittset)

    for ab in boption.fittset.split(","):
        sab = ab.split(":")

        if len(sab) != 2:
            print("Error in option ", boption.fittset)
            exit(1)

        atomname = sab[0]
        basisname = sab[1]

        atomtofittset[atomname] = basisname

    #print(atomtofittset)

    #print(boption.basisset)

    for ab in boption.basisset.split(","):
        sab = ab.split(":")

        if len(sab) != 2:
            print("Error in option ", boption.basisset)
            exit(1)

        atomname = sab[0]
        basisname = sab[1]

        atomtobasisset[atomname] = basisname

    #print(atomtobasisset)

    for an in atoms:
        if not an in atomtobasisset:
            print("Error basisset not defined for \"" + an +"\"")
            print(atomtobasisset)
            exit(1) 
             
        if not an in atomtofittset:
            print("Error fittingset not defined for \""+ an + "\"")
            print(atomtofittset)
            exit(1) 

    atom2fittsetvalues = {}
    atom2basissetvalues = {}

    for ad in basisdata[jsonkey]:
        for k in ad:
            sk = k.split("/")

            if len(sk) != 3:
                print("Error in basis file ", sk)
                exit(1)

            a = sk[0]
            basisname = sk[1]
            basistype = sk[2]

            if basistype == "basisset":
                if a in atomtobasisset:
                    if basisname == atomtobasisset[a]:
                        atom2basissetvalues[a] = ad[k]
                        #print(ad[k]["Dim"])
                        #print(ad[k]["Header"])
                        #print(ad[k]["Values"])
            elif  basistype == "fittset":
                if a in atomtofittset:
                    if basisname == atomtofittset[a]:
                        atom2fittsetvalues[a] = ad[k]
                        #print(ad[k]["Dim"])
                        #print(ad[k]["Values"])
    for an in atoms:
        if not an in atom2basissetvalues:
            print("Error basis set not foud for ", an)
            exit(1) 

        if not an in atom2fittsetvalues:
            print("Error fitting set not foud for ", an)
            exit(1) 

    with open(boption.berthainfname, "w") as fp:
        writeinput(mol, atom2basissetvalues, fp, boption)

    with open(boption.berthafittfname, "w") as fp:
        writefitt(mol, atom2fittsetvalues, fp, boption)

#################################################################################################

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-f","--inputfile", help="Specify XYX input file", required=False, \
        type=str, default="")
    parser.add_argument("-j","--jsonbasisfile", \
        help="Specify BERTHA JSON file for fitting and basis (default: fullsets.json)", \
        required=False, type=str, default="fullsets.json")
    parser.add_argument("-b","--basisset", \
        help="Specify BERTHA basisset \"atomname1:basisset1,atomname2:basisset2,...\"", \
        required=False, type=str, default="")
    parser.add_argument("-t","--fittset", \
        help="Specify BERTHA fitting set \"atomname1:fittset1,atomname2:fittset2,...\"", \
        required=False, type=str, default="")
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
    parser.add_argument("--convertlengthunit", help="Specify a length converter [default=1.0] i.e. 1.8897259886", \
        type=float, default=1.0)
    parser.add_argument("--showatom", help="Show basis and fittset for the given atom, jump generation", \
        type=str, default="")
    parser.add_argument("--totalcharge", help="set total charge of the system (default=0)", \
        type=float, default="0")

    args = parser.parse_args()

    boption = berthainputoption

    boption.inputfile = args.inputfile
    boption.jsonbasisfile = args.jsonbasisfile
    boption.fittset = args.fittset
    boption.basisset = args.basisset
    boption.restarton = args.restarton
    boption.grid = args.grid 
    boption.functxc = args.functxc
    boption.maxit = args.maxit
    boption.usefitt = args.usefitt
    boption.berthainfname = args.berthainfname
    boption.berthafittfname = args.berthafittfname
    boption.convertlengthunit = args.convertlengthunit
    boption.totalcharge= args.totalcharge

    if args.showatom == "" and args.basisset == "" and \
        args.fittset == "":
        print("If --showatom is empty need to specify --basisset and --fittset")
        exit(1)

    if args.basisset != "" and args.fittset != "" and args.showatom != "":
        print("If --showatom is not compatible with --basisset and --fittset")
        exit(1)
 

    if args.showatom != "":
        basissetlist, fittsetlist = showdataforatom( boption, args.showatom )

        print ("Basisset for atom ", args.showatom )
        print ("  - ", end="")
        for b in basissetlist:
            print("\"" + b + "\" ", end="")
        print()
        print ("Fittingset for atom ", args.showatom )
        print ("  - ", end="")
        for b in fittsetlist:
            print("\"" + b + "\" ", end="")
        print()

    else:
        if args.basisset == "" or args.fittset == "" or args.inputfile == "":
            print("Need to specify --basisset and --fittset and --inputfile")
            exit(1)

        generateinputfiles (boption)
