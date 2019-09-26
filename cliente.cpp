
/*
    KLEYTON LEITE

    CONTRUCAO DO LADO CLIENTE PARA APLICAÇÃO DE DATA E HORA

    Fonte: https://www.thecrazyprogrammer.com/2017/06/socket-programming.html

    cliente.cpp
*/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
 
using namespace std;

int main(int argc, char *argv[])
{
    int CreateSocket = 0,n = 0;
    char dataReceived[1024];
    struct sockaddr_in ipOfServer;
 
    memset(dataReceived, '0' ,sizeof(dataReceived));

    if(argc < 3)
    {
        cout << "Comandos invalidos";
    }
    string address;

    address = argv[1];

    char command = argv[2][0];
 
    if((CreateSocket = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("Socket not created \n");
        return 1;   
    }
 
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_port = htons(2018);
    ipOfServer.sin_addr.s_addr = inet_addr(address.c_str());
    if(connect(CreateSocket, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer))<0)
    {
        printf("Connection failed due to port and ip problems\n");
        return 1;
    }

    char send[20];

    memset(send, ' ', sizeof(send));
    send[0] = '1';
    send[1] = command;

    //Dados a serem enviados ao servidor

    write(CreateSocket, send, sizeof(send));

    while((n = read(CreateSocket, dataReceived, sizeof(dataReceived)-1)) > 0)
    {
        dataReceived[n] = 0;
        //fputs(dataReceived, stdout);
        printf("\n");
    }

    char * info;
    info = new char[21];

    int j = 0;

    for(int i =3    ; j < 20 && i < sizeof(dataReceived); i++, j++)
    {   
        info[j] = dataReceived[i];
    }   

    info[j+1] = '\0';
    
    printf("Resposta do servidor: %s.\n", info);
 
    return 0;
}