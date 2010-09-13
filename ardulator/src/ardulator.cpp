#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "arduino_api.h"
#include "private_config.h"

using namespace std;

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
    /* Parse the program options */
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (string(argv[i]) == "-h" || string(argv[i]) == "--help") {
                print_help_menu(argv[0]);
                exit(0);
            }
            if (string(argv[i]) == "-r" || string(argv[i]) == "--realtime") {
                cout << "realtime mode requested.\n";
                exit(0);
            }
            
            if (test_input_file(argv[i])) {
                cout << "found input file " << argv[i] << endl;
                ardu.add_input_file(argv[i]);
            }
        }
    }
    
    setup();
    for (int i = 0; i < 5; i++) {
        loop();
    }
    return 0;
}