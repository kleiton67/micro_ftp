/*
    KLEYTON LEITE

    IMPLEMENTAÇÃO DO PROTOCOLO DE COMUNICAÇÃO

*/

#include "word.h"

using namespace connection;

Word::Word()
{
    version = "1";
}

Word::~Word()
{}

void  Word::setVersion(std::string version)
{
    this->version =  version;

}

std::string Word::makeWord(std::string cmd, std::string control, 
                                            std::string msg)
{
    
    std::string total;
    total.append(version);//0 - byte
    total.append(cmd);//1 - 5 byte
    total.append(setTamanho(msg.size()));//6-9
    total.append(control);//10-11 byte
    total.append(msg);//Mensagem 12 -
    return total;

}

bool Word::nextMessage(std::string msg)
{
    return msg.compare(6,7,"NM")==0;
}

std::string Word::getCommand(std::string msg)
{
    std::string aux("     ");
    if(msg.size() < 5)
    {
        std::cout << "Mensagem vazia!!! - getCommand\n";
    }
    else
    {
        aux = msg.substr(1,5);
    }
    
    return aux;
}

std::string Word::getData(std::string msg)
{
    if(msg.size() > 12){
        int tamanho = 0;
        tamanho = msg[6]*1000+msg[7]*100+msg[8]*10+msg[9];
        return msg.substr(12, tamanho);
    }
    else
        return " ";
}

int Word::getTamanho(std::string msg)
{
    return msg[6]*1000+msg[7]*100+msg[8]*10+msg[9];
}

std::string Word::setTamanho(int tam)
{
    char aux[5];
    sprintf(aux, "%.4d", tam);
    std::string stg;
    stg.append(aux);
    //retira o \0;
    return stg;
}