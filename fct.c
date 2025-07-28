/* "Copyright [2025] <CRETU CRISTINA>"  [legal/copyright] [5] */
#include "struct_fct.h"

// eliminates whitespaces, newlines, carriage returns
void transform_line(char *line) {
  int len = strlen(line);
  while (len > 0 && (line[len - 1] == ' ' || line[len - 1] == '\n' ||
                     line[len - 1] == '\r')) {
    line[len - 1] = '\0';
    len--;
  }
}

// initializing a stack
TStack Init_Stack(void) { return NULL; }

// initialize a tab
void Init_Tab(struct tab *tab, int id) {
  if (!tab) {
    return;
  }
  tab->id = id;
  // starting page
  tab->currentPage = malloc(sizeof(struct page));
  if (!tab->currentPage) {
    free(tab);
    return;
  }
  tab->currentPage->id = 0;
  strncpy(tab->currentPage->url, "https://acs.pub.ro/",
          strlen("https://acs.pub.ro/"));
  int len = strlen("https://acs.pub.ro/");
  tab->currentPage->url[len] = '\0';
  tab->currentPage->description = malloc(100 * sizeof(char));
  if (!tab->currentPage->description) {
    free(tab->currentPage);
    free(tab);
    return;
  }
  len = strlen("Computer Science");
  strncpy(tab->currentPage->description, "Computer Science", len);
  tab->currentPage->description[len] = '\0';
  tab->backwardStack = Init_Stack();
  tab->forwardStack = Init_Stack();
}

/* insert a new cell in a circular double linked list */
int InsCellList(struct tabsList *list, int id) {
  if (!list || !list->santi) {
    return 0;
  }
  TListaC ultim = list->santi->pre;
  struct tab *new_tab = malloc(sizeof(struct tab));
  if (!new_tab)
    return 0;
  Init_Tab(new_tab, id);
  TListaC new_cell = (TListaC)malloc(sizeof(TCelulaL));
  if (!new_cell) {
    return 0;
  }
  new_cell->tab = new_tab;
  ultim->urm = new_cell;
  new_cell->pre = ultim;
  new_cell->urm = list->santi;
  list->santi->pre = new_cell;
  return 1;
}

// initialize the browser
struct browser *InitBrowser() {
  struct browser *b = malloc(sizeof(struct browser));
  if (!b) {
    return NULL;
  }
  // initialize the linked list
  b->list.santi = malloc(sizeof(TCelulaL));
  if (!b->list.santi) {
    free(b);
    return NULL;
  }
  b->list.santi->tab = malloc(sizeof(struct tab));
  if (!b->list.santi->tab) {
    free(b->list.santi);
    free(b);
    return NULL;
  }
  // the sentinel has -1 for the id
  Init_Tab(b->list.santi->tab, -1);
  // asiguram circularitatea listei
  b->list.santi->urm = b->list.santi;
  b->list.santi->pre = b->list.santi;
  // adding the starting tab
  InsCellList(&b->list, 0);
  b->current = b->list.santi->pre->tab;
  return b;
}

// free the memory allocated for the stack
void FreeStack(TStack *s) {
  while (*s) {
    // eliberam fiecare celula pe rand
    TStack aux = *s;
    *s = (*s)->urm;
    if (aux->page) {
      if (aux->page->description)
        free(aux->page->description);
      free(aux->page);
    }
    free(aux);
  }
}

// free the memory allocated for the browser
void FreeBrowser(struct browser *b) {
  if (!b)
    return;

  TListaC aux = b->list.santi->urm;
  while (aux != b->list.santi) {

    TListaC aux_urm = aux->urm;
    if (aux->tab) {
      FreeStack(&(aux->tab->backwardStack));
      FreeStack(&(aux->tab->forwardStack));
      if (aux->tab->currentPage) {
        if (aux->tab->currentPage->description)
          free(aux->tab->currentPage->description);
        free(aux->tab->currentPage);
      }
      free(aux->tab);
    }
    free(aux);
    aux = aux_urm;
  }

  if (b->list.santi->tab) {
    FreeStack(&(b->list.santi->tab->backwardStack));
    FreeStack(&(b->list.santi->tab->forwardStack));
    if (b->list.santi->tab->currentPage) {
      if (b->list.santi->tab->currentPage->description) {
        free(b->list.santi->tab->currentPage->description);
      }
      free(b->list.santi->tab->currentPage);
    }
    free(b->list.santi->tab);
  }
  free(b->list.santi);
  free(b);
}


void NEW_TAB(struct browser *b, int x) {
  if (InsCellList(&b->list, x)) {
    b->current = b->list.santi->pre->tab;
  }
}


void PRINT(FILE *fout, struct browser *b) {
  fprintf(fout, "%d ", b->current->id);
  TListaC cell = b->list.santi->urm;
  while (cell != b->list.santi && cell->tab != b->current)
    cell = cell->urm;
  cell = cell->urm;
  while (cell->tab != b->current) {
    if (cell->tab->id != -1)
      fprintf(fout, "%d ", cell->tab->id);
    cell = cell->urm;
  }
  fprintf(fout, "\n");
  fprintf(fout, "%s\n", b->current->currentPage->description);
}


void CLOSE(FILE *fout, struct browser *b) {
  if (b->current->id == 0) {
    fprintf(fout, "403 Forbidden\n");
    return;
  }
  TListaC cell = b->list.santi->urm;
  while (cell->tab != b->current && cell != b->list.santi) {
    cell = cell->urm;
  }
  b->current = cell->pre->tab;
  cell->pre->urm = cell->urm;
  cell->urm->pre = cell->pre;
  FreeStack(&(cell->tab->backwardStack));
  FreeStack(&(cell->tab->forwardStack));
  if (cell->tab->currentPage) {
    if (cell->tab->currentPage->description) {
      free(cell->tab->currentPage->description);
    }
    free(cell->tab->currentPage);
  }
  free(cell->tab);
  free(cell);
}


