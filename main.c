#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dialog.h"



int main()
{
	Tree* tree = NULL;
	Table table = { 10, NULL };
	initTable(&table);
	int rc;
	if (D_Load(&tree, &table) == 0)
		return 1;
	while (rc = dialog(msgs, NMsgs))
		if (!fptr[rc](&tree, &table))
			break;
	printf("That's all. Bye!\n");
	delTree(&tree);
	return 0;
	//printf("1230");
}