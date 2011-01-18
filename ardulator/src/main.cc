#include "arduino_api.h"
#include "private_config.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <getopt.h>

using namespace std;

Arduino *ardu = NULL;

void
setupArduino() {
    ardu = new Arduino();
}

void
print_help_menu(string name) {
    cout << "Usage: " << name << " [options] [file ...]" << endl;
    cout << "More usage information\n";
}

void
set_realtime(bool flag) {
    
}

bool test_input_file(char *name) {
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
            cout << "i: " << i << "\n";
            string arg(argv[i]);
            if (arg == "-h" || arg == "--help") {
                print_help_menu(argv[0]);
                exit(0);
            }
            if (test_input_file(argv[i])) {
                cout << "found input file " << argv[i] << "\n\n";
                ardu->addInputFile(argv[i]);
                cout << "done?\n";
            }
        }
    }
    
    cout << "Done Parsing the scenario\n\n";
    ardu->runScenario();
    
    cout << "Done running the scenario\n";
    
    ardu->report();
    
    return 0;
}
