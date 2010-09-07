#include "problem.h"
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

void
print_help_menu(string name) {
    cout << "Usage: " << name << " [options] [file ...]" << endl;
}


void
set_realtime(bool flag) {

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
                
            }
        }
    }
    
    setup();
    for (int i = 0; i < 5; i++) {
        loop();
    }
    return 0;
}