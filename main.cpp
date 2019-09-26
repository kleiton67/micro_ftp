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

using namespace connection;

char * hora_Vetor(int tam)
{
    char * info;
    info = new char[tam];

    
    char *hora = __TIME__;

    info[0] = '1';
    info[1] = 'H';
    info[2] = '8';

    for(int i =3, j = 0; i < tam || j < sizeof(hora); i++, j++)
    {   
        info[i] = hora[j];
    }   
    return info;
}

char * data_Vetor(int tam)
{
    char * info;
    info = new char[tam];

    
    char *data = __DATE__;

    info[0] = '1';
    info[1] = 'D';
    info[2] = '8';

    for(int i =3, j = 0; i < tam || j < sizeof(data); i++, j++)
    {   
        info[i] = data[j];
    }   
    return info;
}

char * all_Vetor(int tam)
{
    char * info;
    info = new char[tam];

    char *all = new char[12];

    strcpy(all, __TIME__);

    info[0] = '1';
    info[1] = 'A';
    info[2] = '8';

    int i = 3;

    for(int j = 0; i < tam && j < sizeof(all)-3; i++, j++)
    {   
        info[i] = all[j];
    } 

    memset(all, ' ' ,sizeof(all));
    strcpy(all, __DATE__);

    info[i++] = '-';

    for(int j = 0; i < tam && j < sizeof(all); i++, j++)
    {   
        info[i] = all[j];
    }  
    return info;
}

void *enviaHora(void* param)
{
    ParamThread *aux;
    aux = (_Params*)param;
    char *data, dataReceived[20];

    memset(dataReceived, '0' ,sizeof(dataReceived));

    //Receber dados do cliente

    read(aux->socketClient, dataReceived, sizeof(dataReceived)-1);

    if(dataReceived[1] == 'A')
    {
        data = all_Vetor(20);
        /*close(aux->socketClient);
        return (void*)NULL;
        */
    }
    else if(dataReceived[1] == 'H')
        data = hora_Vetor(20);
    else if(dataReceived[1] == 'D')
        data = data_Vetor(20);
    else
    {
        close(aux->socketClient);
        return (void*)NULL;
    }
    //enviar dados para cliente
    write(aux->socketClient, data, strlen(data));
    
    //fecha socket
    close(aux->socketClient);

    aux->msg = "OK";
    aux->active = false;

    //delete data;    

    return (void*)NULL;
}

int main()
{
    ServerT servidor("127.0.0.1", 2018);

    servidor.initServer(20);

    while(true)
    {
        servidor.multiClient(enviaHora);
    }
 
    return 0;
}