void OPEN(FILE *fout, struct browser *b, int nr) {
  int ok = 0;
  TListaC cell = b->list.santi->urm;
  while (cell != b->list.santi) {
    if (cell->tab->id == nr) {
      ok = 1;
      break;
    }
    cell = cell->urm;
  }
  if (ok == 0) {
    fprintf(fout, "403 Forbidden\n");
    return;
  } else {
    b->current = cell->tab;
  }
}


void NEXT(struct browser *b) {
  TListaC cell = b->list.santi->urm;
  while (cell->tab != b->current)
    cell = cell->urm;
  if (cell->urm->tab->id == -1)
    b->current = b->list.santi->urm->tab;
  else
    b->current = cell->urm->tab;
}


void PREV(struct browser *b) {
  TListaC cell = b->list.santi->urm;
  while (cell->tab != b->current)
    cell = cell->urm;
  if (cell->pre->tab->id == -1)
    b->current = b->list.santi->pre->tab;
  else
    b->current = cell->pre->tab;
}

// makes an upside down copy of the stack
TStack copySinv(TStack s) {
  if (s == NULL)
    return NULL;

  TStack copyinv = Init_Stack();

  TStack aux = s;
  while (aux) {

    TStack nod = (TStack)malloc(sizeof(TCelStack));
    if (!nod)
      return NULL;
    nod->page = malloc(sizeof(struct page));
    if (!nod->page) {
      free(nod);
      return NULL;
    }
    nod->page->id = aux->page->id;
    strncpy(nod->page->url, aux->page->url, strlen(aux->page->url) + 1);
    nod->page->description = malloc(strlen(aux->page->description) + 1);
    if (!nod->page->description) {
      free(nod->page);
      free(nod);
      return NULL;
    }
    strncpy(nod->page->description, aux->page->description,
            strlen(aux->page->description) + 1);

    nod->urm = copyinv;
    copyinv = nod;

    aux = aux->urm;
  }
  return copyinv;
}

// adds a new cell to the stack
int Push(TStack *vf, struct page *mypage) {
  TStack aux = (TStack)malloc(sizeof(TCelStack));
  if (!aux)
    return 0;
  aux->page = malloc(sizeof(struct page));
  if (!aux->page) {
    free(aux);
    return 0;
  }
  aux->page->id = mypage->id;
  strcpy(aux->page->url, mypage->url);
  aux->page->description = malloc(strlen(mypage->description) + 1);
  if (!aux->page->description) {
    free(aux->page);
    free(aux);
    return 0;
  }
  strcpy(aux->page->description, mypage->description);
  aux->urm = *vf;
  *vf = aux;
  return 1;
}

// extract a cell from the stack
int Pop(TStack *vf) {
  TStack aux;
  if (!vf)
    return 0;
  aux = *vf;
  *vf = aux->urm;
  if (aux->page) {
    if (aux->page->description)
      free(aux->page->description);
    free(aux->page);
  }
  free(aux);
  return 1;
}


int PAGE(FILE *fout, struct browser *b, int id, int nrpages,
         struct page *pages) {
  int i;
  int ok = 0;
  for (i = 1; i <= nrpages; i++) {
    if (pages[i].id == id) {
      ok = 1;
      break;
    }
  }
  if (ok == 0) {
    fprintf(fout, "403 Forbidden\n");
    return 0;
  }
  if (Push(&(b->current->backwardStack), b->current->currentPage) == 0)
    return 0;
  FreeStack(&(b->current->forwardStack));
  b->current->currentPage->id = id;
  strcpy(b->current->currentPage->url, pages[i].url);
  strcpy(b->current->currentPage->description, pages[i].description);
}


void print_stack(FILE *fout, TStack stack) {
  if (stack == NULL)
    return;
  for (; stack != NULL; stack = stack->urm) {
    if (stack->page != NULL) {
      fprintf(fout, "%s\n", stack->page->url);
    }
  }
}


void PRINT_HISTORY(FILE *fout, struct browser *b, int id) {
  TListaC cell = b->list.santi->urm;
  while (cell->tab->id != id && cell != b->list.santi)
    cell = cell->urm;
  if (cell == b->list.santi) {
    fprintf(fout, "403 Forbidden\n");
    return;
  }
  print_stack(fout, copySinv(cell->tab->forwardStack));
  fprintf(fout, "%s\n", cell->tab->currentPage->url);
  print_stack(fout, cell->tab->backwardStack);
}


void BACKWARD(FILE *fout, struct browser *b) {
  if (b->current->backwardStack == NULL) {
    fprintf(fout, "403 Forbidden\n");
    return;
  }
  Push(&(b->current->forwardStack), b->current->currentPage);
  b->current->currentPage->id = b->current->backwardStack->page->id;
  strcpy(b->current->currentPage->url, b->current->backwardStack->page->url);
  strcpy(b->current->currentPage->description,
         b->current->backwardStack->page->description);
  Pop(&(b->current->backwardStack));
}


void FORWARD(FILE *fout, struct browser *b) {
  if (b->current->forwardStack == NULL) {
    fprintf(fout, "403 Forbidden\n");
    return;
  }
  Push(&(b->current->backwardStack), b->current->currentPage);
  b->current->currentPage->id = b->current->forwardStack->page->id;
  strcpy(b->current->currentPage->url, b->current->forwardStack->page->url);
  strcpy(b->current->currentPage->description,
         b->current->forwardStack->page->description);
  Pop(&(b->current->forwardStack));
}
