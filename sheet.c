/*******************************************************************/
/***			IZP PROJEKT 1 - Prace s textem					 ***/
/***															 ***/
/***	projekt plni funkce jednoducheho tabulkoveho procesoru	 ***/
/*** 															 ***/
/***						Jakub Kuznik						 ***/
/*** 						rijen 2020						  	 ***/
/*******************************************************************/

/*
 * In my program there are 3 basic operations(commands) that u can choose as argument.
 *
 * 1.editing_table = irow R, arow, drow R, drows N M, icol C, acol, dcol c, dcols N M
 * 2.editing_data = cset C STR, tolower C, toupper C, round C, int C, copy N M, swap N M, move N M,
 * 3.rows_selecting = rows N M, beginswith C STR, contains C STR.
 *
 * U can use rows_selection with edit_data, but u cannot use it with edit_table. 
 * u can use as many edit_table operation as u want in one linux command.
 * if u choose one of rows_selection operation the parameters of edit_data command may be ignored. 
 *
 * R = row
 * C = column
 * N = begin row 
 * M = end row  
 * STR = 3characters 
 *
 * U can always copy,move,swap N to M and must be N<M. 
 * So if u want to copy M to N first swap it.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAXFILESIZE 10000		//Here is max input file size it can be changed 	
#define MAX_CELL_SIZE 50		//Max cell size in function for copy_swap_move	
#define MAX_ARG_UT 18			//Size of array that i store edit table operation(commands) to.  
#define ROW_OPERATION 6			//Size of array for row_operation. 
#define EDIT_OPERATION 26		//Siye of array for Edit_operation 
#define SPACE_FOR_SRING 3
#define MAX_STRING_SIZE 20

enum erorrs{wrong_range = -1, bad_argument_input = -2};
#define WRONG_RANGE -19 //if i have wrong range rows 

/*This 3 functions store input arguments and save it to array int.
 * Every function have common values writen up.*/
//irow 1 2 //arow 3 //drow 4 5 //drows 6 7 8 ///icol 9 10 //acol 11 //dcol 12 13 //dcols 14 15 16
int edit_table(int arguments[], int argc, char *argv[], int row_max);
// 1. cset C STR 2. tolower C, 3. toupper C, 4. round C, 5. int C, 6. copy N M, 7.swap N M, 8.move N M;
int data_manage(int zpracovani_funkce[], int argc, char *argv[], int pocet_radku);
//1. rows N M, 2. beginwith C STR, 3. contains C STR 
int row_selection(int row_operation[],int argc, char *argv[], int row_max);

//We'll send all info to this functions and it'll decide what operation(command) should program do
int edit_decider(int edit_data[],int *max_row, int true_size, char file[],int column_max, char separator);
//there can be only one operation and one row_selection. so the first arg ll be choosen others ll be ignored 
int data_decider(int edit_data[], int row_selection[], int max_row, int true_size, char file[], int column_max, char separator);

/*This function know types of command that can be done.
 * So it decide what type of data should be stored to next byte.
 * It could be STR - string C - start column  NM - column start column end  
 * */
int argument_universal_simplify(int function[], int command, int command_number, int number, int rows_max, char *argv[], int i, int argc);
/*
 *This function solve the problem with row operations. So i can add row operation parameter before edit_data command 
 * */
int rows(char file[], int row_operation[], int file_size, int rows[],int row_max, char separator);


//FUNCTIONS FOR EDIT_TABLE COMMANDS 
//it will add a new row
int irow( int vybrany_radek, int true_size, char file[], int columns, char separator, int *max_radek);
//This function delete rows
void drows(char file[], int true_size, int start, int end); 
/*This functiont delete columns 
first column is 0 and range can be from 0 to infiinity. 
*/
int dcol(char file[], int column, int column_end, int rows_max, int file_size, char separator, int column_max);


//FUNCTIONS for DATA_EDITING	
int cset(char file[], int rows[], int data_edit[], int column, char separator, int rows_max, int file_size);
int tolower_toupper_round_int(char file[], int rows[], int column, char separator, int file_size, int operation);
int copy_swap_move(char file[], int rows[], int column_N, int column_M, char separator, int file_size, int operationi,int column_max);
int copy(char file[], int i, int *file_size, int column_counter, int column_M, char separator);
int swap(char file[], int i, int *file_size, int column_counter, int column_M, char separator);
int move(char file[], int i, int *file_size, int column_counter, int column_M, char separator);

//OTHER FUNCTIONS 
//char d is cels separator  
int round_floatingP_delete(char file[], char* pfile, int start, int operation, char separator);

int cels_rows_counter(int true_size, char file[], char symbol);
int columns_counter(int true_size, char d, char file[]);		

