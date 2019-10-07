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
    total.append(setTamanho((int)msg.size()));//6-9
    total.append(control);//10-11 byte
    total.append(msg);//Mensagem 12 -
    return total;

}

void Word::makeWordchar(char* word, const char* cmd, const char* control, const char* msg, 
                int bytes)
{
    //Tamanho apenas do dado
    memset(word, caractereDep, bytes);
    char * tam;
    tam = setTamanho((short)bytes);
    word[0] = '1';
    word[1] = cmd[0];
    word[2] = cmd[1];
    word[3] = cmd[2];
    word[4] = cmd[3];
    word[5] = cmd[4];
    word[6] = tam[0];
    word[7] = tam[1];
    word[8] = tam[2];
    word[9] = tam[3];
    word[10] = control[0];
    word[11] = control[1];
    for(int i = TAM_CAB; i<bytes+TAM_CAB; i++)
    {
       word[i] = msg[i-TAM_CAB];
    }
}


bool Word::nextMessage(std::string msg)
{
    //std::cout << "nextMessage\n";
    if (msg.compare(10,2,"NM")==0)
    {
        //std::cout << "nextMessage: Há mensagem!!!\n";
        return true;
    }
    if (msg.compare(10,2,"FM") == 0)
    {
        //std::cout << "nextMessage: Não Há mensagem!!!\n";        
        return false;
    }
    else
    {
        return false;
    }
}

bool Word::nextMessage(char * msg)
{
    if(msg[10]=='N' && msg[11] == 'M'){
        //std::cout << "NM: Ha mais mensagens!!!\n";
        return true;
    }
    else
    {
        //std::cout << "NM: Nao Ha mais mensagens!!!\n";
        return false;
    }
    
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
        tamanho = getTamanho(msg);
        return msg.substr(12, tamanho);
    }
    else
        return " ";
}

void Word::getData(char* msg, char * word)
{

    int tamanho = getTamanho(msg);
    for(int i = 0;i<tamanho; i++)
    {
            word[i] = msg[i+TAM_CAB];
    }
}

int Word::getTamanho(std::string msg)
{
    return msg[6]*1000+msg[7]*100+msg[8]*10+msg[9];
}

int Word::getTamanho(char* msg)
{
    int tamanho;
    tamanho = ((int)msg[6]-48)*1000+((int)msg[7]-48)*100+((int)msg[8]-48)*10+((int)msg[9]-48);
    return tamanho;
}

std::string Word::setTamanho(int tam)
{
    char aux[5];
    sprintf(aux, "%.4d", tam);
    std::string stg;
    stg.append(aux);
    return stg;
}

char * Word::setTamanho(short tam)
{
    char *aux = new char[5];
    sprintf(aux, "%.4d", tam);
    return aux;
}

void Word::print(char *vetor, int tam)
{
    printf("Impressao do vetor: \n");
    for (int i = 0 ; i < tam; i++)
    {
        printf("%c\n", vetor[i]);
    }
    printf("Fim da impressao do vetor\n");
}
