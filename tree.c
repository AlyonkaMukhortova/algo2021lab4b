#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

void initTable(Table* table) {
	table->ks = (KeySpace*)calloc(table->size, sizeof(KeySpace));
}

int hash(int key, int size) {
	int sum = 0;
	while (key > 0) {
		sum += key % 10;
		key = key / 10;
	}
	return sum % size;
}

void inTable(Table* tab, int key, Tree* node, int p) {
	if (key != NULL) {
		int ha = hash(key, tab->size);
		tab->ks[ha].key = key;
		tab->ks[ha].node = node;
		tab->ks[ha].p = p;
	}
}

Tree* initTree() {
	Tree* tree;
	tree = (Tree*)malloc(sizeof(Tree));
	tree->key[0] = NULL;
	tree->info1 = NULL;
	tree->info2 = NULL;
	tree->key[1] = NULL;
	tree->key[2] = NULL;
	tree->info3 = NULL;
	tree->left = NULL;
	tree->middle = NULL;
	tree->parent = NULL;
	tree->right = NULL;
	tree->add = NULL;
	return tree;
}	

Tree* split_parent(Tree** tree, Table* tab) {
	Tree* ptr = *tree;
	Tree* t = *tree;
	if (ptr->key[2] != NULL) {
		Tree* a = initTree();
		a->left = ptr->right;
		ptr->right = NULL;
		a->middle = ptr->add;
		ptr->add = NULL;
		a->key[0] = ptr->key[2];
		ptr->key[2] = NULL;
		a->info1 = ptr->info3;
		ptr->info3 = NULL;
		a->parent = ptr->parent;
		inTable(tab, a->key[0], a, 0);
		if(a->left)
			a->left->parent = a;
		if(a->middle)
			a->middle->parent = a;
		if (ptr->parent != NULL) {
			
			if (ptr->parent->key[1] == NULL) {
				if (ptr->key[1] > ptr->parent->key[0]) {
					ptr->parent->key[1] = ptr->key[1];
					ptr->parent->info2 = ptr->info2;
					ptr->parent->right = a;
					ptr->parent->right->parent = ptr->parent;
					inTable(tab, ptr->parent->key[1], ptr->parent, 1);
				}
				else {
					ptr->parent->key[1] = ptr->parent->key[0];
					ptr->parent->info2 = ptr->parent->info1;
					ptr->parent->key[0] = ptr->key[1];
					ptr->parent->info1 = ptr->info2;
					ptr->parent->right = ptr->parent->middle;
					ptr->parent->middle = a;
					ptr->parent->middle->parent = ptr->parent;
					inTable(tab, ptr->parent->key[1], ptr->parent, 1);
					inTable(tab, ptr->parent->key[0], ptr->parent, 0);
				}
				ptr->key[1] = NULL;
				ptr->info2 = NULL;
				
			}
			else {
				if (ptr->key[1] > ptr->parent->key[1]) {
					ptr->parent->key[2] = ptr->key[1];
					ptr->parent->info3 = ptr->info2;
					ptr->parent->add = a;
					a->parent = ptr->parent;
					inTable(tab, ptr->parent->key[2], ptr->parent, 2);
				}
				else if (ptr->key[1] < ptr->parent->key[1] && ptr->key[1] > ptr->parent->key[0]) {
					ptr->parent->key[2] = ptr->parent->key[1];
					ptr->parent->info3 = ptr->parent->info2;
					ptr->parent->key[1] = ptr->key[1];
					ptr->parent->info2 = ptr->info2;
					ptr->parent->add = ptr->parent->right;
					ptr->parent->right = a;
					a->parent = ptr->parent;
					inTable(tab, ptr->parent->key[2], ptr->parent, 2);
					inTable(tab, ptr->parent->key[1], ptr->parent, 1);
				}
				else {
					ptr->parent->key[2] = ptr->parent->key[1];
					ptr->parent->info3 = ptr->parent->info2;
					ptr->parent->key[1] = ptr->parent->key[0];
					ptr->parent->info2 = ptr->parent->info1;
					ptr->parent->key[0] = ptr->key[1];
					ptr->parent->info1 = ptr->info2;
					ptr->parent->add = ptr->parent->right;
					ptr->parent->right = ptr->parent->middle;
					ptr->parent->middle = ptr->parent->left;
					ptr->parent->left = a;
					a->parent = ptr->parent;
					inTable(tab, ptr->parent->key[2], ptr->parent, 2);
					inTable(tab, ptr->parent->key[1], ptr->parent, 1);
					inTable(tab, ptr->parent->key[0], ptr->parent, 0);
				}
				ptr->key[1] = NULL;
				ptr->info2 = NULL;
				
			}
			t = split_parent(&ptr->parent, tab);
		}
		else {
			Tree* root = initTree();
			root->left = ptr;
			root->middle = a;
			root->key[0] = ptr->key[1];
			root->info1 = ptr->info2;
			inTable(tab, root->key[0], root, 0);
			ptr->key[1] = NULL;
			ptr->key[2] = NULL;
			ptr->info2 = NULL;
			ptr->info3 = NULL;
			(*tree)->parent = root;
			ptr->parent = root;
			a->parent = root;
			return root;
			//*tree = root;
		}
		
		//*tree = ptr->parent;
	}
	
	while (t->parent != NULL)
		t = t->parent;
	return t;
}