int separe(int argc, char *argv[]); 				//Function gets cels separator. 
int save_file(int true_size, char file[]);			//Function store input file to variable file[].
int file_print(int true_size, char file[]);

int array_inicialization(int array[], int array_size, int num);		//I am storying 0 values to array variables.
int create_space(char file[],int size, int start); //It create space in file so i can insert some more characters. 
int save_cel(char file[], int start, char separator, char cell[]);
int swap_value(int *one, int *two);

int error(int type);

int main(int argc, char *argv[])
{
	int d_separator = (' ');	 	
	int columns = 0;			
	int rows = 0; 				 
	int real_size = 0; 		  
	char file_stored[MAXFILESIZE]; 
	
	int edit_o[MAX_ARG_UT];			//Here i stored edit table operations that ll be executed 	
	int rows_o[ROW_OPERATION]; 		
	int data_o[EDIT_OPERATION];		
				
	int errors = 0;	
 
	array_inicialization(edit_o, MAX_ARG_UT, 0);
 	array_inicialization(rows_o, ROW_OPERATION, 0);
	array_inicialization(data_o, EDIT_OPERATION, 0);

	d_separator = separe(argc, argv);
	real_size = save_file(real_size, file_stored);
	if(real_size == 0)
	{
		error(3);
		return -1;
	}
	if(real_size == -1)
	{
		error(4);
		return -1;
	}
	rows = cels_rows_counter(real_size,file_stored,'\n');
	columns = columns_counter(real_size, d_separator, file_stored);
	
	errors = edit_table(edit_o,argc,argv,rows);
	if(errors < 0)
	{
		error(2);
		return -1;
	}
	if(edit_o[0] == 0)
	{
		errors = row_selection(rows_o, argc, argv, rows);
		if(errors < 0)
		{
			error(1);
			return -1;
		}
		errors = data_manage(data_o, argc, argv, rows);
		if(errors < 0)
		{
			error(1);
			return -1;
		}
		if(data_o[0] == 1)
		{
			real_size = data_decider(data_o, rows_o, rows, real_size, file_stored, columns, d_separator);
			if(real_size == -1)
			{
				error(1);
				return -1;
			}	
		}
	}
	else
	{
		real_size = edit_decider(edit_o, &rows, real_size, file_stored, columns, d_separator);
	}
	file_print(real_size, file_stored);

	return 0;
}
//This function store input file to char file[]
int error(int type)
{
	switch(type)
	{
		case 1:	
			fprintf(stderr, "Wrong range\n");
			break;
		case 2:
			fprintf(stderr, "Bad argument input\n");
			break;
		case 3:
			fprintf(stderr, "empty imput file \n");
			break;
		case 4:
			fprintf(stderr, "BIG FILE \n");
			break;
	}
	return -1;	
}
int save_file(int true_size, char file[])
{
	int z;
	while((z = getchar()) != EOF)
	{
		file[true_size] = z;
		true_size++;
		if(true_size == MAXFILESIZE)
		{
			return -1;
		}
	}
	return true_size;
}
//At the end of program this function will write an output
int file_print(int true_size, char file[])
{
	int i = 0;
	while((i < true_size)&&(file[i] != EOF))
	{
		putchar(file[i]);
		i++;
	}	
	return true_size;
}
//it will add a new row
int irow( int vybrany_radek, int true_size, char file[], int columns, char separator, int *max_radek)
{
	*max_radek = *max_radek + 1 ;
	int pocitadlo_radku = 0;
	if(vybrany_radek < 0)
		vybrany_radek = 0;
	for(int i=0;i < true_size; i++)
	{
		if(file[i] == '\n')
			pocitadlo_radku++;		
		if(pocitadlo_radku == (vybrany_radek))
		{
			if(vybrany_radek != 0)
			{
				create_space(file, true_size, i);
				true_size++;
				file[i] = '\n';
				i++;
			}
			for (int j = 0; j < columns ; j++)
			{					
				create_space(file, true_size, i);
				true_size++;
				file[i] = separator;
				i++;
			}
			if(vybrany_radek==0)
			{
				create_space(file, true_size, i);
				true_size++;
				file[i] = '\n';
			}
			break;	
		}
	}
	return true_size;
}
/*drows and drows 
 * This function can delete rows 
 * */
