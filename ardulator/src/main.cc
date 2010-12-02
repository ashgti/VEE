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
    pinConfiguration();
    setupComponents();
    double duration = 100.00; // Seconds
    /* TODO: Convert this to use getopt.h */
    /* Parse the program options */
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            string arg(argv[i]);
            if (arg == "-h" || arg == "--help") {
                print_help_menu(argv[0]);
                exit(0);
            }
            if (arg == "-r" || arg == "--realtime") {
                cout << "realtime mode requested.\n";
                exit(0);
            }
            if (arg == "-l" || arg.find("--length") != string::npos) {
                // double duration;
                // stringstream duration_finder;
                // int start = arg.find("--length");
                // duration_finder << arg.substr(start, string("--length").size());
                // duration_finder >> duration;
            }
            if (test_input_file(argv[i])) {
                cout << "found input file " << argv[i] << "\n";
                ardu->addInputFile(argv[i]);
            }
        }
    }
    
    cout << "Done Parsing the scenario\n\n";
    
    ardu->runScenario(duration);
    
    return 0;
}