int tree_insert(Tree** tree, int key, InfoType* info, Table* tab) {
	int a;
	int ha = hash(key, tab->size);
	if (tree_find(tree, key, &a, tab) != NULL)
		return EXISTS;
	if (*tree == NULL) {
		*tree = initTree();
		(*tree)->key[0] = key;
		(*tree)->info1 = info;
		inTable(tab, key, *tree, 0);
		/*tab->ks[ha].key = key;
		tab->ks[ha].node = *tree;
		tab->ks[ha].p = 0;*/
		return SUCCESS;
	}
	
	Tree* leaf = tree_find_leaf(tree, key);
	if (leaf!= NULL && leaf->key[1] != NULL) {
		//Tree* new = initTree();
		int k = NULL;
		InfoType* i = NULL;
		if (key < leaf->key[0]) {
			leaf->key[2] = leaf->key[1];
			leaf->info3 = leaf->info2;
			inTable(tab, leaf->key[2], leaf, 2);
			leaf->key[1] = leaf->key[0];
			leaf->info2 = leaf->info1;
			inTable(tab, leaf->key[1], leaf, 1);
			leaf->key[0] = key;
			leaf->info1 = info;
			inTable(tab, key, leaf, 0);
			/*tab->ks[ha].key = key;
			tab->ks[ha].node = leaf;
			tab->ks[ha].p = 0;*/
		}
		else if (key > leaf->key[0] && key < leaf->key[1]) {
			leaf->key[2] = leaf->key[1];
			leaf->info3 = leaf->info2;
			inTable(tab, leaf->key[2], leaf, 2);
			leaf->key[1] = key;
			leaf->info2 = info;
			inTable(tab, key, leaf, 1);
		}
		else {
			leaf->key[2] = key;
			leaf->info3 = info;
			inTable(tab, key, leaf, 2);
		}
		Tree* new = initTree();
		new->key[0] = leaf->key[2];
		new->info1 = leaf->info3;
		new->parent = leaf->parent;
		inTable(tab, new->key[0], new, 0);
		leaf->key[2] = NULL;
		leaf->info3 = NULL;
		if (leaf->parent == NULL) {
			Tree* root = initTree();
			root->key[0] = leaf->key[1];
			root->info1 = leaf->info2;
			inTable(tab, root->key[0], root, 0);
			leaf->key[1] = NULL;
			leaf->info2 = NULL;
			root->left = leaf;
			root->middle = new;
			leaf->parent = root;
			new->parent = root;
			*tree = root;
			return SUCCESS;
		}
		else {
			if(leaf == leaf->parent->middle || leaf == leaf->parent->right)
				if (leaf->parent->right == NULL) {
					leaf->parent->right = new;
					leaf->parent->key[1] = leaf->key[1];
					leaf->parent->info2 = leaf->info2;
					inTable(tab, leaf->parent->key[1], leaf->parent, 1);
					leaf->key[1] = NULL;
					leaf->info2 = NULL;
				}
				else if (leaf == leaf->parent->middle) {
					leaf->parent->add = leaf->parent->right;
					leaf->parent->right = new;
					leaf->parent->key[2] = leaf->parent->key[1];
					leaf->parent->info3 = leaf->parent->info2;
					inTable(tab, leaf->parent->key[2], leaf->parent, 2);
					leaf->parent->key[1] = leaf->key[1];
					leaf->parent->info2 = leaf->info2;
					inTable(tab, leaf->parent->key[1], leaf->parent, 1);
					leaf->key[1] = NULL;
					leaf->info2 = NULL;
				}
				else {
					leaf->parent->add = new;
					leaf->parent->key[2] = leaf->key[1];
					leaf->parent->info3 = leaf->info2;
					inTable(tab, leaf->parent->key[2], leaf->parent, 2);
					leaf->key[1] = NULL;
					leaf->info2 = NULL;
				}
			else {
				leaf->parent->add = leaf->parent->right;
				leaf->parent->right = leaf->parent->middle;
				leaf->parent->middle = new;
				leaf->parent->key[2] = leaf->parent->key[1];
				leaf->parent->key[1] = leaf->parent->key[0];
				leaf->parent->key[0] = leaf->key[1];
				leaf->parent->info3 = leaf->parent->info2;
				leaf->parent->info2 = leaf->parent->info1;
				leaf->parent->info1 = leaf->info2;
				inTable(tab, leaf->parent->key[2], leaf->parent, 2);
				inTable(tab, leaf->parent->key[1], leaf->parent, 1);
				inTable(tab, leaf->parent->key[0], leaf->parent, 0);
				leaf->key[1] = NULL;
				leaf->info2 = NULL;
				//*tree = split_parent(&leaf->parent);
			}
		}
		*tree = split_parent(&leaf->parent, tab);
		
		return SUCCESS;
		
	}
	if (leaf->key[1] == NULL) {
		if (key > leaf->key[0]) {
			leaf->key[1] = key;
			leaf->info2 = info;
			inTable(tab, key, leaf, 1);
		}
		else {
			int k = leaf->key[0];
			InfoType* i = leaf->info1;
			leaf->key[0] = key;
			leaf->info1 = info;
			inTable(tab, key, leaf, 0);
			leaf->key[1] = k;
			leaf->info2 = i;
			inTable(tab, k, leaf, 1);
		}
		return SUCCESS;
	}
	return SUCCESS;
}