void drows(char file[], int true_size, int start, int end)
{
	int rows_sum = end - start + 1; 
	int help = 0;
	int row_counter = 0;
	for(int i = 0; i < true_size; i++)
	{
		if(file[i] == '\n')
			row_counter++;
		if(row_counter == start)
		{
			for(int j = i; j < 2000; j++)
			{
				if(file[j] == '\n')
					help++;
				if(help == rows_sum)
					break;
				file[j] = '\0';
			}	
			break;	
		}
	}

}
//Function'll add a column in front of chosen_collumn.
// If u try add column befof collumn that doesnt exist it ll add column to end 
int icol(char file[], int true_size, int chosen_column, int column_max, char separator, int rows)
{
	int separator_counter = 0;
	chosen_column = chosen_column +1;
	if(chosen_column > column_max)
	{
		for(int k = 0; k < true_size+rows;k++)
		{	
			if(file[k] == '\n')
			{
				create_space(file, (true_size+rows),k);
				true_size++;
				file[k] = separator;
				k = k+2;
			}
		}
	}
	else
	{
		for(int j = 0; j < true_size+rows;j++)
		{
			if(file[j] == '\n')
			{
				separator_counter = 0;
				j++;
			}	
			if(file[j] == separator)
				separator_counter++;
			if(separator_counter == chosen_column)
			{
				create_space(file, (true_size+rows), j);
				true_size++;
				file[j] = separator;
				separator_counter = 0;
				while(file[j] != '\n')
					j++;
				continue;
			}
		}
	}
	return true_size; 
}
//It delete columns from column to column_end 
int dcol(char file[], int column, int column_end, int rows_max, int file_size, char separator, int column_max)
{
	if(column == column_end)
		return 0;
	if(column_max == 0)	
		return 0;
	int column_counter = 0;
	int i = 0;
	for(int rows_counter = 0; rows_counter <= rows_max;)
	{
		if(column != 0)
		{
			while(column_counter != column)
			{
				if(file[i] == separator)
					column_counter++;
				i++;
			}
			column_counter--;
		}
		while(i < file_size)
		{
			if(file[i] == separator)
				column_counter++;
			if(column_counter == (column_end - column))
				while(file[i] != '\n')
					i++;
			if(file[i] == '\n')
				break;
			file[i] = '\0'; 
			i++;
		}
		i++;
		column_counter = 0;
		rows_counter++;
	}
	return 0;
}
/*Function is making more space in file[]. So i can insert some char like \n or new cell.
 */
