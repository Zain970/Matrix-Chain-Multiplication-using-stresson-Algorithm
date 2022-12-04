#include <cmath>
#include <vector>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <mpi.h>
using namespace std;
void plusMatrices(int** First, int** Second, int** Third,  int dimension) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            Third[i][j] = First[i][j] + Second[i][j];
        }
    }
}
void minusMatrices(int** First, int** Second, int** Third,  int dimension) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            Third[i][j] = First[i][j] - Second[i][j];
        }
    }   
}
int** makeMatrix(int n)
{
    int* data = new int[n * n];
    int** array = new int*[n];
    for (int i = 0; i < n; i++)
    {
        array[i] = &(data[n * i]);
    }
    return array;
}

void stressMulti(int** First,int** Second,int** Third,int dimension, int rank) {

    if (dimension == 1) {
        Third[0][0] = First[0][0] * Second[0][0];
        return;
    }
    else if (dimension == 2) {

        //Continlouslly Dividing The Matrix
        int noRows = dimension / 2;
        int** tableA_a=makeMatrix(noRows);
        int** tableA_b=makeMatrix(noRows);
        int** tableA_c=makeMatrix(noRows);
        int** tableA_d=makeMatrix(noRows);
        int** tableB_a=makeMatrix(noRows);
        int** tableB_b=makeMatrix(noRows);
        int** tableB_c=makeMatrix(noRows);
        int** tableB_d=makeMatrix(noRows);
        int** tableC_a=makeMatrix(noRows);
        int** tableC_b=makeMatrix(noRows);
        int** tableC_c=makeMatrix(noRows);
        int** tableC_d=makeMatrix(noRows);
        int** temp_a=makeMatrix(noRows);
        int** temp_b=makeMatrix(noRows);
        int** temp_c=makeMatrix(noRows);
        int** temp_d=makeMatrix(noRows);
        int** temp_e=makeMatrix(noRows);
        int** temp_f=makeMatrix(noRows);
        int** temp_g=makeMatrix(noRows);
        int** temp_h=makeMatrix(noRows);
        int** temp_i=makeMatrix(noRows);
        int** temp_j=makeMatrix(noRows);
        int** step_a=makeMatrix(noRows);
        int** step_b=makeMatrix(noRows);
        int** step_c=makeMatrix(noRows);
        int** step_d=makeMatrix(noRows);
        int** step_e=makeMatrix(noRows);
        int** step_f=makeMatrix(noRows);
        int** step_g=makeMatrix(noRows);
        int** store_A=makeMatrix(noRows);
        int** store_B=makeMatrix(noRows);

        for (int i = 0; i < noRows; i++) {
            for (int j = 0; j < noRows; j++) {
                 vector <int **> v1;
                v1.push_back(tableB_d);
                tableA_a[i][j] = First[i][j];
                v1.push_back(tableB_d);
                tableA_b[i][j] = First[i][j + noRows];
                v1.push_back(tableB_d);
                tableA_c[i][j] = First[i + noRows][j];
                v1.push_back(tableB_d);
                tableA_d[i][j] = First[i + noRows][j + noRows];
                v1.push_back(tableB_d);
                v1.push_back(tableB_c);
                tableB_a[i][j] = Second[i][j];
                v1.push_back(tableB_a);
                tableB_b[i][j] = Second[i][j + noRows];
                v1.push_back(tableA_b);
                tableB_c[i][j] = Second[i + noRows][j];
                v1.push_back(tableA_d);
                tableB_d[i][j] = Second[i + noRows][j + noRows];
                v1.push_back(tableA_a);
            }
        }

        //It Stores The All Subtracting One Matrices Needed For Calculation
        vector <int **> minus_storing;
        minus_storing.push_back(tableB_b);
        minus_storing.push_back(tableB_d);
        minus_storing.push_back(tableB_c);
        minus_storing.push_back(tableB_a);
        minus_storing.push_back(tableA_b);
        minus_storing.push_back(tableA_d);
        minus_storing.push_back(tableA_a);
        minus_storing.push_back(tableA_c);

        //It Stores The All Adding One Matrices Needed For Calculation
        vector <int **>plus_storing;
        plus_storing.push_back(tableA_a);
        plus_storing.push_back(tableA_b);
        plus_storing.push_back(tableA_c);
        plus_storing.push_back(tableA_d);
        
        plus_storing.push_back(tableA_a);
        plus_storing.push_back(tableA_d);
        plus_storing.push_back(tableB_a);
        plus_storing.push_back(tableB_d);
        

        plus_storing.push_back(tableB_c);
        plus_storing.push_back(tableB_d);

        plus_storing.push_back(tableB_a);
        plus_storing.push_back(tableB_b);


        for(int i=0; i<2; i++){
            int **temp1=makeMatrix(5);
            int **temp2=makeMatrix(5);        
        }

        if(rank==0){
            MPI_Status status;
            int variable=noRows * noRows;
            MPI_Recv(&(temp_a[0][0]), variable, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&(temp_b[0][0]), variable, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&(temp_c[0][0]), variable, MPI_INT, 3, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&(temp_d[0][0]), variable, MPI_INT, 4, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&(temp_e[0][0]), variable, MPI_INT, 5, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&(temp_f[0][0]), variable, MPI_INT, 6, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&(temp_g[0][0]), variable, MPI_INT, 7, 0, MPI_COMM_WORLD, &status);
        }
        
        // If rank=1;
        if(rank==1){
            int variable=noRows * noRows;
            stressMulti(tableA_a, temp_a, step_a, noRows,rank);
            MPI_Send(&(temp_a[0][0]), variable, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        // If rank=2;
        if(rank==2){
            int variable=noRows * noRows;
            stressMulti(temp_b, tableB_d, step_b, noRows,rank);
            MPI_Send(&(temp_b[0][0]), variable, MPI_INT, 0, 0, MPI_COMM_WORLD);

        }
        // If rank=3;
        if(rank==3){
            int variable=noRows * noRows;
            stressMulti(temp_c, tableB_a, step_c, noRows,rank);
            MPI_Send(&(temp_c[0][0]), variable, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        // If rank=4;
        if(rank==4){
            int variable=noRows * noRows;
            stressMulti(tableA_d, temp_d, step_d, noRows,rank);
            MPI_Send(&(temp_d[0][0]), variable, MPI_INT, 0, 0, MPI_COMM_WORLD);

        }
        // If rank=5;
        if(rank==5){
            int variable=noRows * noRows;
            stressMulti(temp_e, temp_f, step_e, noRows,rank); 
            MPI_Send(&(temp_e[0][0]), variable, MPI_INT, 0, 0, MPI_COMM_WORLD);
        
        }
        // If rank=6;
        if(rank==6){
            int variable=noRows * noRows;
            stressMulti(temp_g, temp_h, step_f, noRows,rank);
            MPI_Send(&(temp_f[0][0]), variable, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        // If rank=7;
        if(rank==7){
            int variable=noRows * noRows;
            stressMulti(temp_i, temp_j, step_g, noRows,rank);
            MPI_Send(&(temp_g[0][0]), variable, MPI_INT, 0, 0, MPI_COMM_WORLD);
        
        
        vector <int **> v5;
        //Combing The Matrices Now
        for (int i = 0; i < noRows ; i++) {
            for (int j = 0 ; j < noRows ; j++) {
                // v5.push_back(Third[i][j]);
                Third[i][j] = tableC_a[i][j];
                // v5.push_back(Third[i][j]);
                Third[i][j + noRows] = tableC_b[i][j];
                // v5.push_back(Third[i][j]);
                Third[i + noRows][j] = tableC_c[i][j];
                // v5.push_back(Third[i][j]);
                Third[i + noRows][j + noRows] = tableC_d[i][j];
                // v5.push_back(Third[i][j]);
            }
        }


        }   
     }
}

int main(int argc, char* argv[])
{
    int rank;
    int num_process;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);

    
    int matrixSize=5;
    int n=matrixSize;
    int** First;
    int** Second;
    int** Third = makeMatrix(matrixSize);
    for(int i=0; i<matrixSize; i++){
        for(int j=0; j<matrixSize; j++){
            Third[i]=0;
        }
    }
    vector<int> rowVector(matrixSize);
    // MPI_Bcast(&(First[0][0]), n * n, MPI_INT, 0, MPI_COMM_WORLD);
    
    // MPI_Bcast(&(Second[0][0]), n * n, MPI_INT, 0, MPI_COMM_WORLD);

    stressMulti(First, Second, Third, matrixSize,rank);
    MPI_Finalize();

    return 0;
}
