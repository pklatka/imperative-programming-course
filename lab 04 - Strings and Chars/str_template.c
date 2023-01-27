#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

#define MAX_LINE 128

int count[MAX_DIGRAMS] = {0};

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp(const void *a, const void *b)
{
	int va = *(int *)a;
	int vb = *(int *)b;
	if (count[va] == count[vb])
		return va - vb; // sort alphabetically if counts equal
	return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di(const void *a, const void *b)
{
	int va = *(int *)a;
	int vb = *(int *)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS)
		return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb])
		return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stdin
void wc(int *nl, int *nw, int *nc)
{
	*nl = 0, *nw = 0, *nc = 0;
	char line[MAX_LINE];
	while (fgets(line, MAX_LINE, stdin) != NULL)
	{
		*nl += 1;
		for (int i = 0; i < MAX_LINE; i++)
		{
			*nc += 1;
			if (line[i] == 10)
			{
				if (i >= 1 && line[i - 1] != 10 && line[i - 1] != 32 && line[i - 1] != 9)
				{
					*nw += 1;
				}
				break;
			}

			if (i >= 1 && line[i - 1] != 10 && line[i - 1] != 32 && line[i - 1] != 9 && (line[i] == 10 || line[i] == 32 || line[i] == 9))
			{
				*nw += 1;
			}
		}
	}
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stdin. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt)
{
	// Clean garbage
	int ascii[MAX_CHARS];
	for (int i = 0; i < MAX_CHARS; i++)
	{
		ascii[i] = i;
		count[i] = 0;
	}
	// Read line
	char line[MAX_LINE];
	while (fgets(line, MAX_LINE, stdin) != NULL)
	{
		for (int i = 0; i < MAX_LINE; i++)
		{
			if (line[i] == 10)
			{
				break;
			}
			if (FIRST_CHAR <= line[i] && line[i] < LAST_CHAR)
			{
				count[line[i] - FIRST_CHAR]++;
			}
		}
	}
	qsort(ascii, MAX_CHARS, sizeof(int), cmp);

	*n_char = ascii[char_no - 1] + FIRST_CHAR;
	*cnt = count[ascii[char_no - 1]];
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stdin. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[])
{
	int ascii[MAX_DIGRAMS];
	for (int i = 0; i < MAX_DIGRAMS; i++)
	{
		ascii[i] = i;
		count[i] = 0;
	}
	char line[MAX_LINE];
	while (fgets(line, MAX_LINE, stdin) != NULL)
	{
		if (line[0] == 10)
		{
			continue;
		}
		for (int i = 1; i < MAX_LINE; i++)
		{
			if (line[i] == 10)
			{
				break;
			}
			if (FIRST_CHAR <= line[i - 1] && line[i - 1] < LAST_CHAR && FIRST_CHAR <= line[i] && line[i] < LAST_CHAR)
			{
				// Equation: (ascii)first*MAX_CHARS + (ascii)second
				int index = (line[i - 1] - FIRST_CHAR) * MAX_CHARS + line[i] - FIRST_CHAR;
				count[index]++;
			}
		}
	}
	qsort(ascii, MAX_DIGRAMS, sizeof(int), cmp_di);

	digram[1] = (ascii[digram_no - 1] % MAX_CHARS + FIRST_CHAR);
	digram[0] = (ascii[digram_no - 1] - digram[1] + FIRST_CHAR) / MAX_CHARS + FIRST_CHAR;
	digram[2] = count[ascii[digram_no - 1]];
}

// Count block and line comments in the text read from stdin. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter)
{
	int line[MAX_LINE]; // tu zapisujemy stringa
	int line_counter = 0;
	int block_counter = 0;
	int flag_block = 0;

	int c;
	int length;
	for (length = 0; length < MAX_LINE && (c = getchar()) != EOF && c != '\n'; length++)
	{
		line[length] = c;
	}

	while (length != 0 || c != EOF)
	{
		for (int i = 1; i < length; ++i)
		{
			if (flag_block == 0)
			{

				if (line[i] == 47 && line[i - 1] == 47)
				{

					line_counter++;
					break;
				}

				if (line[i] == 42 && line[i - 1] == 47)
				{
					block_counter++;
					flag_block = 1;
				}
			}
			if (line[i] == 47 && line[i - 1] == 42)
			{
				flag_block = 0;
				i++;
			}
		}
		length = 0;
		for (length = 0; length < MAX_LINE && (c = getchar()) != EOF && c != '\n'; length++)
		{
			line[length] = c;
		}
	}

	*block_comment_counter = block_counter;
	*line_comment_counter = line_counter;
}

