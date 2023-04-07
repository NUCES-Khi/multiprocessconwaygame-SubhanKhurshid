#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define rows 10
#define columns 10

void InitializingMatrix(int matrix[rows][columns]) {
    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++) {
            matrix[i][j] = rand() % 2;
        }
    }
}

void PrinitngTheMatrix(int matrix[rows][columns]) {
    printf("\033[2J"); 
    printf("\033[%d;%dH", 0, 0); 
    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++) {
            if(matrix[i][j] == 1) {
                printf("* ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

int CountingNeighbours(int matrix[rows][columns], int row, int col) {
    int count = 0;
    for(int i=row-1; i<=row+1; i++) {
        for(int j=col-1; j<=col+1; j++) {
            if(i < 0 || i >= rows || j < 0 || j >= columns) {
                continue;
            } else if(i == row && j == col) {
                continue;
            } else if(matrix[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

void UpdatingTheMatrix(int matrix[rows][columns], int new_matrix[rows][columns]) {
    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++) {
            int neighbors = CountingNeighbours(matrix, i, j);
            if(matrix[i][j] == 1 && (neighbors < 2 || neighbors > 3)) {
                new_matrix[i][j] = 0;
            } else if(matrix[i][j] == 0 && neighbors == 3) {
                new_matrix[i][j] = 1;
            } else {
                new_matrix[i][j] = matrix[i][j];
            }
        }
    }
}

int main() {
    int matrix[rows][columns];
    int new_matrix[rows][columns];

    srand(time(NULL));
    InitializingMatrix(matrix);
    InitializingMatrix(new_matrix);

    while(1) {
        UpdatingTheMatrix(matrix, new_matrix);
        PrinitngTheMatrix(new_matrix);
        int pid = fork();
        if(pid == 0){
            for(int i=0; i<rows; i++) {
                for(int j=0; j<columns; j++) {
                    matrix[i][j] = new_matrix[i][j];
                }
            }
            sleep(1);
            exit(0);
        }else{
            for(int i=0; i<rows; i++) {
                for(int j=0; j<columns; j++) {
                    new_matrix[i][j] = matrix[i][j];
                }
            }
            wait(NULL);
        }
    }
    return 0;
}
