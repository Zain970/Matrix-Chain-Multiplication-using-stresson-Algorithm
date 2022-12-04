#include <algorithm>
#include <string.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

const int UPPER_BOUND = 50; // Range
const int LOWER_BOUND = 20;

using namespace std;
string res1="";
string result;
int find_duplicate(int number, int* matrix_rows, int size_array)
{
	int counter = 0;
	for (int i = 0; i < size_array; i++)
	{
		if (matrix_rows[i] == number)
		{
			counter++;
		}
	}

	/*
	-1 means that no duplicate index
	other than yes there is duplicate and i the index of the duplicate
	*/
	if (counter > 1)
		return counter;
	else 
		return -1;
}


int * generate_matrix_order(int no_matrixs)
{
	srand(time(nullptr));

	int total_random_number_to_generate = no_matrixs + 1;

	// generating possible rows
	int *matrix_points_M = new int[total_random_number_to_generate];

	for (int i = 0; i < total_random_number_to_generate; i++)
	{
		int random_number = rand() % (UPPER_BOUND - LOWER_BOUND + 1) + LOWER_BOUND;
		matrix_points_M[i] = random_number;

		if (i != 0)
		{
			//CHECK DUPLICATE & RESOLVE UNTIL DUPLICATE IS NOT REMOVED
			while (find_duplicate(random_number, matrix_points_M, total_random_number_to_generate) != -1)
			{
				std::cout << "DUPLICATE NUMBER IS FOUND "<<random_number << std::endl;
				random_number = rand() % (UPPER_BOUND - LOWER_BOUND + 1) + LOWER_BOUND;
				matrix_points_M[i] = random_number;
			}
		}
	}

	// printing the matrix orders generated
	for (int i = 0; i < total_random_number_to_generate; i++)
	{
		std::cout << matrix_points_M[i] << " \t ";
	}
	std::cout << std::endl;
	return matrix_points_M;
}

void generate_order_of_matrices(int size_matrix_points_M, int* matrix_points_M)
{
	std::string *matrix_order=new std::string[size_matrix_points_M-1];
	for (int i = 0; i < size_matrix_points_M-1; i++)
	{
		matrix_order[i] = std::to_string(matrix_points_M[i]) + "X" + std::to_string(matrix_points_M[i + 1]);
		std::cout << matrix_order[i] << std::endl;

	}

	// paste the order in the file matrix_orders.txt
	std::ofstream matrix_orders_file;
	matrix_orders_file.open("matrix_orders.txt");

	if (matrix_orders_file.is_open())
	{
		for (int i = 0; i < size_matrix_points_M - 1; i++)
		{
			if (i == size_matrix_points_M - 2)
			{
				// if this is the last row then donot perform endl
				matrix_orders_file << matrix_order[i];
			}
			else
			{
				matrix_orders_file << matrix_order[i] << std::endl;
			}
			
		}
	}
	
	matrix_orders_file.close();
	delete []matrix_order;
	matrix_order = nullptr;
}


void recurfunc(int i,int v1, int end_index, int v2, int no, int v3, int* bracket,int v4,char& matrixNO,int v5)
{

    int incre_var=10;
    int arrarmin[101];
	if (i == end_index) {
        char val=matrixNO++;
        result.push_back(val);
        int min=arrarmin[0];
			for(int i=0; i<incre_var; i++){
				if(min<arrarmin[i]){
					min=arrarmin[i];
				}
			}
	    return;
	}
	result.append("(");
     int min=arrarmin[0];
			for(int i=0; i<incre_var; i++){
				if(min<arrarmin[i]){
					min=arrarmin[i];
				}
			}
	recurfunc(i,incre_var, *((bracket + i * no) + end_index),incre_var+1, no, incre_var+2, bracket, incre_var+3, matrixNO, incre_var+4);
			for(int i=0; i<incre_var; i++){
				if(min<arrarmin[i]){
					min=arrarmin[i];
				}
			}
	recurfunc(*((bracket + i * no) + end_index) + 1,incre_var, end_index,incre_var+1, no,incre_var+2,bracket,incre_var+3,matrixNO,incre_var+4);
			for(int i=0; i<incre_var; i++){
				if(min<arrarmin[i]){
					min=arrarmin[i];
				}
			}
    result.append(")");
}

/*

	updated version with last value as m in the last row order mxn 

	n will be taken as m


*/





