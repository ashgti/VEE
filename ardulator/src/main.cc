#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <getopt.h>
#include "private_config.h"


using namespace std;

Arduino *ardu = NULL;

void
setupArduino() {
    ardu = new Arduino();
}

void
printHelpMenu(string name) {
    cout << "Usage: " << name << " [options] [file ...]" << endl;
    cout << "More usage information\n";
}

bool 
testInputFile(char *name) {
    ifstream ifile(name);
    return ifile;
}

int main(int argc, char** argv) {
    setupArduino();
    pinConfiguration();
    setupComponents();
    /* Parse the program options */
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            string arg(argv[i]);
            if (arg == "-h" || arg == "--help") {
                printHelpMenu(argv[0]);
                exit(0);
            }
            if (testInputFile(argv[i])) {
                cout << "found input file " << argv[i] << "\n\n";
                ardu->addInputFile(argv[i]);
            }
        }
    }
    
    cout << "Done parsing the scenario configuration file\n\n";
    ardu->runScenario();
    
    cout << "Done running the scenario\n";
    
    ardu->report();
    
    return 0;
}
