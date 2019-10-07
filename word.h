/*
    KLEYTON LEITE

    HEADER PARA PROTOCOLO DE COMUNICAÇÃO
*/

#ifndef WORD_H_
#define  WORD_H_
#include <string>
#include <vector>
#include <cstring>
#include <iostream>

//tamanho do campo mensagem
#define TAM_DATA 1400
#define TAM_CAB 12
#define caractereDep ' '

namespace connection
{
    class Word{
        //Estrutura do protocolo
        // Versao(1B)|Comando(5B)|Tamanho(4B)|NextMessage(2)[NM/FM]|Mensagem(1400B)
        protected:
            std::string version;
            std::string command;
            void setVersion(std::string version);
            //Faz palavra com string
            std::string makeWord(std::string cmd, std::string control, std::string msg);
            //Faz palavra com char
            void makeWordchar(char* word, const char* cmd, const char* control, const char* msg, 
                int bytes);
            //Verifica se há mensagens a serem a entregues
            bool nextMessage(std::string msg);
            //Verifica se ha mensagem
            bool nextMessage(char * msg);
            //Obtem comando da palavra
            std::string getCommand(std::string msg);
            //Obtem somente mensagem
            std::string getData(std::string msg);
            //Obtem somente mensagem 
            void getData(char* msg, char * data);
            //Obtem tamanho da mensagem
            int getTamanho(std::string msg);
            //Prepara tamanho da palavra
            std::string setTamanho(int tam);
            //Retorna ponteiro de char para tamanho
            char * setTamanho(short tam);
            //Retorna o tamanho da palavra
            int getTamanho(char* msg);
            //Imprime vetor
            void print(char * vetor, int tam);
        public:
            Word();
            ~Word();
    };
}

#endif
