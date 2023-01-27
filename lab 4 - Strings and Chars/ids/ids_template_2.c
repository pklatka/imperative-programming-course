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

int index_cmp(const void *, const void *);
int cmp(const void *, const void *);

char tab[MAX_IDS][MAX_ID_LEN] = {0};
int used_ids[MAX_IDS] = {0};
int tab_index = 0;

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"};

int is_identifier(char *str)
{
	int str_len = strlen(str);
	if (str[0] != 95 && !(str[0] >= 97 && str[0] <= 122) && !(str[0] >= 65 && str[0] <= 90))
		return 0;

	for (int i = 1; i < str_len; i++)
	{
		if (str[i] != 95 && !(str[i] >= 97 && str[i] <= 122) && !(str[i] >= 65 && str[i] <= 90) && !(str[i] >= 48 && str[i] <= 57))
		{
			return 0;
		}
	}

	return 1;
}

int is_not_restricted(char *str)
{
	for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
	{
		if (0 == strcmp(str, keywords[i]))
		{
			return 0;
		}
	}
	return 1;
}

void update_value(char *str)
{
	if (tab_index >= MAX_IDS)
		return;
	for (int i = 0; i < tab_index; i++)
	{
		if (0 == strcmp(str, tab[i]))
		{
			if (used_ids[i] == 1)
				used_ids[i] += 1;
			return;
		}
	}
	strcpy(tab[tab_index], str);
	strcpy(str, "");
	used_ids[tab_index] = 1;
	tab_index++;
}

int find_idents()
{
	char line[MAX_ID_LEN * MAX_IDS] = {0};
	char id[MAX_ID_LEN] = {0};
	int started_block_comment = 0;
	int started_char_sign = 0;
	int started_string_sign = 0;
	while (fgets(line, MAX_ID_LEN * MAX_IDS, stdin) != NULL)
	{
		int i = 0;
		while (i < MAX_ID_LEN * MAX_IDS && line[i] != '\n')
		{
			// -------- comment condition --------
			if (started_block_comment == 0 && line[i - 1] == '/' && line[i] == '/')
			{
				// Start of line comment
				break;
			}
			else if (started_block_comment == 0 && line[i - 1] == '/' && line[i] == '*')
			{
				// Start of block comment
				started_block_comment = 1;
				i++;
			}
			else if (started_block_comment == 1 && line[i - 1] == '*' && line[i] == '/')
			{
				// Stop of block comment
				started_block_comment = 0;
				i++;
				strcpy(id, "");
			}
			if (started_block_comment)
			{
				i++;
				continue;
			}
			// -------- char condition --------
			if (started_char_sign == 0 && line[i] == '\'')
			{
				// Start of block comment
				started_char_sign = 1;
			}
			else if (started_char_sign == 1 && line[i] == '\'')
			{
				// Stop of block comment
				started_char_sign = 0;
				strcpy(id, "");
			}
			if (started_char_sign)
			{
				i++;
				continue;
			}
			// -------- string condition --------
			if (started_string_sign == 0 && line[i] == '\"')
			{
				// Start of block comment
				started_string_sign = 1;
			}
			else if (started_string_sign == 1 && line[i] == '\"')
			{
				// Stop of block comment
				started_string_sign = 0;
				strcpy(id, "");
			}
			if (started_string_sign)
			{
				i++;
				continue;
			}

			if ((line[i] >= 32 && line[i] <= 47) || (line[i] >= 58 && line[i] <= 64) || (line[i] >= 91 && line[i] <= 96) || (line[i] >= 123 && line[i] <= 126) || line[i] == 9)
			{
				if (is_identifier(id) && is_not_restricted(id))
				{
					// Update value
					update_value(id);
				}
				else
				{
					strcpy(id, "");
				}
			}
			else
			{
				// send + more = ram
				char debil[2] = {0};
				debil[0] = line[i];
				debil[1] = '\0';
				strcat(id, debil);
			}
			i++;
		}
		if (is_identifier(id) && is_not_restricted(id))
		{
			// Update value
			update_value(id);
		}
	}

	int unique = 0;
	for (int i = 0; i < tab_index; i++)
	{
		if (used_ids[i] != 0)
		{
			unique++;
			// printf("%s\n", tab[i]);
		}
	}
	return unique;
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
