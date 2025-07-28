/* "Copyright [2025] <CRETU CRISTINA>"  [legal/copyright] [5] */
#include "struct_fct.h"

int main() {
  FILE *fin, *fout;

  // input file
  fin = fopen("file.in", "r");
  if (fin == NULL) {
    return 1;
  }

  // output file
  fout = fopen("file.out", "w");
  if (fout == NULL) {
    fclose(fin);
    return 1;
  }

  char line[100];
  struct page
      remember_pages[50];  // array that keeps the data for the pages
  // nr of pages
  fgets(line, sizeof(line), fin);
  transform_line(line);
  int nrpages = atoi(line);

  // starting page
  remember_pages[0].description = (char *)malloc(100 * sizeof(char));
  if (remember_pages[0].description == NULL) {
    fclose(fin);
    fclose(fout);
    return 1;
  }
  remember_pages[0].id = 0;
  strcpy(remember_pages[0].url, "https://acs.pub.ro/");
  strcpy(remember_pages[0].description, "Computer Science");

  // getting the data for the following used pages
  for (int i = 1; i <= nrpages; i++) {

    // ID
    fgets(line, sizeof(line), fin);
    transform_line(line);
    remember_pages[i].id = atoi(line);

    // URL
    fgets(line, sizeof(line), fin);
    transform_line(line);
    strcpy(remember_pages[i].url, line);

    // DESCRIPTION
    fgets(line, sizeof(line), fin);
    transform_line(line);
    remember_pages[i].description = (char *)malloc(100 * sizeof(char));
    if (remember_pages[i].description == NULL) {
      fclose(fin);
      fclose(fout);
      return 1;
    }
    strcpy(remember_pages[i].description, line);
  }

  // nr of operations
  fgets(line, sizeof(line), fin);
  transform_line(line);
  int nrop = atoi(line);

  int last_id = 0;
  char operation[50];

  struct browser *browser = InitBrowser(); // initialize the browser

  // getting the operations
  for (int i = 1; i <= nrop; i++) {
    fgets(operation, sizeof(operation), fin);
    transform_line(operation);

    if (strcmp(operation, "NEW_TAB") == 0) {
      NEW_TAB(browser, ++last_id);

    } else if (strcmp(operation, "PRINT") == 0) {
      PRINT(fout, browser);

    } else if (strcmp(operation, "CLOSE") == 0) {
      CLOSE(fout, browser);

    } else if (strncmp(operation, "OPEN", 4) == 0) {
      int x = atoi(operation + 5);
      OPEN(fout, browser, x);

    } else if (strcmp(operation, "NEXT") == 0) {
      NEXT(browser);

    } else if (strcmp(operation, "PREV") == 0) {
      PREV(browser);

    } else if (strncmp(operation, "PAGE", 4) == 0) {
      int x = atoi(operation + 5);

      PAGE(fout, browser, x, nrpages, remember_pages);
    } else if (strncmp(operation, "PRINT_HISTORY", 13) == 0) {
      int x = atoi(operation + 14);
      PRINT_HISTORY(fout, browser, x);

    } else if (strcmp(operation, "BACKWARD") == 0) {
      BACKWARD(fout, browser);

    } else if (strcmp(operation, "FORWARD") == 0) {
      FORWARD(fout, browser);
    }
  }


  for (int i = 0; i <= nrpages; i++) {
    free(remember_pages[i].description);
  }
  FreeBrowser(browser);
  fclose(fin);
  fclose(fout);
  return 0;
}
