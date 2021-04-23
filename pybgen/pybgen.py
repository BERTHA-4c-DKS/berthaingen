import json
import argparse

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

    

    

