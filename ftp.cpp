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
    this->sock = socket;
    setVersion("1");
}

Ftp::~Ftp()
{
	close(sock);
}

bool IsUnexpectedCharacters(char c)
{
    switch(c)
    {
    case '(':
    case ')':
    case '-':
    case ' ': 
        return true;
    default:
        return false;
    }
}

void Ftp::comandos()
{
    while (true)
    {
        std::string msg;
		std::cout<< "A receber mensagem\n";
		msg = receiveMsg();
        std::cout << "Mensagem : " << msg << "\n";
        std::string aux;
        aux = getCommand(msg);
        std::transform(aux.begin(), aux.end(), aux.begin(), ::toupper);
		aux.erase(std::remove_if(aux.begin(), aux.end(),
						 &IsUnexpectedCharacters), aux.end());
        if(aux.compare("LS")==0)
        {
			std::cout << "Comando LS\n";
            if(getTamanho(msg) > 0)
                ls(".");
            else
            {
                std::string caminho;
                caminho = getData(msg);
                ls(caminho);
            }
            
        }
        else if(aux.compare("CD") == 0)
        {

        }
        else if(aux.compare("GET") == 0)
        {

        }
        else if(aux.compare("PUT") == 0)
        {
            
        }
        else if(aux.compare("MKDIR") == 0)
        {
            
        }
        else if(aux.compare("CLOSE") == 0)
        {
			std::cout << "Comando CLOSE";
            close(sock);
			return;
        }
		else {
			
        	std::cout << "Comando : "<< aux << ".\n";
			std::cout << "Comando desconhecio!!!\n";
		}
    }
}

std::string Ftp::receiveMsg()
{
    /*
        Receber apenas dados
    */
   char data[TAM_DATA];
   std::string msg;
   read(sock, data, sizeof(data));
   msg.append(data);
   return msg;
}

bool Ftp::sentData(bool more, std::string cmd, std::string msg)
{
    std::string sentMsg;
    char data[TAM_DATA];
    if(msg.size() < TAM_DATA){
        if(more)
        {
            sentMsg = makeWord(cmd, "NM", msg);
        }
        else
        {
            sentMsg = makeWord(cmd, "FM", msg);
        }
    }
    else
    {
        return false;
    }
    
    memset(data, ' ', sizeof(data));
    strcpy(data, sentMsg.c_str());
    write(sock, data, sizeof(data));

    return true;
}

bool Ftp::sentCompleteData(std::string cmd, std::string msg)
{
    if(msg.size() <= TAM_DATA)
    {
        sentData(false, cmd, msg);
    }
    else
    {
        //Se a mensagem for maior que a q cabe
        //no campo, dividir ela
        int i = 0;
        for(; ;i++)
        {
            if(i*TAM_DATA > msg.size())
				break;
           sentData(true, cmd, msg.substr(i*TAM_DATA, TAM_DATA));
        }
		sentData(false, cmd, msg.substr(i*TAM_DATA, 
								msg.size() - i*TAM_DATA));
    }

    return true;
}

bool Ftp::ls(std::string caminho = ".")
{
    std::vector<std::string > v;
    DIR* dirp = opendir(caminho.c_str());
    if (dirp == NULL) {
            printf ("Error LS: Cannot open directory '%s'\n", 
								caminho.c_str());
            sentCompleteData(LS, "LS:Error");
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
    sentCompleteData("ls", s);

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