Tree* tree_find(Tree** tree, int key, int* ind, Table* tab) {

	if (*tree == NULL)
		return NULL;
	Tree* ptr = *tree;
	Tree* res = NULL;
	if (tab->ks[hash(key, tab->size)].key == key) {
		res = tab->ks[hash(key, tab->size)].node;
		*ind = tab->ks[hash(key, tab->size)].p;
		return res;
	}
	int y;
	if (ptr == NULL) {
		res = NULL;
	}
	else {
		if (ptr != NULL && ptr->key[0] == key) {
			res = ptr;
			*ind = 0;
		}
		else if (ptr != NULL && ptr->key[1] == key) {
			res = ptr;
			*ind = 1;
		}
		else {
			if (key < ptr->key[0] ) {
				res = tree_find(&ptr->left, key, ind, tab);
			}
			else if (key > ptr->key[0] && (key < ptr->key[1] || ptr->key[1] == NULL)) {
				res = tree_find(&ptr->middle, key, ind, tab);
			}
			else 
				res = tree_find(&ptr->right, key, ind, tab);
		}
	}

	return res;
}



Tree* tree_find_leaf(Tree** tree, int key) {

	if (*tree == NULL)
		return NULL;
	Tree* ptr = *tree;
	Tree* res = ptr;
	int y;
	if (ptr == NULL) {
		res = NULL;
	}
	else {
		
		if (ptr->left != NULL && key < ptr->key[0]) {
			res = tree_find_leaf(&ptr->left, key);
		}
		else if (ptr->left != NULL && key > ptr->key[0] && (key < ptr->key[1] || ptr->key[1] == NULL)) {
			res = tree_find_leaf(&ptr->middle, key);
		}
		else if (ptr->right != NULL)
			res = tree_find_leaf(&ptr->right, key);
		
	}
	return res;
}