int* read_matrix_order_file(std::string file_name, int &size_matrix_order_interms_of_M)
{

	int* matrix_order_interms_of_M = nullptr;

	std::ifstream matrix_order_file;
	matrix_order_file.open(file_name);

	if (!matrix_order_file.is_open())
	{
		return nullptr;
	}


	//-------- get total lines of file -------------------------------------
	int total_lines_in_file = 0;
	while (!matrix_order_file.eof())
	{
		std::string line;
		std::getline(matrix_order_file, line);
		total_lines_in_file++;
	}

	matrix_order_file.clear();
	matrix_order_file.seekg(0);

	if (total_lines_in_file >= 5 && total_lines_in_file  <= 7)
	{
		matrix_order_interms_of_M = new int[total_lines_in_file+1];

		// updating size variable with total number of line in the file
		size_matrix_order_interms_of_M = total_lines_in_file+1;


		int iterator = 0;
		while (!matrix_order_file.eof())
		{
			std::string line;
			std::getline(matrix_order_file, line);

			std::stringstream iss(line);


			// m and n represnt mxn matrix order
			int m = 0;

			std::string temp = "";

			std::getline(iss, temp, 'X');

			matrix_order_interms_of_M[iterator] = std::stoi(temp);
			iterator++;

			if (iterator == total_lines_in_file)
			{
				std::getline(iss, temp, '\0');
				matrix_order_interms_of_M[iterator] = std::stoi(temp);
			}
			
		}
	}
	else
	{
		std::cout << "!!!!!!!!!!!!!!!!! LINES IN THE FILES ARE NOT CORRECT MUST BE 5,6,7 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
		return nullptr;
	}

	return matrix_order_interms_of_M;
}

void chaining(int Orders[], int no)
{
	int dynamicMatrix[no][no];
	int bracket[no][no];
    int value=2;
    int value1=1;
    int totalnumber=5;
    for (int i = 1; i < no; i++){
        for (int i = 1; i < no; i++){
		dynamicMatrix[i][i] = 0;
        }
    }
    
    int Diagonal = 2;
	while(Diagonal < no)
    {
        int start_index = 1;
		while(start_index < ((no - Diagonal) + value1) )
        { 
            int vari= Diagonal- (value-value1);
            int end_index = start_index + vari;
            dynamicMatrix[start_index][end_index] = 99999999;
			int arrarmin[10]; // It stores The All The Values of the splits possible and then Get minimum from list to assign
			int incre_var=0;
			int split_value = start_index;
            while(split_value <= (end_index - value1) )
            {
                //price From start_index to Split
                int price_initial=dynamicMatrix[start_index][split_value];
                //price From split to end_index
                int price_end=dynamicMatrix[split_value + value1][end_index];
                
                int price =Orders[start_index - value1] * Orders[split_value] * Orders[end_index];
                int total_price =price+price_initial+price_end;
				arrarmin[incre_var]=total_price;
				incre_var=incre_var+value1;
				if (total_price < dynamicMatrix[start_index][end_index])
				{
					dynamicMatrix[start_index][end_index] = total_price;
                     int min=arrarmin[0];
                    for(int i=0; i<incre_var; i++){
                        if(min<arrarmin[i]){
                            min=arrarmin[i];
                        }
                    }
					bracket[start_index][end_index] = split_value;
				}
			split_value=split_value+value1;
            }
            int min=arrarmin[0];
			for(int i=0; i<incre_var; i++){
				if(min<arrarmin[i]){
					min=arrarmin[i];
				}
			}
             start_index=start_index+value1;
		}
        Diagonal=Diagonal+value1;
    }
	char matrixNO = '1';
	recurfunc(value1,value1+1, no - value1,value1+2, no,value1+3, (int*)bracket,value1+4, matrixNO,value1+5);
			for(int i=0; i<value1; i++){
				if(value<value1){
					value=value1;
				}
			}
    cout << "\nAnswer Is:: " << result<<endl;
	cout << "\nNo Of Scalar Multiplications Are:  " ;
    cout<< dynamicMatrix[value1][no - value1]<<endl;
}

int main()
{
    	std::cout << "\nEnter the number matrix (Range must be b/w 5 to 7) :\t";
	int no_matrix = 0;
	std::cin >> no_matrix;
	// no_matrix = 5;


	int* matrix_points_M = nullptr;


	////////////////////////////////////////////// GENERATING AND WRITING IN THE FILE ////////////////////////////////////////////////////////////////////
	if (no_matrix >= 5 && no_matrix <= 7)
	{
		matrix_points_M =generate_matrix_order(no_matrix);
	}
	else
	{
		std::cout << "Invalid Range !!!!" << std::endl;
	}


	// matrix point must not be nullptr
	if (matrix_points_M != nullptr)
	{
		generate_order_of_matrices(no_matrix + 1, matrix_points_M);
	}


	/////////////////////////////////////////// READING FROM THE FILE ////////////////////////////////////////////////////////////////////////////////////
	int size_matrix_order_interms_of_M = 0;
	int* matrix_order_interms_of_M = read_matrix_order_file("matrix_orders.txt",size_matrix_order_interms_of_M);

	if (size_matrix_order_interms_of_M != 0 && matrix_order_interms_of_M != nullptr)
	{
		std::cout << "\n\nMatrix Order Points M are as follows : \n";
		for (int i = 0; i < size_matrix_order_interms_of_M; i++)
		{
			std::cout << matrix_order_interms_of_M[i] << std::endl;
		}
	}
	else
	{
		std::cout << "file format is damange !!!!!" << std::endl;
	}

	// int array[] = { 36,31,27,20,32,49 };
	chaining(matrix_order_interms_of_M, no_matrix);
	return 0;
}
