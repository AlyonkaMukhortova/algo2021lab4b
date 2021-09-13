#ifndef INFO_H
#define INFO_H

#define SIZE 10

typedef struct InfoType {
	int num1;
	int num2;
	char* str;
	int offset;
}InfoType;

enum Errors_e {
	SUCCESS = 0,
	NOT_FOUND = -2,
	EXISTS = -3
};

InfoType* create_info(char* str1, char* str2, int offset);
void print_info(InfoType* info);
InfoType* copy_info(InfoType* info);
char* elist(int num);


#endif //INFO_H

#pragma once
