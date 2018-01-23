
#include <stdlib.h>
#include <stdio.h>

typedef struct{
	int num;
	int cule[10];
	//int is_guess;
}unit;

typedef struct{
	unit * gird[9][9];
	int row_cule[9][10];
	int column_cule[9][10];
	int box_cule[9][10];

	int error;
	int cule;
	int unknow;
	int guess;
}Square;

int Init_Square(Square * S);
void free_Square(Square * S);
int input(Square * S,FILE *fp);
void delete_cule(Square *S, int i, int j, int t);
void put(Square * S, int i, int j, int t );
int solve(Square *S);
void copy_Square(Square * S, Square * S_copy);
void renew_cule(Square *S);
void show_square(Square * S);



int Init_Square(Square * S){
	for (int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			S->gird[i][j] = (unit *)malloc(sizeof(unit));
			if(S->gird[i][j] == NULL)
				return -1;

			// init the gird
			S->gird[i][j]->num 	= 0;
			S->gird[i][j]->cule[0]	= 9;
			for(int k = 1; k < 10; k++)
				S->gird[i][j]->cule[k] = 1;

			//init row, column and box
			S->row_cule[i][j+1] 	= 9;
			S->column_cule[i][j+1] 	= 9;
			S->box_cule[i][j+1] 	= 9;
		}
		//count the num
		S->row_cule[i][0] 	= 9;
		S->column_cule[i][0] 	= 9;
		S->box_cule[i][0] 	= 9;
	}

	S->guess 	= 0;
	S->error 	= 0;
	S->cule 	= 729;
	S->unknow 	= 81;
	//printf("init complated\n");
	//show_square(S);
	return 1;
}


void free_Square(Square * S){
	for (int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			free(S->gird[i][j]);
}


int input(Square * S, FILE *fp){
	int c = 0;
	int t = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			c = fgetc(fp);
			if(c == EOF){printf("the file is not right.\n"); return -1;}
			t = c - '0';
			if(t < 0 || t > 9){
				j--;
				continue;
			}else if (t == 0)
				continue;
			else
                put(S, i, j, t);
		}
		//show_square(S);
	}
	return 1;
}


void delete_cule(Square *S, int i, int j, int t){
	if(S->gird[i][j]->cule[t] == 1){
		S->gird[i][j]->cule[t] = 0;

		S->gird[i][j]->cule[0]--;
		S->row_cule[i][t]--;
		S->column_cule[j][t]--;
		S->box_cule[i/3*3+j/3][t]--;
		S->cule--;
	}
	if(S->gird[i][j]->num == 0 && S->gird[i][j]->cule[0] == 0)
		S->error = 1;
}


void put(Square * S, int i, int j, int t ){ //put t in S[i][j],1 <= t <= 9, and renew the square;
	if(t<1 || t>9) return;
	//printf("put %d in(%d,%d)\n", t, i+1, j+1);
	S->gird[i][j]->num = t;  //the only write a number
	S->unknow--;
	S->row_cule[i][0]--;
	S->column_cule[j][0]--;
	S->box_cule[i/3*3+j/3][0]--;

	for(int k = 1; k < 10; k++)
		delete_cule(S, i, j, k);

	//renew row
	for(int k = 0; k < 9; k++){
		if(k == j) continue;
		delete_cule(S, i, k, t);
	}

	//renew column
	for(int k = 0; k < 9; k++){
		if(k == i) continue;
		delete_cule(S, k, j, t);
	}

	//renew gird
	for(int m = (i/3*3); m < (i/3*3+3); m++)
		for(int n = (j/3*3); n < (j/3*3+3); n++){
			if(m == i||n == j) continue;
			delete_cule(S, m, n, t);
		}
}


