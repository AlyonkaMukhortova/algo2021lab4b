#ifndef DIALOG_H
#define DIALOG_H
#include "tree.h"


//int menu();
int dialog(const char* msgs[], int N);
int get_int();
char* getstr();

int D_Add(Tree**, Table*), D_Find(Tree**, Table*), D_Delete(Tree**, Table*), D_Show(Tree**, Table*), D_Timing(Tree**, Table*), D_Count(Tree**, Table*);

int (*fptr[])(Tree*);
const char* msgs[];
const int NMsgs;
void read_b(Tree**);
int write_binary();


#endif


