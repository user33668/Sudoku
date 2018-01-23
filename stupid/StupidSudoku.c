#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct s_node{
	int position[2];
	int cule[9];
	struct s_node * nest;
}s_node;

int static count = 0;

int try_put_in_row(const int S[9][9], s_node * node, int k){
	for(int j = 0; j < 9; j++)
		if(S[node -> position[0]][j] == k) return -1;
	return 1;
}

int try_put_in_column(const int S[9][9], s_node * node, int k){
	for(int i = 0; i < 9; i++)
		if(S[i][node -> position[1]] == k) return -1;
	return 1;
}

int try_put_in_box(const int S[9][9], s_node * node, int t){
	int i = node -> position[0];
	int j = node -> position[1];
	for(int k = 0; k < 9; k++)
		if(S[i/3*3+k/3][j/3*3+k%3] == t) return -1;
	return 1;
}

int try_put(const int S[9][9], s_node * node, int k){
	if(-1 == try_put_in_row(S, node, k))     return -1;//if(-1) 的值为真
	if(-1 == try_put_in_column(S, node, k)) return -1;
	if(-1 == try_put_in_box(S, node, k))     return -1;
	return 1;
}

int input(int S[9][9], FILE *fp){
	int c = 0;
	int t = 0;
	for(int i = 0; i < 9; i++)
	for(int j = 0; j < 9; j++){
		c = fgetc(fp);
		if(c == EOF){printf("the file is not right.\n"); return -1;}
		t = c - '0';
		if(t < 0 || t > 9){
			j--;
			continue;
		}else
			S[i][j] = t;
	}
	return 1;
}

s_node * init_cule(const int S[9][9]){
	s_node * begain = (s_node *)malloc(sizeof(s_node));
	if (begain == NULL)return NULL;
	s_node * corrent = begain;
	corrent -> nest =NULL;

	for(int i = 0; i < 9; i++)
	for(int j = 0; j < 9; j++){
		if(S[i][j] != 0) continue;
		corrent -> nest = (s_node *)malloc(sizeof(s_node));
		corrent = corrent -> nest;
		corrent -> position[0] = i;
		corrent -> position[1] = j;
		for(int k = 1; k < 10; k++){
			if(1 == try_put(S, corrent, k))
				corrent -> cule[k-1] = 1;
			else
				corrent -> cule[k-1] = 0;
		}
		corrent-> nest = NULL;
	}

	//第一个节点为空，将其释放掉。
	corrent = begain;
	begain = begain -> nest;
	free(corrent);
	corrent = NULL;
	return begain;
}
void show_solution(const int S[9][9]){
    printf("solution : %d\n",count);
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			printf("%d ",S[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


void Stupid_find_all(int S[9][9], s_node * node){
	for(int i = 0; i < 9; i++){
		if(node -> cule[i] == 0) continue;
		if(1 == try_put(S, node, i+1)){
			S[node -> position[0]][node -> position[1]] = i+1;
			if(node -> nest == NULL){
				count++;
				show_solution(S);
				S[node -> position[0]][node -> position[1]] = 0;
				break;
			}
			Stupid_find_all(S, node -> nest);
                        S[node -> position[0]][node -> position[1]] = 0;
                }
        }
}

int main(int argc, const char *argv[]){
	clock_t time = clock();
	if(argc != 2)return -1;

        //open the file
	FILE *fp = fopen(argv[1],"r");
	if(fp == NULL){
		printf("can't find this file : %s\n",argv[1]);
		return -1;
	}
        
	//init the Square
	int S[9][9];
	if(-1 == input(S, fp)){
		printf("the file is not right\n");
		fclose(fp);
		return -1;
	}
        //find all the solution
	Stupid_find_all(S, init_cule(S));
        
        //show resualt
	double total = ((double)(clock() - time))/CLOCKS_PER_SEC;
	if(count != 0)
		printf("find all solution，using time %.10lf second\n",total);
	else
		printf("not find solution，using time %.10lf second\n",total);
        printf("any key to exit\n");
	getchar();
}


