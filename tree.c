#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "tree.h"

void blue()
{ // Add Blue colour to the directories in tree -C command
    printf("\033[0;34m");
}
void reset()
{
    printf("\033[0m");
}
int file_count(char dirname[100])
{
    int count_file = 0;
    DIR *directory;
    directory = opendir(dirname);

    if (directory == NULL)
    {
        return 0;
    }
    struct dirent *entry;
    entry = readdir(directory);
    while (entry != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            count_file++;
        }
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            char path[200] = "";
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entry->d_name);
            count_file = count_file + file_count(path);
        }
        entry = readdir(directory);
    }

    return count_file;
}

int dir_count(char dirname[100])
{
    int count_dir = 0;
    DIR *directory;
    directory = opendir(dirname);
    if (directory == NULL)
    {
        return 0;
    }
    struct dirent *entry;
    entry = readdir(directory);
    while (entry != NULL)
    {	
	   char path[200] = "";
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entry->d_name);
            count_dir++;
            count_dir = count_dir + dir_count(path);
        }
        entry = readdir(directory);
    }
    return count_dir;
}

void listtree_a(char dirname[100], int indentation)
{
    DIR *directory;
    directory = opendir(dirname);
    if (directory == NULL)
    {
        printf(" Unable to open file\n");
        return;
    }
    int count_file, count_directory = 0;
    struct dirent *entry;
    entry = readdir(directory);
    while (entry != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            printf("%*s|--%s\n", indentation, "", entry->d_name);
        }

        // strcmp becoz when we print current directory
        // it prints . current directory  and .. parent directory

        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        { // IT IS A DIRECTORY
            printf("%*s|-- %s\n", indentation, "", entry->d_name);
            char path[200] = "";
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entry->d_name);
            listtree_a(path, indentation + 4);
        }
        entry = readdir(directory);
    }

    closedir(directory);
}
void tree_a(char dirname[100])
{ //  tree -a
    listtree_a(dirname, 0);
    printf(" %d directories %d files", dir_count(dirname), file_count(dirname));
}

void listtree_f(char dirname[100], int indentation)
{ // tree command tree -f
    DIR *directory;
    directory = opendir(dirname);
    if (directory == NULL)
    {
        return;
    }
    struct dirent *entry;
    entry = readdir(directory);
    while (entry != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            char filepath[200] = "";
            strcat(filepath, dirname);
            strcat(filepath, "/");
            strcat(filepath, entry->d_name);
            printf("%*s|-- %s\n", indentation, "", filepath);
        }
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {

            char dir_path[200] = "";
            strcat(dir_path, dirname);
            strcat(dir_path, "/");
            strcat(dir_path, entry->d_name);
            printf("%*s|-- %s\n", indentation, "", dir_path);
            listtree_f(dir_path, indentation + 2);
        }
        entry = readdir(directory);
    }
    closedir(directory);
}
void tree_f(char dirname[200])
{
    listtree_f(dirname, 0);
    printf("\n%d directories %d files", dir_count(dirname), file_count(dirname));
}

void listtree_d(char dirname[100], int indentation)
{ // tree -d
    DIR *directory;
    directory = opendir(dirname);
    if (directory == NULL)
    {
        return;
    }
    struct dirent *entry;
    entry = readdir(directory);
    while (entry != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            blue();
            printf("%*s|-- %s\n", indentation, "", entry->d_name);
            reset();
            char path[200] = "";
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entry->d_name);
            listtree_d(path, indentation + 4);
        }
        entry = readdir(directory);
    }
    closedir(directory);
}
void tree_d(char dirname[200])
{
    listtree_d(dirname, 0);
    printf("\n%d directories ", dir_count(dirname));
}
void listtree_L(char dirname[100], int indentation, int level)
{
    // tree -L
    // goes only L levels deep into subdirectories or folders
    if (level < 0)
    {
        printf("tree: Invalid level, must be greater than 0.\n");
        return;
    }
    DIR *directory;
    directory = opendir(dirname);

    if (directory == NULL)
    {
        return;
    }
    struct dirent *entry;
    entry = readdir(directory);
    while (entry != NULL && level)
    {
        if (entry->d_type == DT_REG)
        {
            printf("%*s|--%s\n", indentation, "", entry->d_name);
        }

        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        { // IT IS A DIRECTORY
            printf("%*s|-- %s\n", indentation, "", entry->d_name);
            char path[200] = "";
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entry->d_name);
            listtree_L(path, indentation + 2, level - 1);
        }
        entry = readdir(directory);
    }
}
void tree_L(char dirname[200], int level)
{
    listtree_L(dirname, 0, level);
    // change file count and dir coiunt function for level
    printf("\n%d directories %d files", dir_count(dirname), file_count(dirname));
}

