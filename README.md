This project helped me better understand concepts such as doubly linked lists and stacks, and improved my ability to work with them and dynamic memory. 

The project description can be found [here](SDA___Tema_1_2025.pdf)  

For running:   

    ./checker.sh


## My Approach:
- I organized the code into 4 .c files and 4 .h files.

1. **[page.c](./page.c)** contains functions that manage the array of pages (initialize, add page, find page, destroy array).
2. **[tab.c](./tab.c)** manages the tab structure (initialize tab, destroy tab), including functions to manipulate the stacks of pages (initialize stack, push, pop, empty stack, destroy).
3. **[browser.c](./browser.c)** contains functions for managing the doubly linked list of tabs.
4. **[main.c](./main.c)** contains the implementation of all commands.

- After inserting the pages into the array, the browser list is initialized with the default tab, which contains the default page. The stacks are then created, and the "game" can begin.
- Each command has its own function with a meaningful name.

**Notes**

Since I used a stack-like structure that doesn't keep track of the rear, in order to display the history from the forward stack, I used a recursive function that traverses the stack and then lists the pages.

If I could go back in time, I wouldn’t open `tema1.out` every time I access it. Instead, I would pass the pointer through the functions.

Initially, I assumed that the page ID would also represent the index in the array, allowing page lookup in constant time (O(1)), so I stored the pages in an array of structures. However, I would now switch to a simple linked list instead.

- I get **120/120** points on my pc.


