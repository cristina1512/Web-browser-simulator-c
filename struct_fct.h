/* "Copyright [2025] <CRETU CRISTINA>"  [legal/copyright] [5] */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct page { // the structure of a page
    int id;
    char url[50];
    char *description;
};

typedef struct celula_stack {
    struct page *page;
    struct celula_stack *urm;
} TCelStack, *TStack;

struct tab { // the structure of a tab
    int id;
    struct page *currentPage;
    TStack backwardStack;
    TStack forwardStack;
};

typedef struct celula_lista { // the structure of a cell from a circular double linked list
    struct tab *tab;
    struct celula_lista *urm, *pre;
}TCelulaL, *TListaC;

struct tabsList { // the structure of a circular double linked list with sentinel
    TListaC santi; 
};

struct browser { // the structure of a browser
    struct tab *current;
    struct tabsList list;
};

void transform_line(char *line);
TStack Init_Stack(void);
void Init_Tab(struct tab *tab, int id);
int InsCellList (struct tabsList *list, int id);
struct browser* InitBrowser();
void FreeBrowser(struct browser *b);
void FreeStack(TStack *s);
void NEW_TAB(struct browser *b, int x);
void PRINT(FILE*fout,struct browser *b);
void CLOSE(FILE *fout, struct browser *b);
void OPEN(FILE *fout, struct browser *b, int nr);
void NEXT(struct browser *b);
void PREV(struct browser *b);
TStack copySinv (TStack stack);
int Push(TStack* vf, struct page *srcPage);
int Pop(TStack* vf);
int Make_Stack_Empty(TStack *stack);
int PAGE(FILE *fout, struct browser *b, int id, int nrpages, struct page *pages);
void print_stack(FILE *fout, TStack stack);
void PRINT_HISTORY(FILE *fout, struct browser *b, int id);
void BACKWARD(FILE *fout, struct browser *b);
void FORWARD(FILE *fout, struct browser *b);