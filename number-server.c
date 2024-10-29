#include "http-server.h"
#include <string.h>

int num = 0;
char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
char const NOT_FOUND_MESSAGE[] = "Uh oh, someone made a typo...";

char const HTTP_200_OK[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator

    // send response
    write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
    write(client_sock, NOT_FOUND_MESSAGE, strlen(NOT_FOUND_MESSAGE));
}

void handle_shownum(int client_sock) {
    write(client_sock, HTTP_200_OK, strlen(HTTP_200_OK));

    char res[50];
    snprintf(res, 50, "Your number is %d\r\n", num);

    write(client_sock, res, strlen(res));
}

void handle_increment(int client_sock) {
    num++;
    handle_shownum(client_sock);
}

void handle_add(int client_sock, char *path) {
   int value;
   
   int result = sscanf(path, "/add?value=%d", &value); 

   printf("Value added: %d\n", value);
   num += value;

   handle_shownum(client_sock);
}

void handle_response(char *request, int client_sock) {
    char path[256];

    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) {
        printf("Invalid request line\n");
        return;
    }

    if (strcmp(path, "/shownum") == 0) {
        handle_shownum(client_sock);
    } else if (strcmp(path, "/increment") == 0) {
        handle_increment(client_sock);
    } else if (strstr(path, "/add?value=") != NULL) {
       handle_add(client_sock, path); 
    }else {
        handle_404(client_sock, path);
    } 
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }

    start_server(&handle_response, port);
}
