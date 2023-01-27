// Celem tego pliku jest przedstawienie wszystkich funkcji co były na labach
// ... i na wykładzie :o

// Podstawowa bibiloteka
#include <stdio.h>
#include <float.h>  // Do posiadania np DBL_MAX czyli takie float('inf')
#include <string.h> // Do wykonwyania funkcji na napisach
#include <stdlib.h> // Np. do wywołania rand()

// Możemy definiować sobie różne stałe
#define CONSTANT_IN_C 1
#define str(A) #A // makro zamieniające argument na tekst

void chapter_01_basics(int zmienna1, int zmienna2)
{
    // int zmienna1, int zmienna2 -> formalne paramety funkcji
    // chapter_01_basics(0,1) -> 0 i 1 -> argumenty funkcji

    // Podstawowe zmienne
    int liczba = 1;

    char *napis_inaczej_tablica_charow = "Jan Kowalski";

    int true_false = 0; // W C nie ma typu bool

    const int stala = 5; // Zmienna niemodyfikowalna

    char napis = {'K', 'R', 'A', 'K', '\0'}; // Pamiętać o znaku specjalnym na końcu napisu

    // ZAWSZE RESETUJ KAŻDĄ ZMIENNĄ!!!!

    int tab[5] = {0}; // Wypełnienie tablicy zerami

    /*
        printf - wypluj zmienną
        %d - int
        %f - float
        %lf - double
        %c - char
        %s - string

        %.2f -> float z dwoma miejscami po przecinku

        printf stosuje promocję typu czyli nie trzeba pisać %lf
    */
    printf("%d ", zmienna1);

    /*
        scanf - wczytaj zmienną
        %d - int
        %f - float
        %lf - double
        %c - char
        %s - string

        Jako argument podajemy adres zmiennej!!!
    */
    scanf("%d", &zmienna2);

    // Funkcja rand()
    int seed = 20000;
    srand((unsigned)seed); // ustawienie ziarna generatora (dla powtarzalnosci wynikow)
    int a = 5, b = 10;
    rand() % (b - a + 1) + a;

    // Operatory bitowe
    // a <<=b  - przesuwa bity a w lewo o b miejsc
    // ~ NOT -> inwersja bitów
    // & - AND
    // | - OR
    // ^ EXOR - jeżeli 0 lub 1, 1 lub 0 zwraca 1
    // x << y - przesuwa w lewo bity x o y miejsc (<=> * 2^y)
    // x >> y - przesuwa w prawo bity x o y miejsc (<=> / 2^y)

    /*
      Dodatkowe uwagi:
      - do switcha nie można przekazywać pointera
      - jak dzielimy inta przez np. 2 i chcemy otrzymać double to dzielimy przez 2.0!
      - duże zmienne warto ustawiać jako zmienne globalne np. tablica 1000x1000
  */
}

// Prototyp funkcji:
void chapter_02_functions(void);

void chapter_02_functions(void)
{
    /*
         void empty_func(void){}

         Powinno się dawać void w funkcji gdy nie ma argumentów.
         Oczywiście nie jest to konieczne.
    */

    /*
         Każde wywołanie funkcji jest odkładane na stos. Tam zapisywane są:
             - adres powrotu gdzie ma wskazywać Stack Pointer
             - użyte argumenty wywołania
             - zmienne deklarowane lokalnie
         Pamiętajmy o Instruction Pointer, który wskazuje na wykonywaną instrukcję zapisaną np. w RAM

        Przed każdym wywołaniem funkcji wykonwyany jest tzw. prolog i epilog.
    */

    /*
         Uwagi:
            - funkcja nie może zwrócić tablicy (ze względu na stack pointer)
            - operator const w funkcji może oznaczać że parametr jest parametrem wejściowym, a nie, że parametr może być obliczany wewnątrz funkcji
            - funkcja może zwrócić wskaźnik ale tylko do zmiennej podanej jako parametr albo do zmiennej zadeklarowanej globalnie
    */
}

