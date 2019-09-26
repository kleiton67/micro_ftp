/*
    KLEYTON LEITE

    IMPLEMENTAÇÃO DO HEADER SERVER.H
 */

#include "server.h"

using namespace connection;

ServerT::ServerT(std::string address, short int port)
{
    memset(&ad_Server, '0', sizeof(ad_Server));
    ad_Server.sin_addr.s_addr = htonl(INADDR_ANY);
    ad_Server.sin_family = AF_INET;
    this->port = port;
    ad_Server.sin_port = htons(port);
}

ServerT::~ServerT()
{
    //aguadar threads terminarem de executar
    std::map<int, _Params *>::iterator it;
    for(it = threads.begin(); it!=threads.end(); it++) {
       int status = pthread_join(it->second->id,NULL);
       if (status) {
          std::cout << "ERROR; return code from pthread_join() is %d\n" <<
                        status;
          continue;
          }
       }
}

bool ServerT::initServer(int maxConnections)
{
    sListen = socket(AF_INET, SOCK_STREAM, 0); // creating socket
    //configuring options socket0
    if (setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                      &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        return false;
    } 
    bind(sListen, (struct sockaddr*)&ad_Server , sizeof(ad_Server));
	listen(sListen , maxConnections);
    connections = maxConnections;
    return  true;
}

bool ServerT::acceptClient(void *function(void *param))
{
    int cSocket;
    parametros = new _Params;
    cSocket = accept(sListen, (struct sockaddr*)NULL, NULL);
    parametros->socketClient = cSocket;
    function(parametros);
    //std:: cout << parametros->msg;
    return true;
}

bool ServerT::multiClient(void *function(void*))
{

    std::map<int, _Params*>::iterator it;
    parametros = new _Params;
    it = threadDisabled(parametros);
    if(threads.size() >= connections)
    {
        if(it == threads.end())
        {
            std::cout << "Server crowded!!!";
            return false;
        }

    }

    int cSocket;
    
    pthread_t idThread;

    cSocket = accept(sListen, (struct sockaddr*)NULL, NULL);
    parametros->active = true;
    parametros->socketClient = cSocket;

    int status = pthread_create(&idThread, NULL, 
        function, (void* )parametros);  
    if (status) {
        std::cout <<
            "ERROR; return code from pthread_create() is %d\n" <<
                    status;
        return false;
    }
    
    parametros->id = idThread;

    return true;
}

std::map<int, _Params* >::iterator ServerT::threadDisabled(_Params* param)
{
    std::map<int, _Params*>::iterator it = threads.end();

    if(threads.size() < connections)
    {
        threads.insert(it, std::pair<int,_Params* >(threads.size(), param) );
        return it;
    }
    else
    {
        for(it = threads.begin() ; it!=threads.end(); it++)
        {
            if(!it->second->active)
            {
                delete it->second;
                return it;
            }
        }
    }

    delete param;
    
}