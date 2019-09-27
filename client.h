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

        public:
            //Endereco do servidor e porta a ser usada
            Client(std::string address, short int port);
            ~Client();
            //Cria socket com o ip do servidor
            bool connectServer(std::string addressServer);
            //Inicia um loop para receber comandos do usuário
            bool iniciaComunicacao();
            //Imprime cabeçalho
            void imprimeCabecalho();
            //Barra de carga
            void barraConclusao();
    };

}

#endif