void chapter_03_declarations_and_types(void)
{
    // enum - typ wyliczeniowy
    // enum przyjmuje tylko liczby całkowite!
    enum cool_stuff
    {
        false = 0,
        true = 1,
        hehe = 12
    };
    printf("%d", hehe); // prints: 12

    /*
        tablice - obszar ciągły w pamięci
        tablice dwuwymiarowe: wzór: i*COLS + j, i-nr wiersza

        designated initializer - wskazanie wybranych indeksów i nadanie im wartości, reszta to zera
        tab[SIZE] = {[0] = 5, [1] = 2};

        definicja tablicy 2D jako argument w funkcji:
        int tab [][COLS]

        tablice VLA - Variable Length Arrays - tablice dynamiczne
        deklarowane z użyciem malloc i free

        Uwagi:
            - wielkość tablicy: sizeof(tab) == sizeof(type) * SIZE
            - obliczanie wielkości tablicy: sizeof(tab) / (sizeof(tab[0]) == sizeof(type))
            - tak rozmiaru tablicy nie da się obliczyć w funkcji!
            - nazwa tablicy to jednocześnie pointer do pierwszego elementu tablicy
            - należy uważać żeby nie przekroczyć rozmiaru tablicy podczas indeksowania
    */

    /*
        Typ struct
            - deklarując pola w struct nie możemy ich inicjować
            - możemy defaultowo inicjować ich pola
            - struktura nie musi zająć ciągłego obszaru pamięci dlatego jej rozmiar to nie jest suma jej pól

        Pola bitowe
            - całkowitoliczbowe pola struktur, których wielkość jest ograniczona do zadanej liczby bitów
    */
    struct osoba
    {
        char imie[32];
        char nazwisko[32];
        int wiek;
        unsigned short color : 4; // Pole bitowe
    } osoba_default = {"", "", 88};

    // Default assginment:
    struct osoba o0 = osoba_default;

    struct osoba o1 = {"Robert", "Kubica", 88};
    printf("%s", o1.imie);

    struct osoba *o2 = malloc(sizeof(struct osoba)); // Drugi sposób na inicjację obiektu struct
    free(o2);                                        // Koniecznie należy zwolnić pamięć !!!

    /*
        Unie
            - jej rozmiar to wielkość największej zmiennej
            - programista jest odpowiedzialny za przydział pamięci
    */
    union data_union
    {
        int int_data;
        char char_data;
        void *ptr_data;
    };

    // typedef [coś] [nazwa] - takie coś ala #define ale można dłuższe rzeczy definiować
    typedef struct test
    {
        int test2;
    } test;
    // Powyższa deklaracja pozwala na ominięcie słówka struct
}

void chapter_04_pointers(void)
{
    // Pointer to inaczej zmienna przechowująca adres innej zmiennej
    int variable = 5;
    int *pointer = &variable;
    int **pointer_2 = &pointer;                                         // Pointer to pointer - zapisujemy adres pointera
    void *no_type_pointer = &variable;                                  // Wskaźnik typu void
    printf("%d %d %d", *pointer, **pointer_2, *(int *)no_type_pointer); // Deferencja zmiennej

    // Dostęp do struct jako pointer
    struct osoba
    {
        char imie[32];
        char nazwisko[32];
        int wiek;
        struct osoba *mother; // Odnośnik do innych obiektów
        struct osoba *father;
    } osoba_default = {"", "", 88};

    struct osoba o1 = {"Robert", "Kubica", 88};

    struct osoba *pointer_to_o1 = &o1;
    // Korzystamy z operatora -> zamiast kropki
    printf("%s", pointer_to_o1->imie);

    // Wskaźniki typu void
    void *pointer_void = &o1;

    // Arytmetyka wskaźników

    // Odczyt bajt bo bajcie: zrzutowanie na char *
    char *new_pointer = (char *)pointer_void;

    new_pointer += 1; // new_pointer + sizeof(char) -> 1 bajt

    // Wypisanie i-tego bitu: *new_pointer&(1 << i), i<=7

    /*
        Tablice:

        Dostęp do tablic jak zwykle: tab[i][j][k]...;

        Pointer do tablicy 2D:
            - int a[ROWS][COLS]
            - int (*p)[COLS] a;

        Tablica wskaźników:

        // Alloc memory (pointer which points to another pointer)
        int **tab = (int **)malloc(n * sizeof(int *));
        for (int i = 0; i < n; i++)
        {
            tab[i] = (int *)malloc(n * siezof(n));
        }

        Second option:
        int (*d)[n] = malloc(n * n * sizeof(int));

    */

    /*
        Uwagi:
            - szczególną wartością jest NULL - wartość zerowa - często traktowana jako znacznik błędu
            - operator const w funkcji może oznaczać że parametr jest parametrem wejściowym, a nie, że parametr może być obliczany wewnątrz funkcji
    */

    // Wskaźniki do funkcji np. do struktury:
    // Można zadeklarować wskaźnik
    // typedef [typ] (*nazwa_funkcji)(argumenty);
    typedef void (*chapter_01_basics)(void);
    // Wtedy w paramterach funkcji: void test(int x, chapter_01_basics f){}
    // Można też wpisać bezpośrednio np. int (*funkcja)(char *char_arr)

    /*
        Przykład wykorzystania: qsort
        qsort(void *base, size_t num, size_t width, *compare)
            base - adres tablicy
            num - liczba el. w tablicy
            width - rozmiar elementu tablicy
            compare - funkcja porównująca

        compare(const void *el1, const void *el2){
            return:
            < 0 if el1 < el2
            = 0 if el1 == el2
            > 0 if el1 > el2
        }
    */
}

