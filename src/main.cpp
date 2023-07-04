#include <fstream>
#include "qm.h"

int main(int argc, char* argv[]){

    Quine_Mccluskey *qm;

    ifstream infile;
    infile.open(argv[1]);
    
    ofstream outfile;
    outfile.open(argv[2]);

    qm = new Quine_Mccluskey(infile);
    qm->solve_qm();
    qm->solve_Patrick_Method(outfile);

    infile.close();
    outfile.close();

    return 0;
}
