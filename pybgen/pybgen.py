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

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-f","--inputfile", help="Specify XYX input file", required=True, \
        type=str, default="")
    parser.add_argument("-j","--jsonbasisfile", \
        help="Specify BERTHA JSON file for fitting and basis (default: fullsets.json)", \
        required=False, type=str, default="fullsets.json")

    args = parser.parse_args()
    
    basisdata = None
    with open(args.jsonbasisfile) as f:
        basisdata = json.load(f)

    mol = molecule()

    with open(args.inputfile) as fp:
        dim = int(fp.readline())
        header  = fp.readline()
        for i in range(dim):
            l = fp.readline()
            sl = l.split()

            if len(sl) != 4:
                print("Error at line "+ l)
                exit(1)

            a = atom(sl[0], float(sl[1]), \
                float(sl[2]), float(sl[3]))
            mol.add_atom(a)

    print(mol)