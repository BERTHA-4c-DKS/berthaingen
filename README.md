# berthaingen

Bertha input generator this is the main  intent, however  this is also 
an excuse to write down some basic code to deal with a basic molecular 
structure class and similar code mainly for teaching purpose 

C++ code example: 

$ ./berthaingen -f "Cl:./Cl/fitt/a2.txt;Cn:./Cn/fitt/cn.abs;Au:./Au/fitt/b20.txt" \
    -b "Cl:./Cl/basis/aug-cc-pVTZ-DK.txt;Cn:./Cn/basis/dyall_vtz.txt;Au:./Au/basis/dyall_vqz.txt" \ 
    -c AuClCn.xyz 

pybgen example: 

$ python3 pybgen.py -f Au20Pb.xyz -b "Au:dyall_vtz,Pb:dyall_vtz" -t "Au:b16,Pb:pb"