void renew_cule(Square *S){
	int change = S->cule;

	/******************************************************/
	/****************以下为更新算法***********************/

	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++){
			if(S->gird[i][j]->cule[0] == 1)
				for(int k = 1; k < 10; k++)
					if(S->gird[i][j]->cule[k] == 1)
						put(S, i, j, k);

			if (S->gird[i][j]->cule[0] == 2) {
				int unit[2] = {0, 0};
				int count = 0;
				//find it
				for(int k = 1; k < 10; k++){
					if(S->gird[i][j]->cule[k] == 1){
						unit[count] = k;
						count++;
					}
					if (count == 2)
						break;
				}

				//row
				for(int m = 0; m < 9; m++){
					if(m == j)continue;
					if(S->gird[i][m]->cule[0] == 2 &&
							S->gird[i][m]->cule[unit[0]] == 1 &&
							S->gird[i][m]->cule[unit[1]] == 1)
						for(int n = 0; n < 9; n++){
							if(n == m || n == j)continue;
							delete_cule(S, i, n, unit[0]);
							delete_cule(S, i, n, unit[1]);
						}
				}

				//column
				for(int m = 0; m < 9; m++){
					if(m == i)continue;
					if(S->gird[m][j]->cule[0] == 2 &&
							S->gird[m][j]->cule[unit[0]] == 1 &&
							S->gird[m][j]->cule[unit[1]] == 1)
						for(int n = 0; n < 9; n++){
							if(n == m || n == i)continue;
							delete_cule(S, n, j, unit[0]);
							delete_cule(S, n, j, unit[1]);
						}
				}

				//box
				for(int m = 0; m < 9; m++){
					if(m == (i%3)*3+j%3)continue;
					if(S->gird[i/3*3+m/3][j/3*3+m%3]->cule[0] == 2 &&
							S->gird[i/3*3+m/3][j/3*3+m%3]->cule[unit[0]] == 1 &&
							S->gird[i/3*3+m/3][j/3*3+m%3]->cule[unit[1]] == 1)
						for(int n = 0; n < 9; n++){
							if(n == m || n == (i%3)*3+j%3)continue;
							delete_cule(S, i/3*3+n/3, j/3*3+n%3, unit[0]);
							delete_cule(S, i/3*3+n/3, j/3*3+n%3, unit[1]);
						}
				}
			}
		}

	for(int i = 0; i < 9; i++)
		for (int k = 1; k < 10; k++){
			//if 1 put it in
			if(S->row_cule[i][k] == 1){
				for(int j = 0; j < 9; j++)
					if(S->gird[i][j]->cule[k] == 1)
						put(S, i, j, k);
			}

            if(S->column_cule[i][k] == 1){
                for(int j = 0; j < 9; j++)
                    if(S->gird[j][i]->cule[k] == 1)
                        put(S, j, i, k);
            }

            if(S->box_cule[i][k] == 1){
                for(int j = 0; j < 9; j++)
                    if(S->gird[i-i%3+j/3][(i%3)*3+j%3]->cule[k] == 1){
                        put(S,i-i%3+j/3, (i%3)*3+j%3, k);
                    }
            }


			//if 2
			if(S->row_cule[i][k] == 2){
				int unit[2] = {0, 0};
				int count = 0;
				//find its
				for(int j = 0; j < 9; j++){
					if(S->gird[i][j]->cule[k] == 1){
						unit[count] = j;
						count++;
					}
					if (count == 2)
						break;
				}
				//the 2 are in one box
				if(unit[0]/3 == unit[1]/3)
					for(int m = (i/3*3); m < (i/3*3+3); m++){
						if(m == i) continue;
						for(int n = (unit[0]/3*3); n < (unit[0]/3*3+3); n++)
							delete_cule(S, m, n, k);
					}

				//
				int compare[2] ={0, 0};
				count = 0;
				for(int c = k+1; c < 10; c++)
					if(S->row_cule[i][c] == 2){
						for(int j = 0; j < 9; j++)
							if(S->gird[i][j]->cule[c] == 1){
								compare[count] = j;
								count++;
							}
						if(unit[0] == compare[0] && unit[1] == compare[1])
							for(int p = 1; p < 10; p++)
								if(p != k && p != c){
									delete_cule(S, i, unit[0], p);
									delete_cule(S, i, unit[1], p);
								}
						count = 0;
					}

				//
				for(int c = i+1; c < 9; c++){
					if(S->row_cule[c][k] == 2 &&
							S->gird[c][unit[0]]->cule[k] == 1 &&
							S->gird[c][unit[1]]->cule[k] == 1)
						for(int m = 0; m < 9; m ++){
							if(m == c || m == i)continue;
							delete_cule(S, m , unit[0], k);
							delete_cule(S, m , unit[1], k);


						}
				}

			}

			if(S->column_cule[i][k] == 2){
				int unit[2] = {0, 0};
				int count = 0;
				//find its
				for(int j = 0; j < 9; j++){
					if(S->gird[j][i]->cule[k] == 1){
						unit[count] = j;
						count++;
					}
					if (count == 2)
						break;
				}
				//the 2 are in one box
				if(unit[0]/3 == unit[1]/3)
					for(int m = (i/3*3); m < (i/3*3+3); m++){
						if(m == i) continue;
						for(int n = (unit[0]/3*3); n < (unit[0]/3*3+3); n++)
							delete_cule(S, n, m, k);
					}

				//
				int compare[2] ={0, 0};
				count = 0;
				for(int c = k+1; c < 10; c++)
					if(S->column_cule[i][c] == 2){
						for(int j = 0; j < 9; j++)
							if(S->gird[j][i]->cule[c] == 1){
								compare[count] = j;
								count++;
							}
						if(unit[0] == compare[0] && unit[1] == compare[1])
							for(int p = 1; p < 10; p++)
								if(p != k && p != c){
									delete_cule(S, unit[0], i, p);
									delete_cule(S, unit[1], i, p);
								}
						count = 0;
					}

				for(int c = i+1; c < 9; c++){
					if(S->column_cule[c][k] == 2 &&
							S->gird[unit[0]][c]->cule[k] == 1 &&
							S->gird[unit[1]][c]->cule[k] == 1)
						for(int m = 0; m < 9; m ++){
							if(m == c || m == i)continue;
							delete_cule(S, unit[0], m, k);
							delete_cule(S, unit[1], m, k);
						}
				}

			}

			if(S->box_cule[i][k] == 2){
				int unit[2] = {0, 0};
				int count = 0;
				//find its
				for(int j = 0; j < 9; j++){
					if(S->gird[i-i%3+j/3][(i%3)*3+j%3]->cule[k] == 1){
						unit[count] = j;
						count++;
					}
					if (count == 2)
						break;
				}
				//the 2 are in one row
				if(unit[0]/3 == unit[1]/3)
					for(int m = 0; m < 9; m++){
						if(m/3 == i%3) continue;
						delete_cule(S, i-i%3+unit[0]/3, m, k);
					}

				//the 2 are in one column
				if(unit[0]%3 == unit[1]%3)
					for(int m = 0; m < 9; m++){
						if(m/3 == i/3) continue;
						delete_cule(S, m, (i%3)*3+unit[0]%3, k);
					}

				//
				int compare[2] ={0, 0};
				count = 0;
				for(int c = k+1; c < 10; c++)
					if(S->box_cule[i][c] == 2){
						for(int j = 0; j < 9; j++)
							if(S->gird[i-i%3+j/3][(i%3)*3+j%3]->cule[c] == 1){
								compare[count] = j;
								count++;
							}
						if(unit[0] == compare[0] && unit[1] == compare[1])
							for(int p = 1; p < 10; p++)
								if(p != k && p != c){
									delete_cule(S, i-i%3+unit[0]/3, (i%3)*3+unit[0]%3, p);
									delete_cule(S, i-i%3+unit[1]/3, (i%3)*3+unit[1]%3, p);
								}
						count = 0;
					}
			}

			//if 3
			if(S->row_cule[i][k] == 3){
				int unit[3] = {0, 0,0};
				int count = 0;
				//find its
				for(int j = 0; j < 9; j++){
					if(S->gird[i][j]->cule[k] == 1){
						unit[count] = j;
						count++;
					}
					if (count == 3)
						break;
				}
				//the 2 are in one box
				if(unit[0]/3 == unit[1]/3 && unit[0]/3 == unit[2]/3)
					for(int m = (i/3*3); m < (i/3*3+3); m++){
						if(m == i) continue;
						for(int n = (unit[0]/3*3); n < (unit[0]/3*3+3); n++)
							delete_cule(S, m, n, k);
					}
			}

			if(S->column_cule[i][k] == 3){
				int unit[3] = {0, 0, 0};
				int count = 0;
				//find its
				for(int j = 0; j < 9; j++){
					if(S->gird[j][i]->cule[k] == 1){
						unit[count] = j;
						count++;
					}
					if (count == 3)
						break;
				}
				//the 2 are in one box
				if(unit[0]/3 == unit[1]/3 && unit[0]/3 == unit[2]/3)
					for(int m = (i/3*3); m < (i/3*3+3); m++){
						if(m == i) continue;
						for(int n = (unit[0]/3*3); n < (unit[0]/3*3+3); n++)
							delete_cule(S, n, m, k);
					}
			}

			if(S->box_cule[i][k] == 3){
				int unit[3] = {0, 0, 0};
				int count = 0;
				//find its
				for(int j = 0; j < 9; j++){
					if(S->gird[i-i%3+j/3][(i%3)*3+j%3]->cule[k] == 1){
						unit[count] = j;
						count++;
					}
					if (count == 3)
						break;
				}
				//the 2 are in one row
				if(unit[0]/3 == unit[1]/3 && unit[0]/3 == unit[2]/3)
					for(int m = 0; m < 9; m++){
						if(m/3 == i%3) continue;
						delete_cule(S, i-i%3+unit[0]/3, m, k);
					}

				//the 2 are in one column
				if(unit[0]%3 == unit[1]%3 && unit[0]%3 == unit[2]%3)
					for(int m = 0; m < 9; m++){
						if(m/3 == i/3) continue;
						delete_cule(S, m, (i%3)*3+unit[0]%3, k);
					}
			}
		}

	/******************算法结束****************************/
	/******************************************************/
	change -=S->cule;

	//有三种可能的返回情况
	//1. 有错误 ：S->error == 1；
	//2. 解决谜题：S->unknow == 0
	//3. 算法未能解决谜题。
	if(S->error || S->unknow == 0) return;
	if(change != 0)renew_cule(S);
	return;
}


