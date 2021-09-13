#ifndef TREE_H
#define TREE_H
#include "info.h"


typedef struct Tree {
	int key[3];
	InfoType* info1;
	InfoType* info2;
	InfoType* info3;
	struct Tree* left;
	struct Tree* middle;
	struct Tree* right;
	struct Tree* add;
	struct Tree* parent;
}Tree;

typedef struct KeySpace {
	int key;
	int p;
	Tree* node;
}KeySpace;

typedef struct Table {
	int size;
	KeySpace* ks;
}Table;

int tree_insert(Tree** tree, int key, InfoType* info, Table*tab);
Tree* tree_find(Tree** tree, int key, int *ind, Table*tab);
int show_tree(Tree** tree, int n);
int tree_remove(Tree** tree, int key, Table*tab);
int delTree(Tree** tree);
Tree* find_min_find(Tree** tree, int* p);
void show_tree_only_one(Tree** tree, int ind);
void show_tree_straight(Tree** tree);
Tree* tree_find_leaf(Tree** tree, int key);
Tree* find_min(Tree** tree);
void show_tree_only_one_offset(Tree** tree, int ind);
Tree* next_no(Tree** tree, int key, int* p);
void show_tree_graphviz(Tree* tree, FILE* fd, int* n);
void initTable(Table* table);

#endif // !TREE_H
#pragma once