int show_tree(Tree** tree, int n) {
	Tree* ptr = *tree;
	if (ptr != NULL) {
		n++;
		show_tree(&ptr->right, n);
		if (ptr->key[1] != NULL) {
			for (int i = 0; i < n - 1; i++)
				printf("	");
			printf("{%d", ptr->key[1]);
			print_info(ptr->info2);
		}
		show_tree(&ptr->middle, n);
		if (ptr->key[0] != NULL) {
			for (int i = 0; i < n - 1; i++)
				printf("	");
			printf("{%d", ptr->key[0]);
			print_info(ptr->info1);
		}
		show_tree(&ptr->left, n);
	}
}

void show_tree_straight(Tree** tree) {
	Tree* ptr = *tree;
	if (ptr != NULL) {
		
		show_tree_straight(&ptr->left);
		if (ptr->key[0] != NULL) {
			printf("{%d", ptr->key[0]);
			print_info(ptr->info1);
		}
		show_tree_straight(&ptr->middle);
		if (ptr->key[1] != NULL) {
			printf("{%d", ptr->key[1]);
			print_info(ptr->info2);
		}
		show_tree_straight(&ptr->right);
	}
}

Tree* find_min(Tree** tree) {
	Tree* ptr = *tree;
	if (ptr == NULL)
		return NULL;
	while (ptr!= NULL && ptr->left != NULL) {
		ptr = ptr->left;
	}
	return ptr;
}

Tree* next_no(Tree** tree, int key, int* p) {
	Tree* result = *tree;
	Tree* res = result;
	if (key < result->key[0])
		*p = 0;
	else if (key <result->key[1] && key > result->key[0])
		*p = 1;
	else if (result->parent!= NULL){
		res = next_no(&result->parent, key, p);
	}
	return res;
}

Tree* find_min_find(Tree** tree, int *p) {
	Tree* ptr = *tree;
	Tree* root = ptr;
	if (root == NULL)
		return NULL;
	while (root->parent != NULL)
		root = root->parent;
	if (ptr == NULL)
		return NULL;
	if (*p == 0 && ptr->middle != NULL) {
		ptr = find_min(&ptr->middle);
		return ptr;
	}
	else if (*p == 1 && ptr->right != NULL) {
		ptr = find_min(&ptr->right);
		*p = 0;
		return ptr;
	}
	if (ptr->left == NULL) {
		while (ptr == ptr->parent->right || ptr == ptr->parent->middle && ptr->parent->right == NULL) {
			if (ptr->parent == root && (ptr == ptr->parent->right || ptr == ptr->parent->middle && ptr->parent->right == NULL))
				return NULL;
			ptr = ptr->parent;	
		}
		if (ptr == ptr->parent->middle)
			*p = 1;
		else *p = 0;
		return ptr->parent;
	}
}

void show_tree_only_one(Tree** tree, int ind) {
	Tree* ptr = *tree;
	if (ptr != NULL) {
		if (ptr->key[1] != NULL && ind == 1) {
			printf("{%d", ptr->key[1]);
			print_info(ptr->info2);
		}
		if (ptr->key[0] != NULL && ind == 0) {
			printf("{%d", ptr->key[0]);
			print_info(ptr->info1);
		}  
	}
}

void show_tree_only_one_offset(Tree** tree, int ind) {
	Tree* ptr = *tree;
	if (ptr != NULL) {
		if (ptr->key[1] != NULL && ind == 1) {
			printf("offset of num %d  =  %d\n", ptr->key[1], ptr->info2->offset);
			
		}
		if (ptr->key[0] != NULL && ind == 0) {
			printf("offset of num %d  =  %d\n", ptr->key[0], ptr->info1->offset);
			
		}
	}
}