int create_space(char file[], int size, int start)
{
	int space = 1;
	char help[size + space];
	for(int i = 0; i < start ; i++)
		help[i] = file[i];	
	for(int j = start + space -1; j < (size + space); j++)	
		help[j+space] = file[j];
	for(int k = 0; k <= (size + space);k++)
		file[k] = help[k];
	return space;
}
//We ll get number of columns from first row 
int columns_counter(int true_size, char d, char file[])
{
	int columns = 0;
	int i = 0;
	while((i <= true_size)&&(file[i] != '\n')) 
	{
		if(file[i] == d)
			columns++;
		i++;
	}
	return columns;
}
int cels_rows_counter(int true_size, char file[], char symbol)
{
	int i = 0;
	int Counter = 0;
	while(i <= true_size)
	{
		if(file[i] == symbol)
			Counter++;
		i++;
	}
	return Counter;
}
//In table we alwazs have some separator character this function ll get it.
//-d 
//separator is always next argument after -d 
//it there is not any default is ' ' space 
int separe(int argc, char *argv[])
{	
	int input_argument = ' ';
	for(int i = 0; i < argc; i++)  
	{
		if(strcmp(argv[i], "-d")==0)
			if(i+1 < argc)
			{
				input_argument = argv[i+1][0];
				break;
			}
	}
	return input_argument; 
}
/*[0] = 1 
 *on the first positon is always 1 if there are any operation else there is just 0 
 * next bytes of array reserved for difrent operations 
 *irow 1 2 //arow 3 //drow 4 5 //drows 6 7 8 ///icol 9 10 //acol 11 //dcol 12 13 //dcols 14 15 16
 *irow R, arow, Drow R, Drows, N M, icol C, acol, dcol C, dcols N M,
*/
int edit_table(int arguments[], int argc, char *argv[], int row_max)
{
	int k = 0;
	for(int i = 0; i < argc; i++)  
	{
		if(strcmp(argv[i], "irow")==0) //R
			k = k + argument_universal_simplify(arguments, 1, 1, 1, row_max, argv, i,  argc);
		else if(strcmp(argv[i], "arow")==0) 
			k = k + argument_universal_simplify(arguments, 4, 3, 1, row_max, argv, i,  argc);
		else if(strcmp(argv[i], "drow")==0) //R
			k = k + argument_universal_simplify(arguments, 1, 4, 1,  row_max, argv, i,  argc);
		else if(strcmp(argv[i], "drows")==0) //N M 
			k = k + argument_universal_simplify(arguments, 3, 6, 1,  row_max, argv, i,  argc);
		else if(strcmp(argv[i], "icol")==0) //C
			k = k + argument_universal_simplify(arguments, 1, 9, 1, row_max, argv, i,  argc);
		else if(strcmp(argv[i], "acol")==0) 
			k = k + argument_universal_simplify(arguments, 4, 11, 1, row_max, argv, i,  argc);
		else if(strcmp(argv[i], "dcol")==0) //C
			k = k + argument_universal_simplify(arguments, 1, 12, 1, row_max, argv, i,  argc);
		else if(strcmp(argv[i], "dcols")==0) //NM 
			k = k + argument_universal_simplify(arguments, 3, 14, 1,  row_max, argv, i,  argc);
	}
	return k;
}
/*This function is here for simplifying the data_manage function. 
R = row
C + STR = columm  number plus string
N M start row and end row 
We ll save the arguments from argv to manage_function array
*/
int argument_universal_simplify(int function[], int command, int command_number, int number, int rows_max, char *argv[], int i, int argc)
{
	switch(command)
	{
		case 1:
			if(i+1 < argc)
			{
				function[0] = 1;
				function[command_number] = number;			
				function[command_number+1] = atoi(argv[i+1]) -1;
				if(atoi(argv[i+1]) < 1)
					return -2; 
			}
			else
				return -1; //there is no comand parametrs
			break;
		case 2:
			if(i+2 < argc)
			{
				function[0] = 1;
				function[command_number] = number;			
				function[command_number+1] = atoi(argv[i+1])-1;
				if(atoi(argv[i+1]) < 1)
					return -2; 
				function[command_number+2] = argv[i+2][0];
				function[command_number+3] = argv[i+2][1];
				function[command_number+4] = argv[i+2][2];
			}
			else
				return -1;
			break;
		case 3:
			if(i+2 < argc)
			{	
				function[0] = 1;	
				function[command_number] = number;
				function[command_number+1] = 1;		
				if(atoi(argv[i+2]) < 1)
					return -2; 
				if(atoi(argv[i+1]) < 1)
					return -2; 
				if(((atoi(argv[i+1])) >= 0) && (atoi(argv[i+2]) <= rows_max))
				{
					if((argv[i+1][0] == 'N') && (argv[i+1][0] == 'n'))
						function[command_number+1] = 1;	
					if(argv[i+2][0] == '-')
						function[command_number+2] = rows_max;
					else
					{
						function[command_number+1] = atoi(argv[i+1])-1;
						function[command_number+2] = atoi(argv[i+2])-1;
					}
				}
			}
			else
				return -20; //bad row range
			break;
		case 4:
			function[command_number] = 1;
			function[0] = 1;
			break;
		case 5:
			if(i+2 >= argc)
				return -1;
			function[0] = 1;	
			function[command_number] = number;			
			function[command_number+1] = atoi(argv[i+1])-1;
			if(atoi(argv[i+1]) < 1)
				return -2; 
			int t = 2;
			int k = 0;
			for(k = k; k<MAX_STRING_SIZE;k++)	
			{
				if(argv[i+2][k] == '\0')
				{
					break;
				}
				function[command_number+t] = argv[i+2][k];
				t++;
			}
			break;
		default:
			break;
	}
	return 0;
}
/*Function ll check out input arguments and if there is any for edit_data it ll store to function_manage[]
 * 1. cset C STR 2. tolower C, 3. toupper C, 4. round C, 5. int C, 6. copy N M, 7.swap N M, 8.move N M;
*/
int data_manage(int function_manage[], int argc, char *argv[], int rows)
{
	int k = 0;
	for(int i = 0; i < argc; i++) 
	{
		if(strcmp(argv[i], "cset")==0)
			k = argument_universal_simplify(function_manage, 5, 1, 1, rows, argv, i, argc);
		else if(strcmp(argv[i], "tolower")==0)
			k = argument_universal_simplify(function_manage, 1, 1, 2, rows, argv, i, argc);
		else if(strcmp(argv[i], "toupper")==0)
			k =  argument_universal_simplify(function_manage, 1, 1, 3, rows, argv, i, argc);
		else if(strcmp(argv[i], "round")==0)
			k = argument_universal_simplify(function_manage, 1, 1, 4, rows, argv, i, argc);
		else if(strcmp(argv[i], "int")==0)
			k = argument_universal_simplify(function_manage, 1, 1, 5, rows, argv, i, argc);
		else if(strcmp(argv[i], "copy")==0)
			k = argument_universal_simplify(function_manage, 3, 1, 6, rows, argv, i, argc);
		else if(strcmp(argv[i], "swap")==0)
			k = argument_universal_simplify(function_manage, 3, 1, 7, rows, argv, i, argc);
		else if(strcmp(argv[i], "move")==0)
			k = argument_universal_simplify(function_manage, 3, 1, 8, rows, argv, i, argc);
	}		
	if(k < 0)
	{
		return k;
		printf("eeee");
	}
	return function_manage[0];
}

