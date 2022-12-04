#include<iostream>
#include "mpi.h"
#include<sstream>
#include<fstream>
#include<stdlib.h>

int **Prepare_Matrix(std::string dimension_of_matrix)
{
       int **M;

       std::stringstream Matrix(dimension_of_matrix);

       std::string temp;

       // Getting rows value before X
       std::getline(Matrix,temp,'X'); 
       //  Matrix rows
       int rows=std::stoi(temp);
       std::cout<<"Rows : "<<rows<<std::endl;
 

       // Getting columns values after X
       std::getline(Matrix,temp,'X');
       //  Matrix columns
       int columns=std::stoi(temp);
       std::cout<<"Columns : "<<columns<<std::endl;

       // Dynamically allocate matrix based on rows and columns
       M=new int *[rows];

       for(int i=0;i<rows;i++)
       {
          M[i]=new int[columns];
       }

       // Initializing with random values
       for(int i=0;i<rows;i++)
       {
         for(int j=0;j<columns;j++)
         {
             M[i][j]=(rand()%5)+1;
         }
       }

       return M;

}

int get_Rows(std::string dimension_of_matrix)
{
       std::stringstream Matrix(dimension_of_matrix);

       std::string temp;

       // Getting rows value before X
       std::getline(Matrix,temp,'X'); 
       //  Matrix rows
       int rows=std::stoi(temp);


       // Getting columns values after X
       std::getline(Matrix,temp,'X');
       //  Matrix columns
       int columns=std::stoi(temp);

       return rows;
}

int get_Columns(std::string dimension_of_matrix)
{
       std::stringstream Matrix(dimension_of_matrix);

       std::string temp;

       // Getting rows value before X
       std::getline(Matrix,temp,'X'); 
       //  Matrix rows
       int rows=std::stoi(temp);

       // Getting columns values after X
       std::getline(Matrix,temp,'X');
       //  Matrix columns
       int columns=std::stoi(temp);

       return columns;
}

int **make_Matrix(int rows,int cols)
{
      int **M;

      M=new int *[rows];

       for(int i=0;i<rows;i++)
       {
          M[i]=new int[cols];
       }
       return M;

}

void display_Matrix(int **M1,int rows_A,int cols_A)
{
      for(int i=0;i<rows_A;i++)
      {
            for(int j=0;j<cols_A;j++)
            {
                  std::cout<<M1[i][j]<<"\t";
            }
            std::cout<<std::endl;
       }


}

void send_Matrix(int **Matrix1,int row_A,int column_A )
{
    // Sending the number of rows of matrix A
      MPI_Send(&row_A,1,MPI_INT,1,10,MPI_COMM_WORLD);

    // Sending the number of columns of matrix A
      MPI_Send(&column_A,1,MPI_INT,1,10,MPI_COMM_WORLD);

      for(int i=0;i<row_A;i++)
      {
          for(int j=0;j<column_A;j++)
          {
              // Sending the matrix
             MPI_Send(&Matrix1[i][j],1,MPI_INT,1,30,MPI_COMM_WORLD);
          }
      }

}
int main(int argc,char *argv[])
{
    MPI_Init(&argc,&argv);

    int rank;
    // Getting the rank of the processes
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int size;
    // Getting the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD,&size);


   if(rank==0)
   {
       int optimal_order[5]={1,2,3,4,5};

       std::cout<<"/////////////////////////////////////////////////////////////"<<std::endl;
       std::cout<<"I am the master process and reading file and distributing data "<<std::endl;
       std::cout<<"/////////////////////////////////////////////////////////////"<<std::endl;

       std:: ifstream obj;
       obj.open("matrix.txt");

       std::string first_matrix_dimension; 
       std::string second_matrix_dimension;

       int count=1;

       int **Matrix1;            // Matrix-1
       int **Matrix2;            // Matrix-2
       int **Result;             // Result

       // Reading the file line by line
       while(std::getline(obj,first_matrix_dimension))
       {

          if(count==1)
          {

             std::cout<<"xxxxxxxxxxxxxxxxx"<<std::endl;
             std::cout<<"Get two matrices "<<std::endl;
             std::cout<<"xxxxxxxxxxxxxxxxxxx"<<std::endl;
            
             // Matrix 1
             Matrix1=Prepare_Matrix(first_matrix_dimension);

             std::getline(obj,second_matrix_dimension);         // Reading 2nd matrix dimension

             std::cout<<"Dimensions are :  "<<first_matrix_dimension<<" :: "<<second_matrix_dimension<<std::endl;

             // Matrix 2
             Matrix2=Prepare_Matrix(second_matrix_dimension);

             // Getting the rows of the first matrix
             int row_A=get_Rows(first_matrix_dimension);

             int column_A=get_Columns(first_matrix_dimension);

             // Getting the rows of the second matrix
             int row_B=get_Rows(second_matrix_dimension);

             // Getting the columns of the second matrix
             int column_B=get_Columns(second_matrix_dimension);

             ///////////////////////// Now sending Matrix-1 to the slave ////////////////////

             send_Matrix(Matrix1,row_A,column_A);

             send_Matrix(Matrix2,row_B,column_B);

             count=count+1;

          }
          else
          {
             std::cout<<"Dimension are :  "<<first_matrix_dimension<<std::endl;

             std::cout<<"Get one matrix "<<std::endl;
          }

       }
   }
   else if(rank==1)
   {
           int rows_A;
           int cols_A;
           
           // Recieving rows of the first matrix
           MPI_Recv(&rows_A,1,MPI_INT,0,10,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
           std::cout<<"Rows are : "<<rows_A<<std::endl;
           // Recieving columns of the first matrix
           MPI_Recv(&cols_A,1,MPI_INT,0,10,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
           std::cout<<"Cols are : "<<cols_A<<std::endl;
           int **M1=make_Matrix(rows_A,cols_A);

          // Now recieving the original first matrix
          for(int i=0;i<rows_A;i++)
          {
               for(int j=0;j<cols_A;j++)
               {
                     // Sending the matrix
                      MPI_Recv(&M1[i][j],1,MPI_INT,0,30,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                }
           }

          std::cout<<"The matrix 1 is "<<std::endl;
          display_Matrix(M1,rows_A,cols_A);

          int rows_B;
          int cols_B;

          // Recieving rows of the second matrix
          MPI_Recv(&rows_B,1,MPI_INT,0,10,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          std::cout<<"Rows are : "<<rows_B<<std::endl;
          // Recieving columns of the second matrix
          MPI_Recv(&cols_B,1,MPI_INT,0,10,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          std::cout<<"Cols are : "<<cols_B<<std::endl;
          // Creating the matrix
          int **M2=make_Matrix(rows_B,cols_B);

          for(int i=0;i<rows_B;i++)
          {
               for(int j=0;j<cols_B;j++)
               {
                     // Sending the matrix
                      MPI_Recv(&M2[i][j],1,MPI_INT,0,30,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                }
           }

          std::cout<<"The matrix 2 is "<<std::endl;
          display_Matrix(M2,rows_B,cols_B);

   }


    // MPI Clean Ups
    MPI_Finalize();
}