void listtree_h(char dirname[200], int indentation)
{
    DIR *directory;
    directory = opendir(dirname);
    if (directory == NULL)
    {
        return;
    }
    struct dirent *entry;
    entry = readdir(directory);
    while (entry != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            struct stat buffer;
            stat(entry->d_name, &buffer);
            printf("%*s|-- [%ld]  %s\n", indentation, "", buffer.st_size, entry->d_name);
        }
        else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            struct stat buffer;
            stat(entry->d_name, &buffer);
            printf("%*s|--[%ld] %s\n", indentation, "", buffer.st_size, entry->d_name);
            char path[200] = "";
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entry->d_name);
            listtree_h(path, indentation + 6);
        }
        entry = readdir(directory);
    }
    closedir(directory);
}

void tree_h(char dirname[200])
{
    listtree_h(dirname, 0);
    printf("\n%d directories %d files", dir_count(dirname), file_count(dirname));
}
void help(char *arg)
{
    if (strcmp(arg, "--help") == 0)
    {
        printf(" ------- Listing options -------\n");
        printf("-a            All files are listed.\n");
        printf("-d            List directories only.\n");
        printf("-f            Print the full path prefix for each file.\n");
        printf("-L level      Descend only level directories deep.\n;");
        printf("  ------- File options -------");
        printf(" -h            Print the size in a more human readable way.");
        printf(" ------- Graphics options -------\n");
        printf(" -C            Turn colorization on always.\n");
        printf("------- Miscellaneous options -------\n");
        printf("--version     Print version and exit.\n");
        printf("  --help        Print usage and this help message and exit.\n");
    }
    else
        return;
}
void version(char *argv)
{
    if (strcmp(argv, "--version") == 0)
    {
        printf("tree v1.8.0 (snap) (c) 1996 - 2018 by Steve Baker, Thomas Moore, Francesc Rocher, Florian Sesser, Kyosuke Tokoro\n");
    }
    else
        return;
}
int isDirectory(char Dirname[100])
{
    DIR *directory;
    directory = opendir(Dirname);
    if (directory == NULL)
    {
        printf("%s [error opening dir]\n", Dirname);
        printf("\n");
        printf("0 directories ,0 files\n");
        return 0;
    }
    else
        return 1;
}

void listtree_C(char *dirname, int indentation)
{
    DIR *directory;
    directory = opendir(dirname);
    if (directory == NULL)
    {
        printf(" Unable to open file\n");
        return;
    }
    struct dirent *entry;
    entry = readdir(directory);
    while (entry != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            printf("%*s|--%s\n", indentation, "", entry->d_name);
        }
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        { // IT IS A DIRECTORY
            blue();
            printf("%*s|-- %s\n", indentation, "", entry->d_name);
            reset();
            char path[200] = "";
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entry->d_name);
            listtree_C(path, indentation + 4);
        }
        entry = readdir(directory);
    }
    closedir(directory);
}
void tree_C(char dirname[200])
{
    listtree_C(dirname, 0);
    printf("\n%d directories %d files", dir_count(dirname), file_count(dirname));
}
void listtree_D(char dirname[100], int indentation, struct stat buffer)
{
    DIR *directory;
    directory = opendir(dirname);
    if (directory == NULL)
    {
        printf(" Unable to open file\n");
        return;
    }

    struct dirent *entry;
    entry = readdir(directory);

    while (entry != NULL)
    {
        char path[200] = "";
        char time[100] = "";
        // struct stat buffer;
        if (entry->d_type == DT_REG)
        {

            stat(entry->d_name, &buffer);
            printf("%*s|--[%s]",indentation,"",ctime(&buffer.st_mtime));
            printf("%s",entry->d_name);
            // printf("%*s |-- [ %s ]     %s\n", indentation, "", ctime(&buffer.st_mtime), entry->d_name);
            printf("\n");
        }

        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            // IT IS A DIRECTORY

            stat(entry->d_name, &buffer);
            printf("%*s |-- [ %s ] %s\n", indentation, "", ctime(&buffer.st_mtime), entry->d_name);
            printf("\n");
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entry->d_name);
            listtree_D(path, indentation + 6, buffer);
        }
        entry = readdir(directory);
    }

    closedir(directory);
}
void tree_D(char dirname[100])
{
    struct stat buffer;
    listtree_D(dirname, 0, buffer);
    printf(" %d directories %d files", dir_count(dirname), file_count(dirname));
}
