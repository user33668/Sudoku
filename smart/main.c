#include "Sudoku.h"
#include <time.h>

int main(int argc, const char *argv[]){
	clock_t time = clock();
	if(argc != 2)return -1;

	FILE *fp = fopen(argv[1],"r");

	//open the file
	if(fp == NULL){
		printf("can't find this file : %s\n",argv[1]);
		return -1;
	}
	//init the Square
	Square S;
	if(!Init_Square(&S)){
		printf("init error, no enough memory\n");
		fclose(fp);
		return -1;
	}

	if(-1 == input(&S, fp)){
		printf("the file is not right\n");
		fclose(fp);
		return -1;
	}
	if(S.error){
		printf(" some numbers are not right\n");
		fclose(fp);
		return -1;
	}
	fclose(fp);
	//solve the puzzle
	int a = 0;
	a = solve(&S);
	//show the result
	if(a == -1)
		printf("There is no solution to this puzzle\n");

	if(a == -2)
		printf("This program did not have a answer to the puzzle\n");

	show_square(&S);
	if(S.guess)
		printf("There may be other solutions\n");
	free_Square(&S);
	time = clock()-time;
	double t = (double) time / CLOCKS_PER_SEC;
	printf("using time %f\nany key to exit\n",t);
	getchar();
}

