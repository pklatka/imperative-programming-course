#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n)
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n)
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[])
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			printf("%.4f ", A[indices[i]][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p], B[p][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n)
{
	// row_A = m, col_A = p, row_B = p, col_B = n;
	for (int row = 0; row < m; row++)
	{
		for (int col = 0; col < n; col++)
		{
			double product = 0;
			for (int i = 0; i < p; i++)
			{
				product += A[row][i] * B[i][col];
			}
			AB[row][col] = product;
		}
	}
}

// Calculate matrix - vector product
void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int n) {}

void backward_substit(double A[][SIZE], double x[], int n) {}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {}

// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n)
{
	// Square matrix
	double determinant = 1;
	for (int i = 0; i < n; i++)
	{
		// Check elements below main element
		for (int j = i + 1; j < n; j++)
		{
			double multiplier = (-1) * (A[j][i] / A[i][i]);
			for (int z = i; z < n; z++)
			{
				A[j][z] += A[i][z] * multiplier;
			}
		}
		determinant *= A[i][i];
		if (A[i][i] == 0)
			return NAN;
	}

	return determinant;
}

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps)
{
	// ---------------------  Matrix triangularization -------------
	for (int p = 0; p < n; p++)
	{
		x[p] = b[p];
	}
	// Set up permutation vector for columns
	int permutation_vector[SIZE];
	double swaps = 1.0;
	for (int i = 0; i < n; i++)
	{
		permutation_vector[i] = i;
	}
	double determinant = 1.0;
	for (int i = 0; i < n - 1; i++)
	{
		int swapped = 0;
		// Non-zero elements to the top
		for (int j = i; j < n; j++)
		{
			if (A[permutation_vector[i]][i] != 0)
			{
				break;
			}

			if (A[permutation_vector[j]][i] != 0 && j != i)
			{
				// Swap elements
				// Swap rows
				int tmp = permutation_vector[i];
				permutation_vector[i] = permutation_vector[j];
				permutation_vector[j] = tmp;
				double d_tmp = x[i];
				x[i] = x[j];
				x[j] = d_tmp;
				swaps *= (-1.0);
				swapped = 1;
				break;
			}
		}
		double element = 0.0;
		for (int j = 0; j < n; j++)
		{
			if (element < A[permutation_vector[i]][i])
			{
				element = A[permutation_vector[i]][i];
			}
		}
		if (element < 0)
		{
			element *= (-1.0);
		}
		if (element < eps && swapped == 1)
		{
			return 0;
		}
		for (int j = i + 1; j < n; j++)
		{
			if (A[permutation_vector[i]][i] != 0)
			{
				double multiplier = (A[permutation_vector[j]][i] / A[permutation_vector[i]][i]);
				if (A[permutation_vector[j]][i] != 0)
				{
					for (int z = 0; z < n; z++)
					{
						A[permutation_vector[j]][z] -= A[permutation_vector[i]][z] * multiplier;
					}
					x[j] -= multiplier * x[i];
				}
			}
		}
	}
	for (int i = 0; i < n; i++)
	{
		if (A[permutation_vector[i]][i] == 0)
		{
			return 0;
		}
		else
		{
			determinant *= A[permutation_vector[i]][i];
		}
	}
	determinant *= swaps;

	// Get results
	for (int i = n - 1; i >= 0; i--)
	{
		double result = x[i];
		for (int j = n - 1; j >= i; j--)
		{
			if (j == i)
			{
				result = result / A[permutation_vector[i]][j];
			}
			else
			{
				result -= x[j] * A[permutation_vector[i]][j];
			}
		}
		x[i] = result;
	}
	return determinant;
}

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps)
{
	// ---------------------  Matrix triangularization -------------

	// Add vector b to matrix A
	for (int i = 0; i < n; i++)
	{
		for (int j = n; j < 2 * n; j++)
		{
			if (i + n == j)
			{
				A[i][j] = 1.0;
			}
			else
			{
				A[i][j] = 0.0;
			}
		}
	}
	// int mn = n+1; // New matrix size

	// Set up permutation vector for columns
	int permutation_vector[SIZE];
	double swaps = 1.0;
	for (int i = 0; i < n; i++)
	{
		permutation_vector[i] = i;
	}
	double determinant = 1.0;

	for (int i = 0; i < n; i++)
	{
		// Get index of maximum column element
		int max_el_index = permutation_vector[i];
		for (int j = i + 1; j < n; j++)
		{
			double x1 = fabs(A[permutation_vector[j]][i]);
			double x2 = fabs(A[max_el_index][i]);
			if (x1 > x2)
			{
				max_el_index = permutation_vector[j];
			}
		}
		if (max_el_index != permutation_vector[i])
		{
			// Swap rows
			int tmp = permutation_vector[i];
			permutation_vector[i] = permutation_vector[max_el_index];
			permutation_vector[max_el_index] = tmp;
			swaps *= (-1.0);
		}
		// Check elements below main element
		// print_mat_ind(A, n, n + 1, permutation_vector);

		for (int j = i + 1; j < n; j++)
		{
			double multiplier = (-1) * (A[permutation_vector[j]][i] / A[permutation_vector[i]][i]);
			for (int z = 0; z < 2 * n; z++)
			{
				A[permutation_vector[j]][z] += A[permutation_vector[i]][z] * multiplier;
			}
		}
		if (fabs(A[permutation_vector[i]][i]) < eps)
			return 0;
	}

	for (int i = 0; i < n; i++)
	{
		determinant *= A[permutation_vector[i]][i];
	}

	if (determinant == 0)
		return 0;

	// Identity
	for (int i = 0; i < n; i++)
	{
		// print_mat_ind(A, n, 2 * n, permutation_vector);
		if (A[permutation_vector[i]][i] == 1)
			continue;

		// Change main element to 1
		double dzielnik = A[permutation_vector[i]][i];
		for (int z = 0; z < 2 * n; z++)
		{
			A[permutation_vector[i]][z] /= dzielnik;
		}
		// print_mat_ind(A, n, 2 * n, permutation_vector);

		for (int j = 0; j < n; j++)
		{
			if (j == i)
				continue;
			double multiplier = (-1) * (A[permutation_vector[j]][i] / A[permutation_vector[i]][i]);
			for (int z = 0; z < 2 * n; z++)
			{
				A[permutation_vector[j]][z] += A[permutation_vector[i]][z] * multiplier;
			}
		}
		// print_mat_ind(A, n, 2 * n, permutation_vector);
	}
	// print_mat_ind(A, n, 2 * n, permutation_vector);
	// ----------- Rewrite array ---------
	for (int i = 0; i < n; i++)
	{
		for (int j = n; j < 2 * n; j++)
		{
			B[i][j - n] = A[permutation_vector[i]][j];
		}
	}

	return determinant * swaps;
}

int main(void)
{

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf("%d", &to_do);

	switch (to_do)
	{
	case 1:
		scanf("%d %d %d", &m, &p, &n);
		read_mat(A, m, p);
		read_mat(B, p, n);
		mat_product(A, B, C, m, p, n);
		print_mat(C, m, n);
		break;
	case 2:
		scanf("%d", &n);
		read_mat(A, n, n);
		printf("%.4f\n", gauss_simplified(A, n));
		break;
	case 3:
		scanf("%d", &n);
		read_mat(A, n, n);
		read_vector(b, n);
		det = gauss(A, b, x, n, eps);
		printf("%.4f\n", det);
		if (det)
			print_vector(x, n);
		break;
	case 4:
		scanf("%d", &n);
		read_mat(A, n, n);
		printf("%.4f\n", matrix_inv(A, B, n, eps));
		print_mat(B, n, n);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	return 0;
}