Tree* fix_tree_first(Tree* leaf, Table* tab) {
	Tree* par = leaf->parent;
	if (leaf->key[0] != NULL)
		return par;
	if (par->key[1] != NULL && par->left->key[1] == NULL && par->middle->key[1] == NULL && par->right->key[1] == NULL) {
		if (leaf == par->left) {
			par->left = par->middle;
			par->left->parent = par;
			par->middle = par->right;
			par->middle->parent = par;
			par->right = NULL;
			int k = par->left->key[0];
			InfoType* i = par->left->info1;
			par->left->key[0] = par->key[0];
			par->left->info1 = par->info1;
			inTable(tab, par->left->key[0], par->left, 0);
			par->left->key[1] = k;
			par->left->info2 = i;
			inTable(tab, k, par->left, 1);
			par->key[0] = par->key[1];
			par->info1 = par->info2;
			inTable(tab, par->key[0], par, 0);
			par->key[1] = NULL;
			par->info2 = NULL;
			par->left->right = par->left->middle;
			par->left->middle = par->left->left;
			if (leaf->left != NULL)
				par->left->left = leaf->left;
			else if (leaf->middle != NULL)
				par->left->left = leaf->middle;
			if (par->left->left != NULL)
				par->left->left->parent = par->left;
		}
		else if (leaf == par->middle) {
			par->left->key[1] = par->key[0];
			par->left->info2 = par->info1;
			inTable(tab, par->left->key[1], par->left, 1);
			par->key[0] = par->key[1];
			par->info1 = par->info2;
			inTable(tab, par->key[0], par, 0);
			par->key[1] = NULL;
			par->info2 = NULL;
			if (leaf->left != NULL)
				par->left->right = leaf->left;
			else if (leaf->middle != NULL)
				par->left->right = leaf->middle;
			if (par->left->right != NULL)
				par->left->right->parent = par->left;
			par->middle = par->right;
			par->right = NULL;
		}
		else if (leaf == par->right) {
			par->middle->key[1] = par->key[1];
			par->middle->info2 = par->info2;
			inTable(tab, par->middle->key[1], par->middle, 1);
			par->key[1] = NULL;
			par->info2 = NULL;
			par->right = NULL;
			if (leaf->left != NULL)
				par->middle->right = leaf->left;
			else if (leaf->middle != NULL)
				par->middle->right = leaf->middle;
			if (par->middle->right != NULL)
				par->middle->right->parent = par->middle;
		}
	}
	else if (par->key[1] != NULL && (par->left->key[1] != NULL || par->middle->key[1] != NULL || par->right->key[1] != NULL)) {
		if (leaf == par->right) {
			if (leaf->left != NULL) {
				leaf->middle = leaf->left;
				leaf->left = NULL;
			}
			leaf->key[1] = leaf->key[0];
			leaf->info2 = leaf->info1;
			inTable(tab, leaf->key[1], leaf, 1);
			leaf->key[0] = par->key[1];
			leaf->info1 = par->info2;
			inTable(tab, leaf->key[0], leaf, 0);
			par->key[1] = NULL;
			par->info2 = NULL;
			if (par->middle->key[1] != NULL) {
				par->key[1] = par->middle->key[1];
				par->info2 = par->middle->info2;
				inTable(tab, par->key[1], par, 1);
				par->middle->key[1] = NULL;
				par->middle->info2 = NULL;
				leaf->left = par->middle->right;
				par->middle->right = NULL;
				if (leaf->left != NULL)
					leaf->left->parent = leaf;
			}
			else if (par->left->key[1] != NULL) {
				par->key[1] = par->middle->key[0];
				par->info2 = par->middle->info1;
				inTable(tab, par->key[1], par, 1);
				par->middle->key[0] = NULL;
				par->middle->info1 = NULL;
				leaf->left = par->middle->middle;
				par->middle->middle = par->middle->left;
				if (leaf->left != NULL)
					leaf->left->parent = leaf;
				par->middle->key[0] = par->key[0];
				par->middle->info1 = par->info1;
				inTable(tab, par->middle->key[0], par->middle, 0);
				par->key[0] = par->left->key[1];
				par->info1 = par->left->info2;
				inTable(tab, par->key[0], par, 0);
				par->left->key[1] = NULL;
				par->left->info2 = NULL;
				par->middle->left = par->left->right;
				par->left->right = NULL;
				if (par->middle->left != NULL)
					par->middle->left->parent = par->middle;
			}
		}
		else if (leaf == par->middle) {
			if (par->right->key[1] != NULL) {
				if (leaf->left == NULL) {
					leaf->left = leaf->middle;
					leaf->middle = NULL;
				}
				par->middle->key[0] = par->key[1];
				par->middle->info1 = par->info2;
				inTable(tab, par->middle->key[0], par->middle, 0);
				par->key[1] = par->right->key[0];
				par->info2 = par->right->info1;
				inTable(tab, par->key[1], par, 1);
				par->right->key[0] = par->right->key[1];
				par->right->info1 = par->right->info2;
				inTable(tab, par->right->key[0], par->right, 0);
				par->right->key[1] = NULL;
				par->right->info2 = NULL;
				par->middle->middle = par->right->left;
				if (par->middle->middle != NULL)
					par->middle->middle->parent = par->middle;
				par->right->left = par->right->middle;
				par->right->middle = par->right->right;
				par->right->right = NULL;
			}
			else if (par->left->key[1] != NULL) {
				if (leaf->middle == NULL) {
					leaf->middle = leaf->left;
					leaf->left = NULL;
				}
				par->middle->key[1] = par->middle->key[0];
				par->middle->info2 = par->middle->info1;
				inTable(tab, par->middle->key[1], par->middle, 1);
				par->middle->key[0] = par->key[0];
				par->middle->info1 = par->info1;
				inTable(tab, par->middle->key[0], par->middle, 0);
				par->key[0] = par->left->key[1];
				par->info1 = par->left->info2;
				inTable(tab, par->key[0], par, 0);
				par->left->key[1] = NULL;
				par->left->info2 = NULL;
				par->middle->left = par->left->right;
				if (par->middle->left != NULL)
					par->middle->left->parent = par->middle;
				par->left->right = NULL;
			}
		}
		else if (leaf == par->left) {
			if (leaf->left == NULL) {
				leaf->left = leaf->middle;
				leaf->middle = NULL;
			}
			par->left->key[0] = par->key[0];
			par->left->info1 = par->info1;
			if (par->middle->key[1] != NULL) {
				par->key[0] = par->middle->key[0];
				par->info1 = par->middle->info1;
				inTable(tab, par->key[0], par, 0);
				par->middle->key[0] = par->middle->key[1];
				par->middle->info1 = par->middle->info2;
				inTable(tab, par->middle->key[0], par->middle, 0);
				par->middle->key[1] = NULL;
				par->middle->info2 = NULL;
				par->left->middle = par->middle->left;
				if (par->left->middle != NULL)
					par->left->middle->parent = par->left;
				par->middle->left = par->middle->middle;
				par->middle->middle = par->middle->right;
				par->middle->right = NULL;
			}
			else if (par->right->key[1] != NULL) {
				par->key[0] = par->middle->key[0];
				par->info1 = par->middle->info1;
				inTable(tab, par->key[0], par, 0);
				par->middle->key[0] = par->key[1];
				par->middle->info1 = par->info2;
				inTable(tab, par->middle->key[0], par->middle, 0);
				par->key[1] = par->right->key[0];
				par->info2 = par->right->info1;
				inTable(tab, par->key[1], par, 1);
				par->right->key[0] = par->right->key[1];
				par->right->info1 = par->right->info2;
				inTable(tab, par->right->key[0], par->right, 0);
				par->right->key[1] = NULL;
				par->right->info2 = NULL;
				par->left->middle = par->middle->left;
				if (par->left->middle != NULL)
					par->left->middle->parent = par->left;
				par->middle->left = par->middle->middle;
				par->middle->middle = par->right->left;
				if (par->middle->middle != NULL)
					par->middle->middle->parent = par->middle;
				par->right->left = par->right->middle;
				par->right->middle = par->right->right;
				par->right->right = NULL;
			}
		}
	}
	else if (par->key[1] == NULL) {
		leaf->key[0] = par->key[0];
		leaf->info1 = par->info1;
		inTable(tab, leaf->key[0], leaf, 0);
		if (leaf == par->left && par->middle->key[1] != NULL) {
			par->key[0] = par->middle->key[0];
			par->info1 = par->middle->info1;
			inTable(tab, par->key[0], par, 0);
			par->middle->key[0] = par->middle->key[1];
			par->middle->info1 = par->middle->info2;
			inTable(tab, par->middle->key[0], par->middle, 0);
			par->middle->key[1] = NULL;
			par->middle->info2 = NULL;
			if (leaf->left == NULL)
				leaf->left = leaf->middle;
			leaf->middle = par->middle->left;
			par->middle->left = par->middle->middle;
			par->middle->middle = par->middle->right;
			par->middle->right = NULL;
			if (leaf->middle != NULL) {
				leaf->middle->parent = leaf;
			}
		}
		else if (leaf == par->middle && par->left->key[1] != NULL) {
			par->key[0] = par->left->key[1];
			par->info1 = par->left->info2;
			inTable(tab, par->key[0], par, 0);
			par->left->key[1] = NULL;
			par->left->info2 = NULL;
			if (leaf->middle == NULL)
				leaf->middle = leaf->left;
			leaf->left = par->left->right;
			par->left->right = NULL;
			if (leaf->left != NULL)
				leaf->left->parent = leaf;
		}
	}
	
	return par;
}