int solve(Square *S){
	renew_cule(S);
	if(S->unknow == 0) return 1;
	if(S->error) return -1;

	Square S_copy;
	Init_Square(&S_copy);

	int change = 0;
	int key[3] ={0,0,0};
	do{
		change = S->cule;
		for(int i = 0; i < 9; i++)
			for(int j = 0; j < 9; j++){
				if(S->gird[i][j]->num != 0)continue;
				for(int k = 1; k < 10; k++){
					if(S->gird[i][j]->cule[k]){
						copy_Square(S, &S_copy);
						put(&S_copy, i, j, k);
						renew_cule(&S_copy);
						if(S_copy.error){
							delete_cule(S, i, j, k);
							renew_cule(S);
							if(S->unknow == 0) {
								free_Square(&S_copy);
								return 1;
							}
							if(S->error) {
								free_Square(&S_copy);
								return -1;
							}
							continue;
						}
						if(S_copy.unknow == 0){
							key[0] = k;
							key[1] = i;
							key[2] = j;
						}
					}
				}
			}
		change -=S->cule;
	}while(change);

	if(key[0]){
		S->guess = 1;
		put(S, key[1], key[2], key[0]);
		renew_cule(S);
		free_Square(&S_copy);
		return 1;
	}
	free_Square(&S_copy);
	return -2;
}


