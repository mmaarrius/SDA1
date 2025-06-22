.PHONY: build clean run

build: tema1

tema1:
	gcc -Wall -Wextra  -std=c99 -c main.c browser.c page.c tab.c
	gcc -Wall -Wextra -std=c99 -o tema1 main.o  browser.o page.o tab.o  

run: build
	./tema1 

clean:
	rm -f *.o tema1
