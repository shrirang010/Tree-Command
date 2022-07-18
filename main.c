#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include "tree.h"
// POSIX Library

int main(int argc, char **argv)
{
    switch (argc)
    {
    case 1:
        tree_a(".");
        break;
    case 2:
        if (strcmp(argv[1], "--help") == 0)
        {
            help(argv[1]);
        }
        else if (strcmp(argv[1], "--version") == 0)
        {
            version(argv[1]);
        }
        else if (strcmp(argv[1], "-a") == 0)
        {
            tree_a(".");
        }
        else if (strcmp(argv[1], "-d") == 0)
        {
            tree_d(".");
        }
        else if (strcmp(argv[1], "-f") == 0)
        {
            tree_f(".");
        }
        else if(strcmp(argv[1],"-C") == 0){
            tree_C(".");
        }
        else if(strcmp(argv[1],"-h") == 0){
            tree_h(".");
        }
        else if(strcmp(argv[1],"-D") == 0){
            tree_D(".");
        }
        else
        {   // check if argv[1] is a directory
            if (isDirectory(argv[1]))
            {
                tree_a(argv[1]);
            }
        }
        break;
    case 3:
        if (strcmp(argv[1], "-L") == 0)
        {
            tree_L(".",  atoi(argv[2]));
        }
        else if (strcmp(argv[1], "-d") == 0 && isDirectory(argv[2]))
        {
            tree_d(argv[2]);
        }
        else if (strcmp(argv[1], "-f") == 0 && isDirectory(argv[2]))
        {
            tree_f(argv[2]);
        }
        else if(strcmp(argv[1],"-C") == 0 && isDirectory(argv[2]))
        {
            tree_C(argv[2]);
        }
        else if(strcmp(argv[1],"-h") == 0 && isDirectory(argv[2])){
            tree_h(argv[2]);
        }
	else if(strcmp(argv[1],"-D") == 0 && isDirectory(argv[2])){
            tree_D(argv[2]);
        }
        break;
    }
}
