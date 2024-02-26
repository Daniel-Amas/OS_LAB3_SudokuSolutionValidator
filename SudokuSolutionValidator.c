#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define NUM_THREADS 11


int sudoku[9][9] = {
	{5, 3, 4, 6, 7, 8, 9, 1, 2},
	{6, 7, 2, 1, 9, 5, 3, 4, 8},
	{1, 9, 8, 3, 4, 2, 5, 6, 7},
	{8, 5, 9, 7, 6, 1, 4, 2, 3},
	{4, 2, 6, 8, 5, 3, 7, 9, 1},
	{7, 1, 3, 9, 2, 4, 8, 5, 6},
	{9, 6, 1, 5, 3, 7, 2, 8, 4},
	{2, 8, 7, 4, 1, 9, 6, 3, 5},
	{3, 4, 5, 2, 8, 6, 1, 7, 9}
};

/* structure for passing data to threads */
typedef struct
{
    int startRow;
    int startCol;
} parameters;

int rowResult[9];     // Array to store row validation results
int colResult[9];     // Array to store column validation results
int subgridResult[9]; // Array to store subgrid validation results

// Thread function to check all rows
void *checkRow(void *param){
    parameters *data = (parameters *)param;
    int startRow = data->startRow;

    for (int i = startRow; i < startRow + 1; i++)
    {
        int rowSet[9] = {0}; // Array to check presence of numbers 1 through 9 in a row
        for (int j = 0; j < 9; j++)
        {
            int num = sudoku[i][j];
            if (rowSet[num - 1] == 1 || num < 1 || num > 9)
            {
                rowResult[startRow] = 0; // Invalid row
                pthread_exit(NULL);
            }
            else
            {
                rowSet[num - 1] = 1;
            }
        }
    }

    rowResult[startRow] = 1; // Valid row
    pthread_exit(NULL);
}

// Thread function to check all columns
void *checkColumn(void *param){
    parameters *data = (parameters *)param;
    int startCol = data->startCol;

    for (int i = startCol; i < startCol + 1; i++)
    {
        int colSet[9] = {0}; // Array to check presence of numbers 1 through 9 in a column
        for (int j = 0; j < 9; j++)
        {
            int num = sudoku[j][i];
            if (colSet[num - 1] == 1 || num < 1 || num > 9)
            {
                colResult[startCol] = 0; // Invalid column
                pthread_exit(NULL);
            }
            else
            {
                colSet[num - 1] = 1;
            }
        }
    }

    colResult[startCol] = 1; // Valid column
    pthread_exit(NULL);
}

// Thread function to check all 3x3 subgrids
void *checkSubGrid(void *param){
    parameters *data = (parameters *)param;
    int startRow = (data->startRow) * 3;
    int startCol = (data->startCol) * 3;

    int subgridSet[9] = {0}; // Array to check presence of numbers 1 through 9 in a subgrid
    for (int i = startRow; i < startRow + 3; i++)
    {
        for (int j = startCol; j < startCol + 3; j++)
        {
            int num = sudoku[i][j];
            if (subgridSet[num - 1] == 1 || num < 1 || num > 9)
            {
                subgridResult[data->startRow * 3 + data->startCol] = 0; // Invalid subgrid
                pthread_exit(NULL);
            }
            else
            {
                subgridSet[num - 1] = 1;
            }
        }
    }

    subgridResult[data->startRow * 3 + data->startCol] = 1; // Valid subgrid
    pthread_exit(NULL);
}

int main() {
	pthread_t threads[NUM_THREADS];
	int threadIndex = 0;
	
    // Creating threads for row checks
    for (int i = 0; i < 9; i++)
    {
        parameters *data = (parameters *)malloc(sizeof(parameters));
        data->startRow = i;
        pthread_create(&threads[threadIndex++], NULL, checkRow, (void *)data);
    }

    // Creating threads for column checks
    for (int i = 0; i < 9; i++)
    {
        parameters *data = (parameters *)malloc(sizeof(parameters));
        data->startCol = i;
        pthread_create(&threads[threadIndex++], NULL, checkColumn, (void *)data);
    }

    // Creating threads for subgrid checks
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            parameters *data = (parameters *)malloc(sizeof(parameters));
            data->startRow = i;
            data->startCol = j;
            pthread_create(&threads[threadIndex++], NULL, checkSubGrid, (void *)data);
        }
    }
	
    // Waiting for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Checking validation results
    int isValid = 1;
    for (int i = 0; i < 9; i++)
    {
        if (rowResult[i] == 0 || colResult[i] == 0 || subgridResult[i] == 0)
        {
            isValid = 0;
            break;
        }
    }

    // Print the result
    if (isValid)
    {
        printf("The Sudoku puzzle is valid.\n");
    }
    else
    {
        printf("The Sudoku puzzle is invalid.\n");
    }

    return 0;
}
