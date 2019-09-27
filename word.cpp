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
    total.insert(6,4,'0');
    total.insert(6,std::to_string(msg.size()));
    total.insert(10, control);//10-11 byte
    total.insert(12, std::to_string(msg.size()));//Mensagem 12 -
    return total;

}

std::string Word::makeWord(std::string cmd, std::string control, std::string msg)
{
    std::string total;
    total.append(version);//0 - byte
    total.insert(1, cmd);//1 - 5 byte
    total.insert(6,setTamanho(msg.size()));//6-9
    total.insert(10, control);//10-11 byte
    total.insert(12, std::to_string(msg.size()));//Mensagem 12 -
    return total;

}

bool Word::nextMessage(std::string msg)
{
    return msg.compare(6,7,"NM")==0;
}

std::string Word::getCommand(std::string msg)
{
    std::string aux("     ");
    aux = msg.substr(1,5);
    return aux;
}

std::string Word::getData(std::string msg)
{
    int tamanho = 0;
    tamanho = msg[6]*1000+msg[7]*100+msg[8]*10+msg[9];
    return msg.substr(12, tamanho);
}

int Word::getTamanho(std::string msg)
{
    return msg[6]*1000+msg[7]*100+msg[8]*10+msg[9];
}

std::string Word::setTamanho(int tam)
{
    char aux[5];
    sprintf(aux, "%4d", tam);
    std::string stg;
    stg.append(aux);
    //retira o \0;
    stg.pop_back();
    return stg;
}