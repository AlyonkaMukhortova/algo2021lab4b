#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "info.h"

InfoType* create_info(int num1, int num2, char* str, int offset) {
	InfoType* new_info = (InfoType*)malloc(sizeof(InfoType));
	new_info->num1 = num1;
	new_info->num2 = num2;
	new_info->str = str;
	new_info->offset = offset;
	return new_info;
}



void print_info(InfoType* info) {
	InfoType* ptr = info;
	if (ptr != NULL) {
		printf(", %d, %d, %s, %d", ptr->num1, ptr->num2, ptr->str, ptr->offset);
		printf("}\n");
	}
}

InfoType* copy_info(InfoType* info) {
	InfoType* new_info = (InfoType*)malloc(sizeof(InfoType));
	new_info->num1 = info->num1;
	new_info->num2 = info->num2;
	new_info->str = info->str;
	new_info->offset = info->offset;
	return new_info;
}

char* from_int_to_str(int num) {
	char* str = NULL;
	int size = 0;
	int num1 = num;
	while (num1 > 0) {
		num1 = num1 / 10;
		size++;
	}
	if (num == 0)
		size++;
	str = (char*)malloc((size + 1) * sizeof(char));
	if (size < 2) {
		str[0] = num + '0';
		str[1] = '\0';
	}

	else {
		int count = size - 1;
		while (num > 0) {
			str[count] = (num % 10) + '0';
			num = num / 10;
			count--;
		}
	}
	str[size] = '\0';
	return str;
}

char* elist(int num) {
	char* number = from_int_to_str(num);
	int i = strlen(number);
	char* null = (char*)malloc(5 + i);
	//	scanf("[\n]\n", null);
	char* n = "null";
	null = strcpy(null, n);
	null = strcat(null, number);
	return null;
}