/*Basicaly function is looking for row_operations 
 *row_operations[0] = If value is 0 it indicate that there are no operations.
 *row_operations[1] = there can be 1,2,3 
1 = rows, 2 = beginswith, 3 = contains
*/
int row_selection(int row_operation[], int argc, char *argv[], int rows_max)
{
	int k = 0;
 	array_inicialization(row_operation, ROW_OPERATION, 0);
	for(int i = 0; i < argc; i++) 
	{
		if(strcmp(argv[i], "rows")==0)
			k = argument_universal_simplify(row_operation, 3, 1, 1, rows_max, argv, i, argc);
		else if(strcmp(argv[i], "beginswith")==0)
			k = argument_universal_simplify(row_operation, 2, 1, 2, rows_max, argv, i, argc);
		else if(strcmp(argv[i], "contains")==0)
			k = argument_universal_simplify(row_operation, 2, 1, 3, rows_max, argv, i, argc);
	}
	if(k<0)
		return k; 
	return row_operation[0];
}
/*
 * It ll just store num to all int in array 
 * */ 
int array_inicialization(int array[], int array_size, int num)
{
	for(int i = 0; i < array_size; i++)
		array[i] = num;
	return 0;
}
//Function is looking for edit data operation.
//irow 1 2 //arow 3 //drow 4 5 //drows 6 7 8 ///icol 9 10 //acol 11 //dcol 12 13 //dcols 14 15 16
// * 1. cset C STR 2. tolower C, 3. toupper C, 4. round C, 5. int C, 6. copy N M, 7.swap N M, 8.move N M;
int edit_decider(int edit_table[], int *max_row, int true_size, char file[],int column_max, char separator)
{
	int max_row_int = *max_row;
	if(edit_table[0] == 1)
	{
		if(edit_table[1] == 1)
			true_size = true_size + irow(edit_table[2],true_size, file, column_max, separator, &max_row_int);
		if(edit_table[3] == 1)
			true_size = true_size + irow(max_row_int, true_size, file, column_max, separator, &max_row_int);		
		if(edit_table[4] == 1)
			drows(file, true_size, edit_table[5], edit_table[5]+1);
		if(edit_table[6] == 1)
			drows(file, true_size, edit_table[7], edit_table[8]);
		if(edit_table[9] == 1)
			true_size = true_size + icol(file, true_size, edit_table[10] - 1, column_max, separator, max_row_int);
		if(edit_table[11] == 1)
			true_size = true_size + icol(file, true_size, column_max+1, column_max, separator, max_row_int);
		if(edit_table[12] == 1)
			dcol(file, edit_table[13], edit_table[13] + 1, max_row_int, true_size, separator, column_max);
		if(edit_table[14] == 1)
			dcol(file, edit_table[15], edit_table[16] , max_row_int, true_size, separator, column_max);
	}
	return true_size;
}
//it works only for 3CHARS 
//Function save STR to cells in some column. 
int cset(char file[], int rows[], int data_edit[], int column, char separator, int rows_max, int file_size)
{
	int rows_sum = 0;
	for(int i = 0; i<rows_max;i++)
		if(rows[i]==1)
			rows_sum++;

	if(rows[0] == 1)
		file_size = file_size + rows_sum*(3+column);
	else
		file_size = file_size + rows_sum*3;
	int i = 0;
	int cell_counter;
	for(int row = 0; row < rows_max;row++)
	{
		if(file[i] == '\n')	
			i++;
		cell_counter = 0;
		if(rows[row] == 1)
		{
			if(column == - 1)  
			{
				while(i < file_size)
				{
					if(file[i] == separator)
						i++;
					for(int x = 3; x<23;x++)
					{
						file_size++;
						create_space(file, file_size, i);
						file[i] = data_edit[x];
						i++;
					}
					for(i=i; file[i] != separator;i++)
					{
						if(file[i] == '\n')	
							break;
						file[i] = '\0';
					}
					if(file[i] == '\n')	
						break;
			}
		}
			else  
			{
				while(i < file_size)
				{	
					if(file[i] == separator)
					{
						cell_counter++;
						i++;
						if(file[i] == '\n')
							break;
					}
					if(cell_counter == column)
					{
						for(int x = 3; x<23;x++)
						{
							file_size++;
							create_space(file, file_size, i);
							file[i] = data_edit[x];
							i++;
						}
						for(i=i; file[i] != separator;i++)
						{
							if(file[i] == '\n')	
								break;
							file[i] = '\0';
						}
						for(i = i; (file[i] != '\n');)
							i++; 
						break;
					}
					i++;
				}
			}
		}
		else
			while(file[i] != '\n')
				i++;
	}
	return file_size;

}
 /*tolower/toupper C 
 *This function can be combinated with row_selection. 
 * if u use beginswith or contains. C number after command ll be ignored.
 * Operation:	1 = tolower 
 *		2 = toupper
 * column 	-1 = do it for all 
*/
/* int C, round C 
 * Round ll round the number 
 * int ll delete the floating point 
*/
int tolower_toupper_round_int(char file[], int rows[], int column, char separator, int file_size, int operation)
{
	int rows_counter = 0;
	int column_counter = 0;
	if(column == -1) //This is for all the columns. 
		for(int i=0;i<file_size;i++)
		{
			if(file[i]== '\n')
			{	
				rows_counter++;
				continue;
			}	
			if(file[i]==separator)
				i++;
			if(rows[rows_counter]==1)
			{
				if(operation == 1)
					file[i] =tolower(file[i]);
				else if(operation == 2)
					file[i] = toupper(file[i]);
			}
		}
				
	else	//here is a chosen column. 
	{
		for(int i=0;i<file_size;i++)
		{
			if(rows[rows_counter] != 1)
			{
				rows_counter++;
				column_counter = 0;
				while(file[i] != '\n')
					i++;
				continue;
			}	
			if(file[i]=='\n')
			{
				rows_counter++;
				column_counter = 0;
				continue;
			}
			if(file[i]==separator)
			{
				i++;
				column_counter++;
			}
			if(column_counter == column)
			{
				for(i=i;i<file_size;i++)
				{
					if(file[i]==separator)
					{
						while(file[i] != '\n')
							i++;
						column_counter = 0;
						rows_counter++;

						break;
					}
					if(file[i]=='\n')
					{
						column_counter = 0;
						rows_counter++;
						break;
					}
					switch(operation)
					{
						case 1:
							file[i] = tolower(file[i]);
							break;
						case 2:
							file[i] = toupper(file[i]);
							break;
						case 3:
							i = round_floatingP_delete(file, &file[i], i, 1, separator);
							break;
						case 4:
							i = round_floatingP_delete(file, &file[i], i, 2, separator);
							break;
						default:
							break;
					}
				}

			}
		}
	}	
	return 0;
}
/*
 * this function will round number or delete floating point
 * operation 1 = round
 * operation 2 = deleting_floating_point 
*/ 
int round_floatingP_delete(char file[], char* pfile, int start, int operation, char separator)
{
	double number = 0;
	int inumber = 0;
	char *startfile;
	char *endfile;
	int i = start;	

	if(isdigit(file[start]))
	{
		endfile = startfile = pfile;
		for(i=start;((file[i] != separator)&&(file[i] != '\n'));i++)	
		{}
		number = strtod(pfile, &endfile-1); //store number that begins in char[] memory addres pfile and ends in endfile 
		if(operation == 1)
			inumber = (int)(number + 0.5);
		else
			inumber = (int)(number);
		startfile = (char*)&inumber;
		for(i=start+1;(file[i] != '\n');i++)	
		{	
			if(file[i] == separator)
				break;
			file[i] = '\0';	
		}
	}
	else
		return i;
	return i;
}
/*
 *This function ll copy swap or move values form cells in column N to cels in column M
 * operations: 1copy 2swap 3move
 */
