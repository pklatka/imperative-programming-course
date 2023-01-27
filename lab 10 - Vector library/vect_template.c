#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector
{
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person
{
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size)
{
	vector->capacity = block_size;
	vector->element_size = element_size;
	vector->data = malloc(block_size * element_size);
	vector->size = 0;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity)
{
	if (vector->capacity < new_capacity)
	{
		vector->capacity = new_capacity;
		vector->data = realloc(vector->data, vector->capacity * vector->element_size);
	}
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size)
{
	if (vector->size < new_size)
	{
		char *ptr = (char *)vector->data;
		// Move pointer
		ptr += (vector->element_size * vector->size);
		for (size_t i = 0; i < new_size - vector->size; i++)
		{
			// Init zero element
			memcpy(ptr, calloc(1, vector->element_size), vector->element_size);
			ptr += vector->element_size;
		}
	}
	vector->size = new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value)
{
	// Check if needs an increment
	if (vector->capacity < vector->size + 1)
	{
		reserve(vector, vector->capacity * 2);
	}
	char *ptr = (char *)vector->data;
	// Move pointer
	ptr += (vector->element_size * vector->size);
	// Address of value
	memcpy(ptr, value, vector->element_size);
	vector->size += 1;
}

// Remove all elements from the vector
void clear(Vector *vector)
{
	vector->size = 0;
}

// Remove the last element from the vector
void pop_back(Vector *vector)
{
	vector->size -= 1;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, int index, void *value)
{

	// 	// Realloc vector
	if (vector->size + 1 > vector->capacity)
	{
		reserve(vector, vector->capacity * 2);
	}

	if (index == vector->size)
	{
		push_back(vector, value);
		return;
	}

	// 	// Move pointer do insert index
	char *ptr = (char *)vector->data;
	ptr += (vector->element_size) * index;

	char *last_element = (char *)vector->data;
	last_element += vector->element_size * vector->size;
	char *previous_element = last_element - vector->element_size;
	// 	// Move data
	int vector_size = vector->size;
	while (vector_size > index)
	{
		memcpy(last_element, previous_element, vector->element_size);
		vector_size -= 1;
		last_element = previous_element;
		previous_element -= vector->element_size;
	}

	// Copy value
	memcpy(last_element, value, vector->element_size);
	vector->size += 1;
}

// Erase element at position index
void erase(Vector *vector, int index)
{
	char *ptr = (char *)vector->data;
	ptr += (vector->element_size) * index;

	char *next_element = (char *)vector->data;

	next_element += (vector->element_size) * (index + 1);

	int c = index;
	while (c < vector->size - 1)
	{
		memcpy(ptr, next_element, vector->element_size);
		ptr = next_element;
		next_element += vector->element_size;
		c += 1;
	}

	vector->size -= 1;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, int (*cmp)(const void *, const void *))
{
	char *ptr = (char *)vector->data;
	int dynamic_vector_size = vector->size, i = 0;
	while (i < dynamic_vector_size)
	{
		if (cmp(ptr, value) == 0)
		{
			erase(vector, i);
			dynamic_vector_size--;
		}
		else
		{
			ptr += vector->element_size;
			i++;
		}
	}
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *))
{
	char *ptr = (char *)vector->data;
	int dynamic_vector_size = vector->size, i = 0;
	while (i < dynamic_vector_size)
	{
		if (predicate(ptr))
		{
			erase(vector, i);
			dynamic_vector_size--;
		}
		else
		{
			ptr += vector->element_size;
			i++;
		}
	}
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector)
{
	vector->data = realloc(vector->data, vector->size * vector->element_size);
	vector->capacity = vector->size;
}

// Print integer vector
void print_vector_int(Vector *vector)
{
	printf("%d\n", vector->capacity);
	char *ptr = (char *)vector->data;
	for (size_t i = 0; i < vector->size; i++)
	{
		printf("%d ", *(int *)ptr);
		ptr += vector->element_size;
	}
}

// Print char vector
void print_vector_char(Vector *vector)
{
	printf("%d\n", vector->capacity);
	char *ptr = (char *)vector->data;
	for (size_t i = 0; i < vector->size; i++)
	{
		printf("%c ", *ptr);
		ptr += vector->element_size;
	}
}

// Print vector of Person
void print_vector_person(Vector *vector)
{
	printf("%d\n", vector->capacity);
	char *ptr = (char *)vector->data;
	for (size_t i = 0; i < vector->size; i++)
	{
		Person tmp = *(Person *)ptr;
		printf("%d %s %s\n", tmp.age, tmp.first_name, tmp.last_name);
		ptr += vector->element_size;
	}
}

