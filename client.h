/*i
    KLEYTON LEITE
    HEADER  PARA CLIENTE FTP
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "ftp.h"

namespace connection{

    class Client : public Ftp{
        private:
            std::string address;
            short int port;
            struct sockaddr_in confServer;
            //Contem caminho da pasta atual

        public:
            Client(){};
            //Endereco do servidor e porta a ser usada
            Client(std::string address, short int port);
            Client(short int port);
            ~Client();
            //Cria socket com o ip do servidor
            bool connectServer(std::string addressServer);
            //Inicia um loop para receber comandos do usuário
            bool comunicacaoServer();
            //Imprime cabeçalho
            void imprimeCabecalho();
            //Barra de carga
            void barraConclusao();
            //Split in cpp
            std::vector<std::string> tokenize(std::string const &str,
                                                    const char delim);
            /*
                Recebe mensagem do usuario e traduz para o envio 
                ao servidor
            */
            bool comandoRequerido(std::string mensagem);
            //Mostra a mensagem de comando incompleto
            void msgIncomplete();
            void imprimeFile(std::string file);
    };

}

#endif