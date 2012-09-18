#include <stdlib.h>
#include <iostream>
using namespace std;

#include "config_parser.h"

int main(int argc, char **argv)
{
    IniConfigParser parser;
    cout<<parser;
    string str("fuck");
    parser.Set(str, str, str);
    parser.Set(str, str, str);
    parser.Set(str, str, str);
    parser.Set(str, str, str);
    cout<<parser;
    cout<<parser.Get(str, str);

    cout<<endl;
    return 0;
}