Tree* fix_tree_second(Tree* leaf, Table*tab) {
	Tree* par = leaf->parent;
	if (leaf->key[0] != NULL)
		return par;
	if (leaf == par->left) {
		par->middle->key[1] = par->middle->key[0];
		par->middle->info2 = par->middle->info1;
		inTable(tab, par->middle->key[1], par->middle, 1);
		par->middle->key[0] = par->key[0];
		par->middle->info1 = par->info1;
		inTable(tab, par->middle->key[0], par->middle, 0);
		par->middle->right = par->middle->middle;
		//par->middle->right->parent = par->middle;
		par->middle->middle = par->middle->left;
		//par->middle->middle->parent = par->middle;
		if (leaf->left != NULL)
			par->middle->left = leaf->left;
		else if (leaf->middle != NULL)
			par->middle->left = leaf->middle;
		if (par->middle->left != NULL)
			par->middle->left->parent = par->middle;
		par->key[0] = par->key[1];
		par->info1 = par->info2;
		inTable(tab, par->key[0], par, 0);
		par->key[1] = NULL;
		par->info2 = NULL;
		par->left = NULL;
		//par->left = par->middle;
		//par->middle = par->right;
	}
	else if (leaf == par->middle) {
		par->left->key[1] = par->key[0];
		par->left->info2 = par->info1;
		inTable(tab, par->left->key[1], par->left, 1);
		if (leaf->left != NULL)
			par->left->right = leaf->left;
		else if (leaf->middle != NULL)
			par->left->right = leaf->middle;
		if (par->left->right != NULL)
			par->left->right->parent = par->left;
		par->key[0] = par->key[1];
		par->key[1] = NULL;
		par->info1 = par->info2;
		par->info2 = NULL;
		par->middle = NULL;
		inTable(tab, par->key[0], par, 0);
	}
	if (par->parent == NULL) {
		Tree* new = NULL;
		if (par->left != NULL)
			new = par->left;
		else
			new = par->middle;
		new->parent = NULL;
		return new;
	}
	free(leaf);
	return par;
}

