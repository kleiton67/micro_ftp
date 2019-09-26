/*
    KLEYTON LEITE
    HEADER DE OPERACOES FTP
    QUANDO JA HÁ UM SOCKET PRONTO A SER USADO
*/
#ifndef FTP_H_
#define FTP_H_

#include <string>
#include "word.h"
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

namespace connection
{
    class Ftp : public Word{

        private:
            int socket;
            //Verifica se o diretorio é valido
            bool dirValid(std::string caminho);
            //Recebe mesagem
            std::string receiveData();
            //Envia mensagem
            bool sentData(std::string cmd, std::string msg);
            //Abre arquivo e inicia envio
            //bool sentDataFile();
        public:

            Ftp(int socket);
            ~Ftp();

            //Traduz comandos
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

    };
}

#endif