int read_line()
{
	char line[MAX_LINE];
	int n;

	fgets(line, MAX_LINE, stdin); // to get the whole line
	sscanf(line, "%d", &n);
	return n;
}

int main(void)
{
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int digram[3];

	to_do = read_line();
	switch (to_do)
	{
	case 1: // wc()
		wc(&nl, &nw, &nc);
		printf("%d %d %d\n", nl, nw, nc);
		break;
	case 2: // char_count()
		char_no = read_line();
		char_count(char_no, &n_char, &cnt);
		printf("%c %d\n", n_char, cnt);
		break;
	case 3: // digram_count()
		char_no = read_line();
		digram_count(char_no, digram);
		printf("%c%c %d\n", digram[0], digram[1], digram[2]);
		break;
	case 4:
		find_comments(&line_comment_counter, &block_comment_counter);
		printf("%d %d\n", block_comment_counter, line_comment_counter);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define MAX_LINE MAX_ID_LEN *MAX_IDS

int index_cmp(const void *, const void *);
int cmp(const void *, const void *);

char tab[MAX_IDS][MAX_ID_LEN] = {0};
int tab_index = 0;
int line[MAX_LINE] = {0};

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"};

int is_identifier(int str[], int length)
{
	if (str[0] != 95 && !(str[0] >= 97 && str[0] <= 122) && !(str[0] >= 65 && str[0] <= 90))
		return 0;

	for (int i = 1; i < length; i++)
	{
		if (str[i] != 95 && !(str[i] >= 97 && str[i] <= 122) && !(str[i] >= 65 && str[i] <= 90) && !(str[i] >= 48 && str[i] <= 57))
		{
			return 0;
		}
	}

	return 1;
}

int compare_strings(int str[], int length, char str2[], int length2)
{
	if (length != length2)
		return 0;
	for (int i = 0; i < length; i++)
	{
		if (str[i] != str2[i])
		{
			return 0;
		}
	}
	return 1;
}

int is_not_restricted(int str[], int length)
{
	for (int j = 0; j < sizeof(keywords) / sizeof(keywords[0]); j++)
	{
		int keyword_length = strlen(keywords[j]);
		if (keyword_length != length)
			continue;
		int isok = 1;
		for (int i = 0; i < length; i++)
		{
			if (str[i] != keywords[j][i])
			{
				isok = 0;
				break;
			}
		}
		if (isok)
		{
			return 0;
		}
	}

	return 1;
}

void update_value(int str[], int length)
{
	// Check if there is str in tab array
	for (int i = 0; i < tab_index; i++)
	{
		if (compare_strings(str, length, tab[i], strlen(tab[i])))
		{
			return;
		}
	}
	// Update benc
	int i;
	for (i = 0; i < length; i++)
	{
		tab[tab_index][i] = str[i];
	}
	tab[tab_index][i] = '\0';
	tab_index++;
}

int find_idents()
{
	int c;
	int length;
	for (length = 0; length < MAX_LINE && (c = getchar()) != EOF && c != '\n'; length++)
	{
		line[length] = c;
	}
	// Flags
	int block_comment_flag = 0, string_flag = 0;

	while (length != 0 || c != EOF)
	{
		// ----------------------- main code ----------------------------
		int id_holder_index = 0;
		int id_holder[MAX_ID_LEN] = {0};
		for (int i = 0; i < length; i++)
		{
			if (line[i] == 39)
			{
				i++;
				continue;
			}

			if ((line[i] >= 32 && line[i] <= 47) || (line[i] >= 58 && line[i] <= 64) || (line[i] >= 91 && line[i] <= 96) || (line[i] >= 123 && line[i] <= 126) || line[i] == 9)
			{
				if (is_identifier(id_holder, id_holder_index) && is_not_restricted(id_holder, id_holder_index))
				{
					// Update value
					update_value(id_holder, id_holder_index);
				}

				id_holder_index = 0;
				// Skip null char
				i++;
			}
			id_holder[id_holder_index] = line[i];
			id_holder_index++;
		}
		if (is_identifier(id_holder, id_holder_index) && is_not_restricted(id_holder, id_holder_index))
		{
			// Update value
			update_value(id_holder, id_holder_index);
		}

		// ----------------------- end of code ----------------------------
		length = 0;
		for (length = 0; length < MAX_LINE && (c = getchar()) != EOF && c != '\n'; length++)
		{
			line[length] = c;
		}
	}
	return tab_index;
}

int cmp(const void *first_arg, const void *second_arg)
{
	char *a = *(char **)first_arg;
	char *b = *(char **)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void *first_arg, const void *second_arg)
{
	int a = *(int *)first_arg;
	int b = *(int *)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(void)
{
	printf("%d\n", find_idents());
	return 0;
}
