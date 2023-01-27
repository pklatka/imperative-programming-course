#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct
{
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation(pair *relation, int n, pair new_pair)
{
	for (int i = 0; i < n; i++)
	{
		if (relation[i].first == new_pair.first && relation[i].second == new_pair.second)
		{
			return n;
		}
	}
	relation[n] = new_pair;
	return n + 1;
}

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *relation, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (relation[i].first == relation[i].second)
			continue;

		int is_ok = 0;
		for (int j = 0; j < n; j++)
		{
			if (i == j)
				continue;

			if ((relation[j].first == relation[i].first && relation[j].second == relation[i].first) || (relation[j].first == relation[i].second && relation[j].second == relation[j].second))
			{
				is_ok = 1;
				break;
			}
		}
		if (is_ok == 0)
		{
			return 0;
		}
	}
	return 1;
}
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *relation, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (relation[i].first == relation[i].second)
		{
			return 0;
		}
	}
	return 1;
}
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *relation, int n)
{
	for (int i = 0; i < n; i++)
	{
		int is_symetric = 0;
		for (int j = 0; j < n; j++)
		{
			if (relation[i].first == relation[j].second && relation[i].second == relation[j].first)
			{
				is_symetric = 1;
				break;
			}
		}
		if (is_symetric == 0)
		{
			return 0;
		}
	}
	return 1;
}
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *relation, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (relation[i].first == relation[j].second && relation[i].second == relation[j].first)
			{
				if (relation[i].first != relation[i].second)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *relation, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (relation[i].first == relation[j].second && relation[i].second == relation[j].first)
			{
				return 0;
			}
		}
	}
	return 1;
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *relation, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i != j && relation[i].second == relation[j].first)
			{
				int is_transitive = 0;
				for (int k = 0; k < n; k++)
				{
					if (relation[i].first == relation[k].first && relation[j].second == relation[k].second)
					{
						is_transitive = 1;
						break;
					}
				}
				if (is_transitive == 0)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

int compare(const void *a, const void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	// an easy expression for comparing
	return (int_a > int_b) - (int_a < int_b);
}

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *relation, int n)
{
	int a = is_reflexive(relation, n);
	int b = is_transitive(relation, n);
	int c = is_antisymmetric(relation, n);

	if (a != 0 && b != 0 && c != 0)
		return 1;
	return 0;
}
// A total order relation is a partial order relation that is connected
int is_total_order(pair *relation, int n)
{
	int a = is_partial_order(relation, n);
	int b = is_connected(relation, n);
	if (a != 0 && b != 0)
		return 1;
	return 0;
}
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *relation, int n)
{
	int domain[MAX_REL_SIZE], n_domain;
	n_domain = get_domain(relation, n, domain);
	for (int i = 0; i < n_domain; i++)
	{
		for (int j = 0; j < n_domain; j++)
		{
			int x = domain[i], y = domain[j];
			// Find in relation
			int found = 0;
			for (int k = 0; k < n; k++)
			{
				if ((relation[k].first == x && relation[k].second == y) || (relation[k].first == y && relation[k].second == x))
				{
					found = 1;
					break;
				}
			}
			if (found == 0)
				return 0;
		}
	}
	return 1;
}
int find_max_elements(pair *relation, int n, int *ans)
{
	int domain[MAX_REL_SIZE], n_domain, index = 0;
	n_domain = get_domain(relation, n, domain);
	for (int i = 0; i < n_domain; i++)
	{
		int is_max = 1;
		for (int j = 0; j < n; j++)
		{
			if (relation[j].first == domain[i] && relation[j].first != relation[j].second)
			{
				is_max = 0;
			}
		}
		if (is_max == 1)
		{
			// Remove duplicate
			int is_dup = 0;
			for (int j = 0; j < index; j++)
			{
				if (ans[j] == domain[i])
				{
					is_dup = 1;
				}
			}
			if (is_dup == 0)
			{
				ans[index] = domain[i];
				index++;
			}
		}
	}
	qsort(ans, index, sizeof(int), compare);
	return index;
}
int find_min_elements(pair *relation, int n, int *ans)
{
	int domain[MAX_REL_SIZE], n_domain, index = 0;
	n_domain = get_domain(relation, n, domain);
	for (int i = 0; i < n_domain; i++)
	{
		int is_max = 1;
		for (int j = 0; j < n; j++)
		{
			if (relation[j].second == domain[i] && relation[j].first != relation[j].second)
			{
				is_max = 0;
			}
		}
		if (is_max == 1)
		{
			// Remove duplicate
			int is_dup = 0;
			for (int j = 0; j < index; j++)
			{
				if (ans[j] == domain[i])
				{
					is_dup = 1;
				}
			}
			if (is_dup == 0)
			{
				ans[index] = domain[i];
				index++;
			}
		}
	}
	qsort(ans, index, sizeof(int), compare);
	return index;
}
int get_domain(pair *relation, int n, int *ans)
{
	int index = 0;
	for (int i = 0; i < n; i++)
	{
		int can_add = 1;
		for (int k = 0; k < index; k++)
		{
			if (ans[k] == relation[i].first)
			{
				can_add = 0;
				break;
			}
		}
		if (can_add)
		{
			ans[index] = relation[i].first;
			index++;
		}
	}
	for (int i = 0; i < n; i++)
	{
		int can_add = 1;
		for (int k = 0; k < index; k++)
		{
			if (ans[k] == relation[i].second)
			{
				can_add = 0;
				break;
			}
		}
		if (can_add)
		{
			ans[index] = relation[i].second;
			index++;
		}
	}
	qsort(ans, index, sizeof(int), compare);
	return index;
}

// Case 3:
int composition(pair *relation, int n, pair *relation2, int n2, pair *comp_relation)
{
	int index = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n2; j++)
		{
			if (relation[i].second == relation2[j].first)
			{
				pair new_pair;
				new_pair.first = relation[i].first;
				new_pair.second = relation2[j].second;
				index = add_relation(comp_relation, index, new_pair);
			}
		}
	}
	return index;
}

int cmp(pair p1, pair p2)
{
	if (p1.first == p2.first)
		return p1.second - p2.second;
	return p1.first - p2.first;
}
// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation)
{
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		pair new_pair;
		scanf("%d", &new_pair.first);
		scanf("%d", &new_pair.second);
		relation[i] = new_pair;
	}
	return n;
}

void print_int_array(int *array, int n)
{
	printf("%d\n", n);
	for (int i = 0; i < n; ++i)
	{
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void)
{
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];

	scanf("%d", &to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do)
	{
	case 1:
		printf("%d ", is_reflexive(relation, size));
		printf("%d ", is_irreflexive(relation, size));
		printf("%d ", is_symmetric(relation, size));
		printf("%d ", is_antisymmetric(relation, size));
		printf("%d ", is_asymmetric(relation, size));
		printf("%d\n", is_transitive(relation, size));
		break;
	case 2:
		ordered = is_partial_order(relation, size);
		n_domain = get_domain(relation, size, domain);
		printf("%d %d\n", ordered, is_total_order(relation, size));
		print_int_array(domain, n_domain);
		if (!ordered)
			break;
		int max_elements[MAX_REL_SIZE];
		int min_elements[MAX_REL_SIZE];
		int no_max_elements = find_max_elements(relation, size, max_elements);
		int no_min_elements = find_min_elements(relation, size, min_elements);
		print_int_array(max_elements, no_max_elements);
		print_int_array(min_elements, no_min_elements);
		break;
	case 3:
		size_2 = read_relation(relation_2);
		printf("%d\n", composition(relation, size,
								   relation_2, size_2, comp_relation));
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	return 0;
}
