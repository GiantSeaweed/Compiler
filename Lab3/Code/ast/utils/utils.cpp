//
// Created by 冯诗伟 on 2019-05-15.
//

#include "utils.h"
#include <iostream>
using namespace std;

void printError(int errNum, const string &message, int location) {
    cout << "Error type " << errNum << " at Line " << location << " : " << message << endl;
}