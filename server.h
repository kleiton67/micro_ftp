/*
    KLEYTON LEITE

    HEADER PARA CONSTRUÇÃO DO SERVIDOR

    server.h
*/

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <string>
#include <sys/socket.h>//Socket
#include <sys/types.h>//Tipos para socket
#include <arpa/inet.h> //addrstructure
#include <unistd.h>//Constantes para LINUX
#include <pthread.h>//Bilblioteca para threads
#include <map>//Estrutura de dados map
#include <utility>//pair
#include <cstring>//preecher estrutura

#include "structArg.h"//Estrutura de dados para parametros de 
//funcoes em multithread

namespace connection
{

    /*
        Criação de um servidor utilizando protocolo TCP com IPV4
    */ 
    class ServerT
    {
        private:
            short int port, connections;
            int sListen = 0, opt;
            struct sockaddr_in ad_Server;
            //Parametros para aplicação single Thread
            ParamThread *parametros;
            std::map <int, _Params* > threads;
            //retorna posicao do vetor com thread desativada ou inexistente
            std::map<int, _Params*>::iterator threadDisabled(_Params* );

        public:
            ServerT(std::string address, short int port);
            ~ServerT();
            //Faz o socket de escuta, bind e listen
            bool initServer(int maxConnections);
            //Função para aceitar um cliente por vez
            bool acceptClient(void *function(void *));
            //Função multithreading para aceitar clientes
            bool multiClient(void *function(void*));
    };

} // namespace name

#endif