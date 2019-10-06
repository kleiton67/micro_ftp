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
#include <fstream> //Manipulação de arquivos
#include <cstdlib> //system
#include <fcntl.h> // open
 

#include "word.h"

#define LS    "LS   "
#define CD    "CD   "
#define GET   "GET  "
#define PUT   "PUT  "
#define MKDIR "MKDIR"
#define CLOSE "CLOSE"
#define ERRO  "ERROR"

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
            //Recebe a mensagem completa
            std::string receiveAllMsg();
            //Recebe e grava em um arquivo com o nome especificado
            bool receiveMsgRecordFile(std::string caminho);
            //Envia mensagem em binario a partir de um arquivo
            bool sentFile(std::string caminho, std::string cmd);
            //Recebimento de arquivos Binarios
            bool receiveBinRecordFile(std::string caminho);
            //Envio de arquivos binarios
            bool sentFileBinArq(std::string caminho, std::string cmd);
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
            //Muda de diretório
            bool cd(std::string caminho);
            //Cria pasta
            bool mkdir(std::string name);
            //Envia arquivo solicitado
            bool get(std::string file);
            //Recebe arquivo solicitado
            bool put(std::string file);
            //Imprime dados recebidos
            void printReceive();

    };
}

#endif