int tree_remove(Tree** tree, int key, Table*tab) {
	int a;
	Tree* our = tree_find(tree, key, &a, tab);
	Tree* o = tree_find(tree, key, &a, tab);
	if (our == NULL)
		return NOT_FOUND;
	if (our->left != NULL) {            // if our is not a leaf
		if (key == our->key[0]) {
			our = find_min(&our->middle);
			if (our == NULL)
				our = o;
		}
		else {
			our = find_min(&our->right);
			if (our == NULL)
				our = o;
		}
	}
	if (o != our) {                    // our is not a leaf
		int k = key;                   // changing our info and next info for deleting infi in leaf
		InfoType* i = NULL;
		if (key == o->key[0]) {
			i = o->info1;
			o->key[0] = our->key[0];
			o->info1 = our->info1;
			inTable(tab, o->key[0], o, 0);
			our->key[0] = k;
			our->info1 = i;
			inTable(tab, k, our, 0);
		}
		else {
			i = o->info2;
			o->key[1] = our->key[0];
			o->info2 = our->info1;
			inTable(tab, o->key[1], o, 1);
			our->key[0] = k;
			our->info1 = i;
			inTable(tab, our->key[0], our, 0);
		}	
	}
	if (our->key[1] != NULL) { // if 2 keys in our
		if (key == our->key[0]) {
			our->key[0] = our->key[1];
			our->key[1] = NULL;
			InfoType * i = our->info1;
			our->info1 = our->info2;
			our->info2 = NULL;
			inTable(tab, our->key[0], our, 0);
			free(i);
		}
		else {
			our->key[1] = NULL;
			our->info2 = NULL;
		}
		return SUCCESS;
	}
	if (our == *tree) { // our is root
		if (our->key[1] == NULL) {
			*tree = NULL;
			return SUCCESS;
		}
	}
	our->key[0] = NULL;
	our->info1 = NULL;
	Tree* par = our->parent;
	while (par != NULL) {
		if (par->left->key[1] != NULL || par->middle->key[1] != NULL || par->key[1] != NULL)
			our = fix_tree_first(our, tab);
		else if (par->key[1] != NULL && par->right->key[1] != NULL)
			our = fix_tree_first(our, tab);
		else
			our = fix_tree_second(our, tab);
		par = our->parent;
	}
	*tree = our;
	return SUCCESS;
}

