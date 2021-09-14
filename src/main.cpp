#include <iostream>
#include "Session.h"


using namespace std;

int main(int argc, char** argv){
    if(argc != 2) {
        std::cout << "usage cTrace <config_path>" << argc <<endl;
        return 0;
    }

    Session sess(argv[1]);
    sess.simulate();


    return 0;
}

