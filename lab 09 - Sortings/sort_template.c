#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person
{
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2)
{
	// Convert p1 to Person pointer and get the value (without dereferencing because of -> operator usage).
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

// Read data to Person array (till EOF)
int read_person_array(Person *persons)
{
	int persons_tab_index = 0;
	while (1)
	{
		Person new_person;

		// Read age, name and surname
		char *name = &new_person.first_name, *surname = &new_person.last_name, buffer[2 * MAX_STR_LEN + 2], age[MAX_STR_LEN];
		int age_index = 0, name_index = 0, surname_index = 0, buffer_index = 0;

		if (fgets(buffer, 2 * MAX_STR_LEN + 2, stdin) == NULL)
		{
			return persons_tab_index;
		}

		if (strlen(buffer) <= 1)
		{
			continue;
		}

		while (buffer[buffer_index] != ' ')
		{
			age[age_index] = buffer[buffer_index];
			age_index++;
			buffer_index++;
		}
		age[age_index] = '\0';
		new_person.age = (int)strtol(age, &age[age_index], 10);
		buffer_index++;
		while (buffer[buffer_index] != ' ')
		{
			name[name_index] = buffer[buffer_index];
			name_index++;
			buffer_index++;
		}
		name[name_index] = '\0';
		buffer_index++;
		while (buffer[buffer_index] != '\n')
		{
			surname[surname_index] = buffer[buffer_index];
			surname_index++;
			buffer_index++;
		}
		surname[surname_index] = '\0';

		persons[persons_tab_index] = new_person;
		persons_tab_index++;
	}
}

// Print Person array
void print_person_array(Person *persons, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d %s %s\n", persons[i].age, persons[i].first_name, persons[i].last_name);
	}
}

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (in that order)
int cmp_lines(const void *l1, const void *l2)
{
	// Convert input to human-readable variables
	char *p1 = (char *)l1;
	char *p2 = (char *)l2;

	// Read person no. 1
	char name_1[MAX_STR_LEN], surname_1[MAX_STR_LEN], age_str_1[MAX_STR_LEN];
	int age_1 = 0, age_index = 0, name_index = 0, surname_index = 0, buffer_index = 0, is_1_woman = 0;

	while (p1[buffer_index] != ' ')
	{
		age_str_1[age_index] = p1[buffer_index];
		age_index++;
		buffer_index++;
	}
	age_str_1[age_index] = '\0';
	age_1 = (int)strtol(age_str_1, &age_str_1[age_index], 10);
	buffer_index++;
	while (p1[buffer_index] != ' ')
	{
		name_1[name_index] = p1[buffer_index];
		name_index++;
		buffer_index++;
	}
	name_1[name_index] = '\0';
	if (name_1[name_index - 1] == 'a')
	{
		is_1_woman = 1;
	}
	buffer_index++;
	while (p1[buffer_index] != '\n')
	{
		surname_1[surname_index] = p1[buffer_index];
		surname_index++;
		buffer_index++;
	}
	surname_1[surname_index] = '\0';

	// Read person no. 2
	char name_2[MAX_STR_LEN], surname_2[MAX_STR_LEN], age_str_2[MAX_STR_LEN];
	int age_2 = 0, is_2_woman = 0;
	age_index = 0, name_index = 0, surname_index = 0, buffer_index = 0;
	while (p2[buffer_index] != ' ')
	{
		age_str_2[age_index] = p2[buffer_index];
		age_index++;
		buffer_index++;
	}
	age_str_2[age_index] = '\0';
	age_2 = (int)strtol(age_str_2, &age_str_2[age_index], 10);
	buffer_index++;
	while (p2[buffer_index] != ' ')
	{
		name_2[name_index] = p2[buffer_index];
		name_index++;
		buffer_index++;
	}
	if (name_2[name_index - 1] == 'a')
	{
		is_2_woman = 1;
	}
	name_2[name_index] = '\0';
	buffer_index++;
	while (p2[buffer_index] != '\n')
	{
		surname_2[surname_index] = p2[buffer_index];
		surname_index++;
		buffer_index++;
	}
	surname_2[surname_index] = '\0';

	// Sort woman first
	if (is_1_woman != is_2_woman)
	{
		return is_2_woman - is_1_woman;
	}

	if (is_1_woman == 0)
	{
		// Sort men by last name
		return strcmp(surname_1, surname_2);
	}
	else
	{
		// Sort by age
		return age_1 - age_2;
	}
}

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN])
{
	int persons_tab_index = 0;
	while (1)
	{
		// Read age, name and surname
		char buffer[2 * MAX_STR_LEN + 2];

		if (fgets(buffer, 2 * MAX_STR_LEN + 2, stdin) == NULL)
		{
			return persons_tab_index;
		}

		if (strlen(buffer) <= 1)
		{
			continue;
		}

		strcpy(lines[persons_tab_index], buffer);
		persons_tab_index++;
	}
}

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%s", lines[i]);
	}
}

// -------------------------------------------------

int read_int()
{
	char buf[MAX_STR_LEN];
	int n;
	fgets(buf, MAX_STR_LEN, stdin);
	sscanf(buf, "%d", &n);
	return n;
}

int main(void)
{
	int to_do = read_int();
	int n;
	Person persons[MAX_PERSONS];
	char lines[MAX_PERSONS][MAX_STR_LEN];
	switch (to_do)
	{
	case 1:
		n = read_person_array(persons);
		qsort(persons, (size_t)n, sizeof(Person), cmp_person);
		print_person_array(persons, n);
		break;
	case 2:
		n = read_lines(lines);
		qsort(lines, (size_t)n, MAX_STR_LEN, cmp_lines);
		print_lines(lines, n);
		break;
	default:
		printf("Nothing to do for %d\n", to_do);
		break;
	}
}
