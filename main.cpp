/*
    KLEYTON LEITE

    CONSTRUCAO DE SERVIDOR COM PTHREADS EM C++

    main.cpp
*/

#include <iostream>
#include <cstring>
#include <fstream>
#include "server.h"
#include <time.h> //manipulação de data
#include "ftp.h"

#define PORTCMT  20000

using namespace connection;

void *serveCliente(void* param)
{
    ParamThread * parametros = new _Params;
    parametros = (_Params*) param;

    Ftp serverFtp(parametros->socketClient);
    serverFtp.comandos();
    return (void*)NULL;
}

int main()
{
    ServerT servidor("127.0.0.1", PORTCMT);

    servidor.initServer(20);

    while(true)
    {
        servidor.multiClient(serveCliente);
    }
 
    return 0;
}


