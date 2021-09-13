#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dialog.h"

(*fptr[])(Tree**, Table*) = { 0, D_Add, D_Find, D_Delete, D_Show, D_Timing, D_Count };

const char* msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Timing", "6. Count"};

const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int write_binary() {
	printf("Enter text file name -->\n");
	char* tf_name = NULL;
	tf_name = getstr();
	printf("Enter binary file name -->\n");
	char* bf_name = NULL;
	bf_name = getstr();
	int t = 0;
	FILE* fdt = NULL;
	FILE* fdb = NULL;
	fdt = fopen(tf_name, "r");
	fdb = fopen(bf_name, "w + b");
	while (t != EOF) {
		int key = 0;
		t = fget_int(&key, fdt);
		if (t != EOF)
			fwrite(&key, sizeof(int), 1, fdb);
	}
	return 0;
}

int get_int(int* a) {
	int n;
	do {
		n = scanf("%d", a);
		if (n == 0) {
			printf("Error\n");
			scanf("%*c", 0);
		}
	} while (n == 0);
	return n < 0 ? 0 : 1;
}

int get_uint(int* a) {
	int n;
	do {
		n = scanf("%d", a);
		if (n == 0) {
			printf("Error\n");
			scanf("%*c", 0);
		}
		if (*a < 0)
			printf("Error\n");
	} while (n == 0 || *a < 0);
	return n < 0 ? 0 : 1;
}


char* getstr()
{
	char* ptr = (char*)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			free(ptr);
			ptr = NULL;
			continue;
			//break;
		}
		if (n == 0) {
			scanf("%*c");
			//break;
		}
		else {
			len += strlen(buf);
			ptr = (char*)realloc(ptr, len + 1);
			int k = strlen(buf);
			int l = strlen(ptr) + k + 1;
			strncat_s(ptr, l, buf, k);
		}
	} while (n > 0);
	return ptr;
}

int dialog(const char* msgs[], int N)
{
	char* errmsg = "";
	int rc;
	int i, n;
	do {
		puts(errmsg);
		errmsg = "You are wrong. Repeate, please!";
		for (i = 0; i < N; ++i)
			puts(msgs[i]);
		puts("Make your choice: --> ");
		n = get_int(&rc);
		if (n == 0)
			rc = 0;
	} while (rc < 0 || rc >= N);
	return rc;
}

int D_Add(Tree** tree, Table* tab) {
	int key, num1, num2;
	char* strinfo = "\0", * s;
	//char* key1 = key;
	printf("Enter key \n");
	get_uint(&key);
	printf("Enter num 1 for item\n");
	get_int(&num1);
	printf("Enter num 1 for item\n");
	get_int(&num2);
	printf("Enter string for item\n");
	scanf("%*c");
	strinfo = getstr();
	InfoType* info = create_info(num1, num2, strinfo, 0);
	int res = tree_insert(tree, key, info,tab);
	if (res == SUCCESS)
		printf("Item has been added successfully\n");
	else
		printf("Key exists in table.\n");
}

int D_Find(Tree** tree, Table* tab) {
	Tree* result = NULL;
	int n = 0, p;
	printf("1. FInd by key\n2. Find next\n");
	get_int(&p);
	if (p == 1) {
		printf("Enter int key\n");
		int key = 0;
		get_uint(&key);
		result = tree_find(tree, key, &p, tab);
		if (result != NULL)
			show_tree_only_one(&result, p);
		else
			printf("No such element in table");
	}
	else if (p == 2) {
		printf("Enter int key\n");
		int key = 0;
		get_uint(&key);
		result = tree_find(tree, key, &p, tab);
		if (result == NULL) {
			result = tree_find_leaf(tree, key);
			result = next_no(&result, key, &p);
		}
		else
			result = find_min_find(&result, &p);
			
		if (result != NULL && p != 2)
			show_tree_only_one(&result, p);
		else
			printf("No such element in table");
	}

}

int D_Show(Tree** tree, Table* tab) {
	//show_tree_straight(tree);
	printf("\n");
	int i = 0;
	while (i < tab->size) {
		printf("[%d]\n", tab->ks[i].key);
		i++;
	}
	show_tree(tree, 0);
	printf("Enter file name -->  ");
	//scanf("%*c");
	char* fname = "";
	while(strcmp(fname, "") == 0)
		fname= getstr();
	FILE* fd;
	fopen_s(&fd, fname, "w");
	fputs("digraph G{ \n", fd);
	fputs("\tgraph [ordering=\"out\"];\n", fd);
	int n = 0;
	show_tree_graphviz(*tree, fd, &n);
	fputs("}", fd);
	fclose(fd);
	printf("\n");
	return 1;
}