// integer comparator - increasing order
int int_cmp(const void *v1, const void *v2)
{
	int w1 = *(int *)v1;
	int w2 = *(int *)v2;

	if (w1 < w2)
	{
		return -1;
	}
	if (w1 > w2)
	{
		return 1;
	}
	return 0;
}

// char comparator - lexicographical order (case sensitive)
int char_cmp(const void *v1, const void *v2)
{
	char w1 = *(char *)v1;
	char w2 = *(char *)v2;

	if (w1 < w2)
	{
		return -1;
	}
	if (w1 > w2)
	{
		return 1;
	}
	return 0;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2)
{
	int age_1 = ((Person *)p1)->age;
	char *name_1 = ((Person *)p1)->first_name;
	char *surname_1 = ((Person *)p1)->last_name;

	int age_2 = ((Person *)p2)->age;
	char *name_2 = ((Person *)p2)->first_name;
	char *surname_2 = ((Person *)p2)->last_name;

	if (age_1 != age_2)
	{
		return age_2 - age_1;
	}
	else
	{
		// Compare names
		int compare = strcmp(name_1, name_2);
		if (compare == 0)
		{
			// Compare surnames
			return strcmp(surname_1, surname_2);
		}
		else
		{
			return compare;
		}
	}
}

// predicate: check if number is even
int is_even(void *value)
{
	return *(int *)value % 2 == 0;
}

// predicate: check if char is a vowel
int is_vowel(void *value)
{

	return *(char *)value == 'a' || *(char *)value == 'e' || *(char *)value == 'i' || *(char *)value == 'o' || *(char *)value == 'u' || *(char *)value == 'y' || *(char *)value == 'A' || *(char *)value == 'E' || *(char *)value == 'I' || *(char *)value == 'O' || *(char *)value == 'U' || *(char *)value == 'Y';
}

// predicate: check if person is older than 25
int is_older_than_25(void *person)
{
	Person tmp = *(Person *)person;
	return tmp.age > 25;
}

// -------------------------------------------------------------

void read_int(void *value)
{
	scanf("%d", (int *)value);
}

void read_char(void *value)
{
	char c[2];
	scanf("%s", c);
	*(char *)value = c[0];
}

void read_person(void *value)
{
	Person *person = (Person *)value;
	scanf("%d %s %s", &person->age, person->first_name, person->last_name);
}

void vector_test(Vector *vector, int n, void (*read)(void *),
				 int (*cmp)(const void *, const void *), int (*predicate)(void *))
{
	char op[2];
	int index;
	size_t size;
	void *v = malloc(vector->element_size);
	for (int i = 0; i < n; ++i)
	{
		scanf("%s", op);
		switch (op[0])
		{
		case 'p': // push_back
			read(v);
			push_back(vector, v);
			break;
		case 'i': // insert
			scanf("%d", &index);
			read(v);
			insert(vector, index, v);
			break;
		case 'e': // erase
			scanf("%d", &index);
			read(v);
			erase(vector, index);
			erase_value(vector, v, cmp);
			break;
		case 'd': // erase (predicate)
			erase_if(vector, predicate);
			break;
		case 'r': // resize
			scanf("%zu", &size);
			resize(vector, size);
			break;
		case 'c': // clear
			clear(vector);
			break;
		case 'f': // shrink
			shrink_to_fit(vector);
			break;
		case 's': // sort
			qsort(vector->data, vector->size,
				  vector->element_size, cmp);
			break;
		default:
			printf("No such operation: %s\n", op);
			break;
		}

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// print_vector_char(vector);
		// printf("%s", "\n");
		// printf("Size: %d\n", vector->capacity);
	}
	free(v);
}

int main(void)
{
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do)
	{
	case 1:
		init_vector(&vector_int, 4, sizeof(int));
		vector_test(&vector_int, n, read_int, int_cmp, is_even);
		print_vector_int(&vector_int);
		free(vector_int.data);
		break;
	case 2:
		init_vector(&vector_char, 2, sizeof(char));
		vector_test(&vector_char, n, read_char, char_cmp, is_vowel);
		print_vector_char(&vector_char);
		free(vector_char.data);
		break;
	case 3:
		init_vector(&vector_person, 2, sizeof(Person));
		vector_test(&vector_person, n, read_person, person_cmp, is_older_than_25);
		print_vector_person(&vector_person);
		free(vector_person.data);
		break;
	default:
		printf("Nothing to do for %d\n", to_do);
		break;
	}

	return 0;
}
