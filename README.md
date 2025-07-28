This program simulates the behavior of a modern web browser with multiple tabs and navigation history, using custom data structures in C. It manages Web Pages, Tabs and Browser.

=============================================================================================================================================================================

Input Format:
The first line contains the number of available web pages

For each page, there are 3 lines:
Page ID (not 0)
Page URL (max 50 characters)
Page description (can be of any length)

One line with N, the number of commands

Followed by N lines, each containing one browser operation



Input example:
3
1
https://www.stanford.edu/
A Mission Defined by Possibility
2
https://www.ox.ac.uk/
University of Oxford
3
https://www.harvard.edu/
Harvard University
3
PAGE 3
PAGE 2
PRINT_HISTORY 0

=============================================================================================================================================================================

Data is read from file.in and output is written to file.out.


To test the implementationrun in the terminal the following commands:
make
./file

=============================================================================================================================================================================

The browser supports the following commands:

NEW_TAB – Creates a new tab initialized with a default page;

CLOSE – Closes the current tab (except tab 0);

OPEN <ID> – Switches to the tab with the specified ID;

NEXT / PREV – Cycles through tabs in circular order;

PAGE <ID> – Opens a new page in the current tab, pushing the current one to the BACKWARD stack and clearing FORWARD;

BACKWARD / FORWARD – Navigates through page history using the respective stack;

PRINT – Displays tab IDs starting from the current one, and the current page's description;

PRINT_HISTORY <ID> – Shows the full history of a given tab (FORWARD, current, BACKWARD), ordered correctly;



Technologies & Concepts Used: C programming language, Dynamic Memory Allocation, Modular Programming with Functions, Circular Doubly Linked Lists, Stacks, Input Parsing and Command Execution, Input Validation and Error Handling, File I/O.

=============================================================================================================================================================================

DOCUMENTATION

MAIN FUNCTIONALITY:
After opening the input (fin) and output (fout) files, we read the data from tema1.in.
Each line is processed to remove special characters.

The page data is stored in an array of type struct page, allowing us to access it later.

Next, we read the number of operations, initialize the browser (with the default tab and default page), and then process the operations one by one.
Each operation is executed as it is read. For every operation, we have implemented a dedicated function (found in fct.c), along with other necessary utility functions.

At the end, we free all allocated memory for the browser (using FreeBrowser) and the array of pages.

OPERATIONS:
NEW_TAB
A new tab is added to the list. If the insertion (via InsCellList) is successful, the new tab becomes the current tab.

CLOSE
After verifying that the current tab is not tab with ID 0, we locate the cell containing the current tab in the list. Once found, the current tab is updated to the previous one.
The list links are updated to remove the current cell, and the memory for that tab is freed (using free and FreeStack).

OPEN <ID>
We traverse the tab list to check if the tab with the specified ID exists. If it doesn't, an error message is displayed.
If it does exist, b->current (the current tab) is updated to that tab.

NEXT
We traverse the list to reach the current tab. If it is the last tab before the sentinel node, the current tab becomes the tab after the sentinel.

PREV
Similar to NEXT, but in reverse. If the current tab is the first after the sentinel, after this operation, the current tab becomes the last one before the sentinel.

PAGE <ID>
We search the page vector to verify that the page with the given ID exists.
If the Push operation succeeds, it means the current page was pushed to the backward stack.
We then clear and free the forward stack (using FreeStack), and the current page is updated to the new page, taking its ID, URL, and description.

BACKWARD
We check if the backward stack is not empty. If it isn't, we push the current page onto the forward stack.
Then, the top of the backward stack becomes the new current page (copied), and it is removed from the stack (with Pop).

FORWARD
Same logic as BACKWARD, but in reverse: push current page onto backward stack and pop from forward.

PRINT
We traverse the list to find the current tab. We display its ID, then iterate through the circular list to print all other tab IDs, skipping the sentinel.
On a new line, we print the description of the current tab's page.

PRINT_HISTORY <ID>
We first verify if the tab with the specified ID exists.
If so, we print the URLs from the forward stack (in reversed order using copySinv), followed by the URL of the current page, and then the URLs from the backward stack (from top to bottom).
The printing is done using the print_stack function, which iterates the stack from top to bottom and prints the page URLs.
