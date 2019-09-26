/*
    KLEYTON LEITE

    IMPLEMENTAÇÃO DO PROTOCOLO DE COMUNICAÇÃO

*/

#include "word.h"

using namespace connection;

Word::Word()
{}

Word::~Word()
{}

void  Word::setVersion(std::string version)
{
    this->version =  version;

}

void Word::setCommand(std::string cmd)
{
    command  = cmd;
}

std::string Word::makeWord(std::string msg, std::string control)
{
    std::string total;
    total.append(version);//0 - byte
    total.insert(1, command);//1 - 5 byte
    total.insert(6, control);//6-7 byte
    total.insert(8, std::to_string(msg.size()));//Mensagem 8 -
    return total;

}

std::string Word::makeWord(std::string cmd, std::string control, std::string msg)
{
    std::string total;
    total.append(version);//0 - byte
    total.insert(1, cmd);//1 - 5 byte
    total.insert(6, control);//6-7 byte
    total.insert(8, std::to_string(msg.size()));//Mensagem 8 -
    return total;

}

bool Word::nextMessage(std::string msg)
{
    return msg.compare(6,7,"NM")==0;
}
