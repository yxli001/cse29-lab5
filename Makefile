all:
	make number-server
	make start-server

start-server:
	./server 8000

number-server:
	gcc -std=c11 -Wall -Wno-unused-variable -fsanitize=address -g number-server.c http-server.c -o server

clean:
	rm server
