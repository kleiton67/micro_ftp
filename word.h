/*
    KLEYTON LEITE

    HEADER PARA PROTOCOLO DE COMUNICAÇÃO
*/

#ifndef WORD_H_
#define  WORD_H_
#include <string>
namespace connection
{
    class Word{
        //Estrutura do protocolo
        // Versao(1B)|Comando(5B)|Tamanho(4B)|NextMessage(2)[NM/FM]|Mensagem(1400B)
        protected:
            std::string version;
            std::string command;
            void setVersion(std::string version);
            void setCommand(std::string cmd);
            std::string makeWord(std::string msg, std::string control);
            std::string makeWord(std::string cmd, std::string control, std::string msg);
            bool nextMessage(std::string msg);
        public:
            Word();
            ~Word();
    };
}

#endif