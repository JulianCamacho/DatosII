#include <auxclass.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <iostream>
#define PORT 8080
using namespace std;

/**
 * @brief Clase Servidor.
 *
 */
int main() {
    int server_fd, new_socket, valread; /**< TODO: describe */
    struct sockaddr_in address; /**< TODO: describe */
    int opt = 1; /**< TODO: describe */
    int addrlen = sizeof(address); /**< TODO: describe */
    char buffer[1024] = {0}; /**< TODO: describe */
    char *hello = "Hello from server"; /**< TODO: describe */

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)  {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read( new_socket, buffer, 1024);
    printf("%s\n",buffer );

    string nameFile, source, target;
    int cont = 0;
    for (int i = 0; i < sizeof(buffer); i++){
        if (cont == 0){
            source += buffer[i];
            cont++;
            continue;
        }
        if (cont == 1){
            target += buffer[i];
            cont++;
            continue;
        } else if(cont >= 2) {
            nameFile += buffer[i];
        }

    }
    try{
        int sourceInt;
        int targetInt;
        sourceInt = stoi(source);
        targetInt = stoi(target);
        Graph g(sourceInt, targetInt, nameFile);
        cout << "Grafo creado" << endl;

        char char_array[g.display().size() + 1];
        strcpy(char_array, g.display().c_str());
        send(new_socket , char_array , strlen(char_array) , 0 );
        printf("%s\n",char_array );


    } catch(invalid_argument){}

    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}


