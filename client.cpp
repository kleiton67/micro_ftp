/*

    KLEYTON LEITE

    IMPLEMENTAÇÃO DO CABEÇALHO CLIENT.H

*/

#include "client.h"

using namespace connection;

Client::Client(std::string addres, short int port)
{
    memset(&confServer, '0', sizeof(confServer));
    confServer.sin_addr.s_addr = htonl(INADDR_ANY);
    confServer.sin_family = AF_INET;
    this->port = port;
    confServer.sin_port = htons(port);
}

bool Client::connectServer(std::string addressServer)
{
    if((CreateSocket = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("Socket not created \n");
        return false;   
    }
 
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_port = htons(2018);
    ipOfServer.sin_addr.s_addr = inet_addr(address.c_str());
    if(connect(CreateSocket, (struct sockaddr *)&ipOfServer,
                             sizeof(ipOfServer))<0)
    {
        printf("Connection failed due to port and ip problems\n");
        return false;
    }

    return true;
}

bool Client::iniciaComunicacao()
{
    /*
        Estrutura da interface com usuário:
        <Endereco.ip>
    */

}

void Client::imprimeCabecalho()
{

}