int swap_value(int *one, int *two)
{
	int k = *one;
	*one = *two;
	*two = k;
	return 0;
}
/* function just copy from one cell to another and return position in file[] which is value of int i*/
int copy(char file[], int i, int *file_size, int column_counter, int column_M, char separator)
{
	char help_var[MAX_CELL_SIZE];
	int f, cell_size, k, n;
	f = cell_size = k = n = 0; //helping variables inicialization 
	
	cell_size = save_cel(file, i, separator, help_var);	
	*file_size = *file_size + cell_size;
	for(i = i;file[i] != '\n';i++)
	{	
		if(column_counter == column_M)
		{
			for(int z = 0;z<cell_size;z++)
				create_space(file, *file_size, i+z);
			f = (i + cell_size);
			for(i = i;i<f;i++)
			{
				file[i] = help_var[k];
				k++;
			}
			break;
		}	
		if(file[i] == separator)
			column_counter++;
	}
	for(i=i;(file[i] != separator);i++)
	{
		if(file[i] == '\n')
		{	
			n = 1;
			break;
		}
		file[i] = '\0';	
	}
	if(n == 0)
		for(i=i;(file[i]!='\n');i++)
		{}
	return i;
}
/* it ll swap 2 columns  */ 
int swap(char file[], int i, int *file_size, int column_counter, int column_M, char separator)
{
	char help_var[MAX_CELL_SIZE];
	char help_var_two[MAX_CELL_SIZE];
	int f, cell_size, k, n, z;
	int cell_position_one, cell_position_two;
	int cell_size_two = 0; 
	f = cell_size = k = n = z = 0; //helping variables inicialization 
					
	cell_size = save_cel(file, i, separator, help_var); //store first cell 	
	*file_size = *file_size + cell_size;
	cell_position_one = i;
	for(i = i;file[i] != '\n';i++)
	{	
		if(column_counter == column_M)
		{
			cell_size_two = save_cel(file, i, separator, help_var_two); //store second cell 	
			cell_position_two = i;
			*file_size = *file_size + cell_size;
			//store to M cell	
			for(z = 0;z<cell_size;z++ )
				create_space(file, *file_size, i+z);
			f = (i + cell_size);
			for(i = cell_position_two;i<f;i++)
			{
				file[i] = help_var[k];
				k++;
			}
			for(i=i;(file[i] != separator);i++)
			{
				if(file[i] == '\n')
					break;
				file[i] = '\0';
			}	
			i = cell_position_one;
			k = 0;
			//store to N cell
			for(z = 0;z<cell_size_two;z++ )
				create_space(file, *file_size, i+z);
			f = (i + cell_size_two);
			for(i = cell_position_one;i<f;i++)
			{
				file[i] = help_var_two[k];
				k++;
			}
			for(i=i;(file[i] != separator);i++)
				file[i] = '\0';
			for(i=i;((file[i] != '\n')&&(i<*file_size));i++)
			{}
			break;
		}
		if(file[i] == separator)
			column_counter++;
	}
	return i;
}
//it ll move column N in front of column M 
int move(char file[], int i, int *file_size, int column_counter, int column_M, char separator)
{
	int z, k,f, cell_size;
	z = k = f = 0;  //inicialization
	char help_var[MAX_CELL_SIZE];

	cell_size = save_cel(file, i, separator, help_var); //store first cell 	
	*file_size = *file_size + cell_size+1;
	for(i=i;i<*file_size;i++)
	{	
		if(file[i] == '\n')
			break;
		if(file[i] == separator)
		{
			file[i] = '\0';
			break;
		}
		file[i] = '\0';
	}
	for(i = i;i<*file_size;i++)
	{
		if((column_counter == column_M)||(file[i] == '\n'))
		{
			if(file[i]=='\n')
			{
				for(z = 0;z<cell_size+1;z++ )
					create_space(file, *file_size, i+z);
				f = (i + cell_size);
				file[i] = separator;
				for(i = i+1;i<f+1;i++)
				{
					file[i] = help_var[k];
					k++;
				}
				break;
			}
			i = i-2;
			for(z = 0;z<cell_size+1;z++)
				create_space(file,*file_size, i+z);
			f = (i + cell_size);
			for(i = i;i<f;i++)
			{
				file[i] = help_var[k];
				k++;
			}
			file[i] = separator;
			break;
		}
		if(file[i] == separator)
			column_counter++;
	}
	column_counter = z = k = 0;  //inicialization
	return i;
}
int copy_swap_move(char file[], int rows[], int column_N, int column_M, char separator, int file_size, int operation, int column_max)
{
	int rows_counter, column_counter;
	rows_counter = column_counter = 0;
	//first 3 if are fixing some erors 
	
	if(column_N < 0)
		column_N = 0;
	if(column_N == column_M)
		return file_size;
	if(column_N > column_M)
		swap_value(&column_M, &column_N);
	if(column_M <= column_max)
	{
		for(int i=0;i<file_size;i++)
		{
			if(rows[rows_counter] != 1)
			{
				rows_counter++;
				column_counter = 0;
				while(file[i] != '\n')
					i++;
				continue;
			}	
			if(file[i]=='\n')
			{
				rows_counter++;
				column_counter = 0;
				continue;
			}
			if(file[i]==separator)
			{
				i++;
				column_counter++;
			}
			if(column_counter == column_N)
			{
				for(i=i;i<file_size;i++)
				{
					if(operation == 1)//copy	
						i = copy(file, i, &file_size, column_counter, column_M, separator);
					else if(operation == 2)//swap 
						i = swap(file,i, &file_size,column_counter, column_M, separator);
					else if(operation == 3)//move
						i = swap(file,i, &file_size,column_counter, column_M, separator);
					break;
				}
				column_counter =0;
				rows_counter++;
				
			}
		}
	}	
	else 
		return -1; //there is no such a column 

	return file_size;
}
int save_cel(char file[], int start, char separator, char cell[])
{
	int cels_size = 0;
	int j = 0;
	for(int i=start; ((file[i] != separator) && (file[i] != '\n')); i++)
	{
		cell[j] = file[i];
		j++;
		cels_size++;
	}	
	return cels_size;
}