void copy_Square(Square * S, Square * S_copy){
	for (int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			S_copy->gird[i][j]->num     = S->gird[i][j]->num;
			for(int k = 0; k < 10; k++)
				S_copy->gird[i][j]->cule[k] = S->gird[i][j]->cule[k];

			//copy row, column and box
			S_copy->row_cule[i][j+1]    = S->row_cule[i][j+1];
			S_copy->column_cule[i][j+1] = S->column_cule[i][j+1];
			S_copy->box_cule[i][j+1]    = S->box_cule[i][j+1];
		}
		//count the num
		S_copy->row_cule[i][0]      = S->row_cule[i][0];
		S_copy->column_cule[i][0]   = S->column_cule[i][0];
		S_copy->box_cule[i][0]      = S->box_cule[i][0];
	}

	S_copy->error   = S->error;
	S_copy->cule    = S->cule;
	S_copy->unknow  = S->unknow;
}


void show_square(Square * S){
	//printf("gird\n");
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++)
			printf("%d ",S->gird[i][j]->num);
		printf("\n");
	}
	return;
	printf("unknow: %d\n",S->unknow);
	printf("row_cule\n");
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 10; j++)
			printf("%d ",S->row_cule[i][j]);
		printf("\n");
	}
	printf("column\n");
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 10; j++)
			printf("%d ",S->column_cule[i][j]);
		printf("\n");
	}
	printf("box_cule\n");
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 10; j++)
			printf("%d ",S->box_cule[i][j]);
		printf("\n");
	}

}