int D_Delete(Tree** tree, Table* tab) {
	printf("Enter key\n");
	int key;
	int j = get_uint(&key);
	j = tree_remove(tree, key, tab);
	if (j == SUCCESS)
		printf("Elemet has been deleted successfully\n");
	else if (j == NOT_FOUND)
		printf("No such element in table");
	return 1;
}

int D_Load(Tree** tree, Table* tab)
{
	char* fname = NULL;
	printf("Enter file name: --> ");
	fname = getstr();
	if (fname == NULL)
		return 0;
	load(tree, fname, tab);
	free(fname);
	return 1;
}

char* fgetstr(FILE* fd)
{
	char* ptr = (char*)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do {
		n = fscanf(fd, "%80[^\n]", buf);
		if (n < 0) {
			free(ptr);
			ptr = NULL;
			continue;
			//break;
		}
		if (n == 0) {
			fscanf(fd, "%*c");
			//break;
		}
		else {
			len += strlen(buf);
			ptr = (char*)realloc(ptr, len + 1);
			int k = strlen(buf);
			int l = strlen(ptr) + k + 1;
			strncat_s(ptr, l, buf, k);
		}
	} while (n > 0);
	return ptr;
}

int fget_int(int* a, FILE* fd) {
	int n;
	do {
		n = fscanf(fd, "%d", a);
		if (n == 0) {
			printf("Error\n");
			fscanf(fd, "%*c", 0);
		}
	} while (n == 0);
	return n;
}

int load(Tree** tree, char* fname, Table* tab)
{
	FILE* fd;
	int t = 0;
	fopen_s(&fd, fname, "r");
	if (fd == NULL)
		return 0;

	while (t != EOF) {
		int key = 0;
		int num1, num2;
		t = fget_int(&key, fd);
		printf("%d", key);
		fscanf(fd, "%*c", 0);
		char* str2 = fgetstr(fd);//(char*)malloc(9999);
		//char* str2 = (char*)malloc(9999);
		t = fget_int(&num1, fd);
		t = fget_int(&num2, fd);
		//fscanf(fd, "%*c", 0);
		//fscanf(fd, "%[^\n]\n", str2);
		if (t != EOF) {
			InfoType* info = create_info(num1, num2, str2, 0);
			int res = tree_insert(tree, key, info, tab);
			
			printf("\n");
		}
	}
	D_Show(tree,tab);
	fclose(fd);
	return 1;
}

int D_Timing(Tree** nnn, Table* tab)
{
	Tree* root = NULL;
	int n = 10, key[50000], k, cnt = 100000, i, m, z;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0) {
		for (i = 0; i < 50000; ++i)
			key[i] = rand() * rand();
		for (i = 0; i < cnt; ) {
			k = rand() * rand();
			if (tree_insert(&root, k, NULL, tab) == SUCCESS) {
				++i;

			}
		}
		m = 0;
		first = clock();
		for (i = 0; i < 50000; ++i)
			if (tree_find(&root, key[i], &z, tab) != NULL)
				++m;
		last = clock();
		printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 -
			n) * cnt, last - first);
	}
	delTree(&root);
	return 1;
}

int D_Count(Tree** nnn, Table* tab) {
	Tree* tree = NULL;
	read_b(&tree, tab);
	Tree* result = NULL;
	int n = 1, p = 0;
	while (n != 0) {
		printf("0. Quit\n1. Find offset\n");
		get_int(&n);
		if (n == 1) {
			printf("Enter int key\n");
			int key = 0;
			get_uint(&key);
			result = tree_find(&tree, key, &p, tab);
			if (result != NULL)
				show_tree_only_one_offset(&result, p);
			else if (n != 0)
				printf("No such element in table");
		}
		else
			printf("Error! Enter again\n");
	}
	return 1;
}

void read_b(Tree** tree, Table* tab) {
	printf("Enter file name -->\n");
	char* file_name = NULL;
	scanf("%*c");
	file_name = getstr();
	FILE* fd = NULL;
	fd = fopen(file_name, "r+b");
	int t = 0;
	while (1) {
		int key;
		int y = ftell(fd);
		fread(&key, sizeof(int), 1, fd);
		if (feof(fd))
			break;
		InfoType* info = create_info(0, 0, "\0", 0);
		int t = tree_insert(tree, key, info, tab);
		if (t == SUCCESS)
			info->offset = y;
	}
	D_Show(tree, tab);
	fclose(fd);
}