void chapter_05_strings(void)
{
    /*
        Wczytywanie stringów linia po linii:
        char line[MAX_BUFFER];
        while (fgets(line, MAX_BUFFER, stdin) != NULL){
            tu coś robimy z line...(zakończonym '\0')
        }

        Przydatne: sscanf(line, "%d %*d %s",v1,v2) -> zczytuje zmienne z tekstu (%*d skipuje zmienną)

        Przegląd funkcji z biblioteki string.h
            - size_t strlen(const char * string) -> zwraca długość stringa
            - char * strcpy(char *dest, const char *soruce) -> kopiuje stringa source do dest (len(dest) >= len(source) + 1)
            - char * strcat(char * dest, const char * source) -> konkatenacja stringów: dest += source. (len(dest) >= len(source) + len(dest) + 1)
            - int strcmp(const char *str1,const char *str2) - porównanie stringów
            - int strcoll(const char *str1,const char *str2) - porównanie stringów ale interpretuje teksty zgodnie z ustawieniami locale (setlocale(LC_ALL, ""))
            - char * strtok(char * str, const char * delimiters) - wydzielanie stringa, coś ala .split(delimiters)
            - const char * strchr(const char * str, int c) - funkcja, która znajduje znak c w str lub NULL jeżeli nie znaleziono
            - char * strstr(const char*str, const char*pattern) - znajduje pattern w str i zwraca indeks dopasowania, jeżeli nie znajdzie to zwraca NULL
    */

    // Przykład strtok:
    char str[] = "- This, a sample string.";
    char delimeter = " ,.-";
    char *pch = strtok(str, delimeter);
    while (pch != NULL)
    {
        printf("%s\n", pch);
        // Aby otrzymać następne oddzielone napisy przekazujemy NULL
        pch = strtok(NULL, delimeter);
    }

    /*
        Inne funkcje:
            - strcasecmp() - porównanie bez uwzględnienia wielkości znaków
            - stricmp() - porównanie tekstów ignorując wielkość znaków
            - strdup(s) - zwraca wskaźnik do kopii stringa s (kopiuje go)

        UWAGA: przy alokowaniu stringa dynamicznie pamiętać o znaku \0 tzn. malloc(len(str) + 1)
    */
}

