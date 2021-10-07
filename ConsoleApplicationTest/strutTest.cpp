#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma pack(4)
typedef struct _A {
	char c;
	int i;
	short s;
}A;

typedef struct _B {
	A a;
	double d;

}B;

typedef struct _C {
	int i;

char зу;

short s;

}C;

typedef struct _D{
	char C;

char ast[101]; }D;

typedef class H {
	H();

	~H();

	virtual int a();

	virtual char*b()= 0; char C;

	int b(int i);

};

#pragma pack()
int main(){

printf("%d %d %d %d %d\n", sizeof(A), sizeof(B), sizeof(C), sizeof(D), sizeof(H));
printf("%d %d %d %d %d\n", sizeof(char), sizeof(int), sizeof(short), sizeof(D), sizeof(H)); return 0;

}