/*Function will find rows that, should be edite with some data_edit operation*/
//cels[0] = 1; it means that all cells should be editet else means that there is some chosen column
//there are 3 operations:
//rows N M - It say that u have to edit rows from n to m, and thah means for all columns 		
//
//
int rows(char file[], int row_operation[], int file_size, int rows[] ,int row_max, char separator)
{	
	int i = 0;
	int cell_counter;
	if(row_operation[1] == 1)//rows
	{
		for(int i=row_operation[2]; i<row_operation[3]; i++)
			rows[i] = 1; 
	}
	else if(row_operation[1] == 2)//BW 
	{
		cell_counter = 0;
		for(int rows_counter = 0; rows_counter <= row_max;)
		{	
			if(row_operation[2] == 0)
			{
				if((file[i] == row_operation[3])&&(file[i+1]== row_operation[4])&&(file[i+2]==row_operation[5]))
					rows[rows_counter] = 1;
				while((file[i] != '\n')&&(i< file_size))
					i++;
			}
			else
			{
				while(i < file_size)
				{	
					if(file[i] == separator)
						cell_counter++;
					if(file[i] == '\n')
						break;
					if(cell_counter == row_operation[2])
					{	
						if((file[i+1] == row_operation[3])&&(file[i+2]== row_operation[4])&&(file[i+3]==row_operation[5]))
							rows[rows_counter] = 1;
						while((file[i] != '\n')&&(i< file_size))
							i++;
						break;
					}
					i++;
				}
			}
			i++;
			cell_counter = 0;
			rows_counter++;
		}	
	}	
	else if(row_operation[1] == 3)//CO
	{
		cell_counter = 0;
		for(int rows_counter = 0; rows_counter <= row_max;)
		{	
			if(row_operation[2] == 0)
			{
				for(i=i;file[i] != separator;i++)
					if(file[i] == row_operation[3])
						if((file[i+1] == row_operation[4]) && (file[i+2] == row_operation[5]))
							rows[rows_counter] = 1;
				while((file[i] != '\n')&&(i< file_size))
					i++;
			}
			else
			{	while(i < file_size)
				{	
					if(file[i] == separator)
						cell_counter++;	
					if(cell_counter == row_operation[2])
					{
						for(int j=i;file[j+1] != separator;j++)
							if(file[j+1] == row_operation[3])
								if((file[j+2] == row_operation[4]) && (file[j+3] == row_operation[5]))
									rows[rows_counter] = 1;
						while((file[i] != '\n')&&(i< file_size))
							i++;
					}
					if(file[i] == '\n')
						break;
					i++;
				}
			}
			i++;
			cell_counter = 0;
			rows_counter++;
		}	
	}
	return 0;
}
// * 1. cset C STR 2. tolower C, 3. toupper C, 4. round C, 5. int C, 6. copy N M, 7.swap N M, 8.move N M;
//1 = rows, 2 = beginswith, 3 = contains
//array_inicialization [0] = 1 = rows N M; [0] = 2 = beginswith; [0] = 3 = contains 
//cels_0 = 0 means that i ll be editing all cells 
int data_decider(int edit_data[], int row_selection[], int max_row, int true_size, char file[], int column_max, char separator)
{
	int rows_edit[max_row];	 //This variable is important becouse i ll calculate what rows and cels should this comand edit
	array_inicialization(rows_edit, max_row, 0);
	
	if(row_selection[0] == 1) //If there is any row_selection argument, it ll decide what rows and columns to edit based on it  
		rows(file, row_selection,true_size, rows_edit, max_row, separator);
	else
		array_inicialization(rows_edit, max_row, 1);

	switch(edit_data[1])
	{
		case 1://cset
			true_size = cset(file, rows_edit, edit_data, edit_data[2], separator, max_row, true_size);
			break;
		case 2://tolower
			tolower_toupper_round_int(file, rows_edit, edit_data[2], separator, true_size, 1);
			break;
		case 3://toupper
			tolower_toupper_round_int(file, rows_edit, edit_data[2], separator, true_size, 2);
			break;
		case 4://round
			tolower_toupper_round_int(file, rows_edit, edit_data[2], separator, true_size, 3); //operation 2 is toupper etc....
			break;
		case 5://int 
			tolower_toupper_round_int(file, rows_edit, edit_data[2], separator, true_size, 4); //operation 2 is toupper etc....
			break;
		case 6://copy
			true_size = copy_swap_move(file, rows_edit, edit_data[2] ,edit_data[3], separator, true_size, 1, column_max);
			if(true_size == -1)
				return -1;
			break;
		case 7://swap
			true_size = copy_swap_move(file, rows_edit, edit_data[2] ,edit_data[3], separator, true_size, 2, column_max);
			if(true_size == -1)
				return -1;
			break;
		case 8://move
			true_size = copy_swap_move(file, rows_edit, edit_data[2] ,edit_data[3], separator, true_size, 3, column_max);
			if(true_size == -1)
				return -1;
			break;
		default:
			return 0;
			break;
	}
	return true_size;
}
