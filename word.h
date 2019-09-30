/*
    KLEYTON LEITE

    HEADER PARA PROTOCOLO DE COMUNICAÇÃO
*/

#ifndef WORD_H_
#define  WORD_H_
#include <string>
#include <iostream>

//tamanho do campo mensagem
#define TAM_DATA 1400

namespace connection
{
    class Word{
        //Estrutura do protocolo
        // Versao(1B)|Comando(5B)|Tamanho(4B)|NextMessage(2)[NM/FM]|Mensagem(1400B)
        protected:
            std::string version;
            std::string command;
            void setVersion(std::string version);
            //Faz palavra
            std::string makeWord(std::string cmd, std::string control, std::string msg);
            //Verifica se há mensagens a serem a entregues
            bool nextMessage(std::string msg);
            //Obtem comando da palavra
            std::string getCommand(std::string msg);
            //Obtem somente mensagem
            std::string getData(std::string msg);
            //Obtem tamanho da mensagem
            int getTamanho(std::string msg);
            //Prepara tamanho da palavra
            std::string setTamanho(int tam);
        public:
            Word();
            ~Word();
    };
}

#endif