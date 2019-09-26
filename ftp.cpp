/*
    KLEYTON LEITE

    IMPLEMENTAÇÃO DO HEADER FTP.H

    NESSA IMPLEMENTAÇÃO A CLASSE 
    FICA RESPONSAVEL TANTO PELA
    CONEXAO DO CLIENTE AO SERVIDOR
    QUANTO VICE E VERSA.

    FTP RECEBE UM SOCKET PRONTO
    E SE ENCARREGA DE ENCERRAR 
    A CONEXAO
*/

#include "ftp.h"

using namespace connection;

Ftp::Ftp(int socket)
{
    this->socket = socket;
    setVersion("1");
}

Ftp::~Ftp()
{
}

void Ftp::comandos()
{
    while (true)
    {
        std::string msg;
        msg = receiveData();
        std::string aux;
        aux = getCommand(msg);
        std::transform(aux.begin(), aux.end(), aux.begin(), ::toupper);
        if(aux.compare(LS))
        {
            if(getTamanho(msg) > 0)
                ls();
            else
            {
                std::string caminho;
                caminho = getData(msg);
                ls(caminho);
            }
            
        }
        else if(aux.compare(CD))
        {

        }
        else if(aux.compare(GET))
        {

        }
        else if(aux.compare(PUT))
        {
            
        }
        else if(aux.compare(MKDIR))
        {
            
        }
        else if(aux.compare(CLOSE))
        {
            
        }
    }
}

std::string Ftp::receiveData()
{
    char data[1400];
    std::string msg;
    msg.append(data);
    read(socket, data, sizeof(data));
    msg.insert(0, data);

    return msg;

}

bool Ftp::sentData(std::string cmd, std::string msg)
{
    std::string send;
    //Enviar mensagens parte por parte
    while(true)
    {
        //Envio da mensagem pelo socket
        char data[1400];
        memset(data, ' ',sizeof(data));
        //Couber em apenas uma mensagem
        if(msg.size() <= 1388){
            //Prepara a palavra
            send = makeWord(cmd, "FM",msg);
            strncpy(data, msg.c_str(), sizeof(data));

            if(write(socket, data, sizeof(data))==-1)
                return false;
            else
            {
                //Tranferência concluída
                break;
            }
        }
        else
        {
            std::string aux;
            aux = msg;
            std::string temp;
            while(true)
            {
                memset(data, ' ',sizeof(data));
                //Envio da primeira parte primeiro
                if(aux.size()>TAM_DATA){
                    temp.insert(0, aux,0, TAM_DATA);
                    aux.erase(0,TAM_DATA);
                    send = makeWord(cmd, "NW", temp);
                    strncpy(data, send.c_str(), sizeof(data));

                    if(write(socket, data, sizeof(data))==-1)
                        return false;
                }
                else
                {
                   temp.insert(0,aux, 0,aux.size());
                   send = makeWord(cmd, "FM", temp);
                   strncpy(data, send.c_str(), sizeof(data));

                   if(write(socket, data, sizeof(data))==-1)
                        return false;

                    break;
                }
            }
        }
    }
    
    return true;

}

bool Ftp::ls(std::string caminho = ".")
{
    std::vector<std::string > v;
    DIR* dirp = opendir(caminho.c_str());
    if (dirp == NULL) {
            printf ("Cannot open directory '%s'\n", v[1]);
            return false;
        }
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);

    //Convertendo o vetor de strings em string
    std::stringstream ss;
    for(size_t i = 0; i < v.size(); ++i)
    {
    if(i != 0)
        ss << "\n";
    ss << v[i];
    }
    std::string s = ss.str();

    //Enviar dados
    sentData("ls", s);

    return true;
}

bool Ftp::cd(std::string name)
{

}

bool Ftp::mkdir(std::string name)
{

}

bool Ftp::get(std::string file)
{

}

bool Ftp::put()
{

}