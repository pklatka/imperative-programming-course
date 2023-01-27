#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAB_SIZE 1000
#define BUF_SIZE 1000

double get(int cols, int row, int col, const double *A)
{
	return *(A + cols * row + col);
}

void set(int cols, int row, int col, double *A, double value)
{
	*(A + row * cols + col) = value;
}

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB)
{
	for (int i = 0; i < rowsA; i++)
	{
		for (int j = 0; j < colsB; j++)
		{
			AB[i * rowsA + j] = 0.0;
			for (int k = 0; k < colsA; k++)
			{
				// double a = A[i * colsA + k];
				// double b = B[k * rowsA + j];
				AB[i * rowsA + j] += A[i * colsA + k] * B[k * rowsA + j];
			}
		}
	}
}

void read_mat(int rows, int cols, double *t)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			scanf("%lf", t + i * cols + j);
		}
	}
}

void print_mat(int rows, int cols, double *t)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			printf("%.2f ", *(t + i * cols + j));
		}
		printf("\n");
	}
}

int read_char_lines(char *tab[])
{
	int ptr_tab_index = 0;
	int isEOF = 0;
	while (1)
	{
		// Extract doubles
		char tmp = ' ';
		char *buff = (char *)malloc(BUF_SIZE);
		char *start_address = buff;
		int i = 0;
		while (tmp != '\n')
		{
			if (scanf("%c", &buff[i]) == EOF)
			{
				isEOF = 1;
				break;
			}

			// In case something goes wrong
			if (buff[i] == '\n' && i != 0)
			{
				break;
			}

			if (buff[i] == '\n' && i == 0)
			{
				i--;
			}

			i++;
		}

		if (isEOF)
		{
			char *x = &buff[i - 1];
			x += 1;
			tab[ptr_tab_index] = x;
			break;
		}
		else
		{
			tab[ptr_tab_index] = start_address;
			ptr_tab_index++;
		}
	}

	return ptr_tab_index;
}

void write_char_line(char *tab[], int n)
{
	char *test = tab[n - 1];
	while (test != tab[n] && *test != '\n')
	{
		printf("%c", *test);
		test += 1;
	}
}

void delete_lines(char *tab[], int line_count)
{
	for (int i = 0; i < line_count; i++)
	{
		free(tab[i]);
	}
}

int read_dbl_lines_v1(double *ptr_tab[])
{
	int ptr_tab_index = 1;
	int i = 0;
	double *buff = ptr_tab[0];
	int isEOF = 0;
	while (1)
	{
		// Extract doubles
		char tmp = ' ';
		double *start_address = &buff[i];
		while (tmp != '\n')
		{
			if (scanf("%lf%c", &buff[i], &tmp) == EOF)
			{
				isEOF = 1;
				break;
			}
			i++;
		}

		if (isEOF)
		{
			double *x = &buff[i - 1];
			x += 1;
			ptr_tab[ptr_tab_index] = x;
			break;
		}
		else
		{
			ptr_tab[ptr_tab_index] = start_address;
			ptr_tab_index++;
		}
	}

	return ptr_tab_index;
}

void write_dbl_line_v1(double *ptr_tab[], int n)
{
	// for (int i = 0; i < ptr_tab_index; i++)
	// {
	double *test = ptr_tab[n];
	while (test != ptr_tab[n + 1])
	{
		printf("%.2f ", *test);
		test += 1;
	}
	// printf("%c", '\n');
	// }
}

int main(void)
{
	int to_do;

	scanf("%d", &to_do);

	double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	char *char_lines_table[TAB_SIZE];
	double series_table[TAB_SIZE];
	double *ptr_table[TAB_SIZE];

	switch (to_do)
	{
	case 1:
		scanf("%d %d", &rowsA, &colsA);
		read_mat(rowsA, colsA, A);
		scanf("%d %d", &rowsB, &colsB);
		read_mat(rowsB, colsB, B);
		prod_mat(rowsA, colsA, colsB, A, B, C);
		print_mat(rowsA, colsB, C);
		break;
	case 2:
		scanf("%d", &n);
		ptr_table[0] = series_table;
		lines_counter = read_dbl_lines_v1(ptr_table);
		write_dbl_line_v1(ptr_table, n);
		break;
	case 3:
		scanf("%d", &n);
		lines_counter = read_char_lines(char_lines_table);
		write_char_line(char_lines_table, n);
		delete_lines(char_lines_table, lines_counter);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
