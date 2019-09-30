/*
    KLEYTON LEITE
    HEADER DE OPERACOES FTP
    QUANDO JA HÁ UM SOCKET PRONTO A SER USADO
*/
#ifndef FTP_H_
#define FTP_H_

#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sys/types.h>
#include <dirent.h>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>//Socket
#include <sys/types.h>//Tipos para socket
#include <arpa/inet.h> //addrstructure

#include "word.h"

#define LS    "LS   "
#define CD    "CD   "
#define GET   "GET  "
#define PUT   "PUT  "
#define MKDIR "MKDIR"
#define CLOSE "CLOSE"

namespace connection
{
    class Ftp : public Word{

        private:
            //Envia uma mensagem
            bool sentData(bool more, std::string cmd, std::string msg);

        protected:
            int sock=0;
            //Verifica se o diretorio é valido
            bool dirValid(std::string caminho);
            //Recebe uma mesagem
            std::string receiveMsg();
            //Envia mensagem completa
            bool sentCompleteData(std::string cmd, std::string msg);
        public:
            Ftp(){}
            Ftp(int socket);
            ~Ftp();
            
            //Verifica os caracteres
            friend bool IsUnexpectedCharacters(char c);
            //Traduz comandos recebidos em mensagens
            void comandos();
            //Retorna lista de arquivos em diretorio
            bool ls(std::string caminho);
            //Muda de diretórioo
            bool cd(std::string caminho);
            //Cria pasta
            bool mkdir(std::string name);
            //Envia arquivo solicitado
            bool get(std::string file);
            //Recebe arquivo solicitado
            bool put();
            //Imprime dados recebidos
            void printReceive();

    };
}

#endif