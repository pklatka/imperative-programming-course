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
    if (length <= 0)
        return;
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
    int block_comment_flag = 0;
    while (length != 0 || c != EOF)
    {
        // ----------------------- main code ----------------------------
        int id_holder_index = 0;
        int id_holder[MAX_ID_LEN] = {0};
        for (int i = 0; i < length; i++)
        {
            if (block_comment_flag)
            {
                if (line[i] == '/' && line[i - 1] == '*')
                    block_comment_flag = 0;
                continue;
            }
            // Block comment
            if (line[i] == '*' && line[i - 1] == '/')
            {
                block_comment_flag = 1;
                i += 1;
                continue;
            }
            // Inline comment
            if (line[i] == '/' && line[i - 1] == '/')
            {
                break;
            }
            // '' sign
            if (line[i] == 39)
            {
                i++;
                while (line[i] != '\'')
                {
                    // Detect escape char
                    if (line[i] == '\\')
                        i++;
                    i++;
                }
                continue;
            }
            // "" Sign
            if (line[i] == 34)
            {
                i++;
                while (line[i] != '\"')
                {
                    // Detect escape char
                    if (line[i] == '\\')
                        i++;
                    i++;
                }
                continue;
            }

            if ((line[i] >= ' ' && line[i] <= '/') || (line[i] >= ':' && line[i] <= '@') || (line[i] >= '[' && line[i] <= ']') || (line[i] >= '{' && line[i] <= '}') || line[i] == 9)
            {
                if (is_identifier(id_holder, id_holder_index) && is_not_restricted(id_holder, id_holder_index))
                {
                    // Update value
                    update_value(id_holder, id_holder_index);
                }

                id_holder_index = 0;
                // Skip null char?????
                // i++;
            }
            else
            {
                id_holder[id_holder_index] = line[i];
                id_holder_index++;
            }
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

    // Just for debug
    // for (int i = 0; i < tab_index; i++)
    // {
    //     printf("%s\n", tab[i]);
    // }

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