int delTree(Tree** tree) {
	Tree* ptr = *tree;
	int a;
	if (ptr != NULL) {
		delTree(&ptr->right);
		delTree(&ptr->middle);
		delTree(&ptr->left);
		free(ptr->info1);
		free(ptr->info2);
	}
	free(ptr);
	return 1;
}

void show_tree_graphviz(Tree* tree, FILE* fd, int* n) {
	if (tree != NULL) {
		if (tree->left != NULL) {
			fprintf(fd, "\t\"%d  |  %d\" -> \"%d  |  %d\";\n", tree->key[0], tree->key[1], tree->left->key[0], tree->left->key[1]);
			if (tree->middle != NULL) {
				fprintf(fd, "\t\"%d  |  %d\" -> \"%d  |  %d\";\n", tree->key[0], tree->key[1], tree->middle->key[0], tree->middle->key[1]);
				if (tree->right != NULL) {
					fprintf(fd, "\t\"%d  |  %d\" -> \"%d  |  %d\";\n", tree->key[0], tree->key[1], tree->right->key[0], tree->right->key[1]);
					show_tree_graphviz(tree->left, fd, n);
					show_tree_graphviz(tree->middle, fd, n);
					show_tree_graphviz(tree->right, fd, n);
				}
				else {
					char* null = elist(*n);
					fprintf(fd, "\t%s[shape = point];\n", null);
					fprintf(fd, "\t\"%d  |  %d\" -> \"%s\";\n", tree->key[0], tree->key[1], null);
					(*n)++;
					show_tree_graphviz(tree->left, fd, n);
					show_tree_graphviz(tree->middle, fd, n);
				}
			}
			else {
				char* null = elist(*n);
				fprintf(fd, "\t%s[shape = point];\n", null);
				fprintf(fd, "\t\"%d  |  %d\" -> \"%s\";\n", tree->key[0], tree->key[1], null);
				(*n)++;
				null = elist(*n);
				fprintf(fd, "\t%s[shape = point];\n", null);
				fprintf(fd, "\t\"%d  |  %d\" -> \"%s\";\n", tree->key[0], tree->key[1], null);
				(*n)++;
				show_tree_graphviz(tree->left, fd, n);
			}
		}
		/*else  if (tree->right != NULL) {

			if (tree->left != NULL) {
				fprintf(fd, "\t\"%d\" -> \"%d\";\n", tree->key, tree->left->key);
				show_tree_graphviz(tree->left, fd, n);
				show_tree_graphviz(tree->right, fd, n);
				fprintf(fd, "\t\"%d\" -> \"%d\";\n", tree->key, tree->right->key);
			}

			else {
				char* null = elist(*n);
				fprintf(fd, "\t%s[shape = point];\n", null);
				fprintf(fd, "\t\"%d\" -> \"%s\";\n", tree->key, null);
				(*n)++;
				fprintf(fd, "\t\"%d\" -> \"%d\";\n", tree->key, tree->right->key);
				show_tree_graphviz(tree->right, fd, n);
			}
		}
		else {
			char* null = elist(*n);
			fprintf(fd, "\t\"%d\" -> \"%s\";\n", tree->key, null);
			(*n)++;
			null = elist(*n);
			fprintf(fd, "\t\"%d\" -> \"%s\";\n", tree->key, null);
			(*n)++;
		}*/
	}
}