void chapter_06_dynamic_memory(void)
{
    /*
        VLA (dynamiczne tablice) ogranicza jedynie rozmiar stosu

        Alokowanie pamięci:
        void *malloc (size_t size) -> NULL gdy wystąpił błąd (warto sprawdzać po alokacji)
        void *calloc(size_t num ,size_t size) -> num elementów o size, wypełnia pamięć zerami

        Powiększanie pamięci:
        void * realloc(void * ptr, size_t size)

        Kopiowanie pamięci
        void * memcpy(void * dest, void * source, size_t size)

        Zwalnianie pamięci:
        void free (void *memblock)

        Deklarowanie tablicy 2D
        np. double (*m)[cols] = malloc(rows*cols*sizeof(double))

        Uwagi:
            - int *p = malloc(n * sizeof(int)); -> pamiętać o sizeof typu
            - Zawsze pamiętać o ZWOLNIENIU PAMIĘCI
            - Podczas zwalniania pamięci w tablicy wskaźników (**) najpierw musimy zwolnić pamięć dla elementów tablicy

        Lista ogólnego przeznaczenia:
        Była w zadaniu o listach, warto ogarnąć
        Ale tutaj implementacja z wykładu:

        typedef struct tagListElement
        {
            struct tagListElement *next;
            void *data;
        } ListElement;

        typedef void (*ConstDataFp)(const void *);
        typedef void (*DataFp)(void *);
        typedef int (*CompareDataFp)(const void *, const void *);
        typedef struct tagList
        {
            ListElement *head;
            ListElement *tail;
            int size;
            ConstDataFp dumpData;
            DataFp freeData;
            CompareDataFp compareData;
        } List;

        void init(List * list)
        {
            list->head = 0;
            list->tail = 0;
            list->size = 0;
            list->dumpData = 0;
            list->freeData = 0;
            list->compareData = 0;
        }

        void freeList(List * list)
        {
            ListElement *current = 0;
            current = list->head;
            while (current != 0)
            {
                ListElement *todelete = current;
                current = current->next;
                if (list->freeData)
                    list->freeData(todelete->data);
                free(todelete);
            }
            list->size = 0;
            list->head = 0;
            list->tail = 0;
        }

        void pushBack(List * list, void *data)
        {
            ListElement *element = (ListElement *)malloc(sizeof(ListElement));
            element->data = data;
            element->next = 0;
            if (list->tail)
                list->tail->next = element;
            list->tail = element;
            if (!list->head)
                list->head = list->tail;
            list->size++;
        }

        void pushFront(List * list, void *data)
        {
            ListElement *element = (ListElement *)malloc(sizeof(ListElement));
            element->data = data;
            element->next = list->head;
            list->head = element;
            if (!list->tail)
                list->tail = list->head;
            list->size++;
        }

        ListElement *findInsertionPoint(const List *list, ListElement *element)
        {
            ListElement *insertionPoint = 0;
            ListElement *current = 0;
            for (current = list->head; current != 0; current = current->next)
            {
                if (list->compareData(current->data, element->data) <= 0)
                    insertionPoint = current;
            } // else break;
            return insertionPoint;
        }

        void add(List * list, void *data)
        {
            ListElement *element = (ListElement *)malloc(sizeof(ListElement));
            element->next = 0;
            element->data = data;
            if (!list->compareData)
            { // bez sortowania
                if (list->tail == 0)
                {
                    list->head = element;
                    list->tail = element;
                }
                else
                {
                    list->tail->next = element;
                    list->tail = element;
                }
            }
            else
            { // sortowanie podczas wstwiania
                ListElement *insertionPt = findInsertionPoint(list, element);
                if (insertionPt == 0)
                {
                    element->next = list->head;
                    list->head = element;
                    if (list->tail == 0)
                        list->tail = list->head;
                }
                else
                {
                    element->next = insertionPt->next;
                    insertionPt->next = element;
                }
            }
            list->size++;
        }

        void *popFront(List * list)
        {
            if (!list->head)
                return 0;
            void *data = data = list->head->data;
            ListElement *toDel = list->head;
            list->head = list->head->next;
            free(toDel);
            if (list->head == 0)
                list->tail = 0;
            list->size--;
            return data;
        }

        void dumpList(const List *list)
        {
            ListElement *i;
            for (i = list->head; i != 0; i = i->next)
            {
                if (list->dumpData)
                    list->dumpData(i->data);
                else
                    printf("%p ", i->data);
            }
        }

        void printString(const void *data)
        {
            printf("%s ", data);
        }

    */
}

void chapter_07_files(void)
{
    // W skrócie...

    /*

        PLIK ZAWSZE NALEŻY ZAMKNĄĆ!

        Struktura file
            - stdin - wejście z klawiatury
            - stdout - wyjście standartowe
            - strerr - strumień błędów

        Funkcje:
            - FILE *fopen(const char *filename, const char *mode) - zwraca wskaźnik do pliku (mode np. "r" - read)
            - int fclose(FILE *stream) - zamyka plik
            - fprintf(FILE * stream, const char *format [, argumenty]) - wpisywanie do pliku zadanego wzorca
            - fscanf(FILE *stream, const char *format) - odczyt argumentów z pliku tekstowego
    */
}

// Główna funkcja programu w C
int main(int argc, char **argv)
{
    chapter_04_pointers();
    // Zazwyczaj wychodzi się returnem
    // Ale można też użyć exit(exit_code) lub abort()
    return 0;
}