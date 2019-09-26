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

using namespace connection;

void *serveCliente(void* param)
{
    
    return (void*)NULL;
}

int main()
{
    ServerT servidor("127.0.0.1", 20100);

    servidor.initServer(20);

    while(true)
    {
        servidor.multiClient(serveCliente);
    }
 
    return 0;
}


