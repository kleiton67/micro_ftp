/*

    KLEYTON LEITE

    IMPLEMENTAÇÃO DO CABEÇALHO CLIENT.H

*/

#include "client.h"

using namespace connection;

Client::Client(std::string address, short int port)
{
	this->address = address;
    memset(&confServer, '0', sizeof(confServer));
	confServer.sin_addr.s_addr = inet_addr(address.c_str());
    confServer.sin_family = AF_INET;
    this->port = port;
    confServer.sin_port = htons(port);
}

Client::Client(short int port)
{
    memset(&confServer, '0', sizeof(confServer));
    confServer.sin_family = AF_INET;
    this->port = port;
    confServer.sin_port = htons(port);
}

Client::~Client()
{
    
}

bool Client::connectServer(std::string addressServer)
{
	address = addressServer;
	confServer.sin_addr.s_addr = inet_addr(addressServer.c_str());
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(sock, (struct sockaddr *)&confServer,
                             sizeof(confServer))<0)
    {
        printf("\nConnection failed due to port and ip problems\n");
        return false;
    }

    return true;
}

bool Client::comunicacaoServer()
{
   while(true)
   {
       //imprime o cabecalho no terminal de comunicacao
       imprimeCabecalho();
       
    	std::string comando;
    	std::getline(std::cin, comando);
		if(	!comandoRequerido(comando))
			return false;


   }
}

std::vector<std::string> Client::tokenize(std::string const &str,
                                                 const char delim)
{
    std::vector<std::string> out;
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
    return out;
}

void Client::imprimeCabecalho()
{
    std::cout << '<' << address << '>' << ':';
}

bool Client::comandoRequerido(std::string mensagem)
{
    std::vector<std::string > out;
    out = tokenize(mensagem, ' ');

	if(out.size() == 0)
		return false;

	if(out[0] == "ls")
	{
		if(out.size() > 2){
			//std::cout <<"Dados enviados ao server!!!\n";
			sentCompleteData(LS, out[2]);
		}
		else
		{
			//std::cout << "Envio de dados!!!\n";	
			sentCompleteData(LS, ".");
			std::string msg_receive;
			//std::cout << "Recebendo dados!!!3\n";
			msg_receive = receiveMsg();
			std::cout << getData(msg_receive);
		}
    }
	else if(out[0] == "close")
	{
		std::cout << "Encerrando conexao com server!!!\n";
		sentCompleteData(CLOSE, "...");
		return false;
	}
	else if(out[0] == "cd")
	{
		if(out.size() >= 2){
			sentCompleteData(CD, out[1]);
		}
		else
		{
			msgIncomplete();
		}
		
	}
    else
    {
        std::cout << "Comando desconhecido!!!\n";
    }
	return true;
}

void msgIncomplete()
{
	std::cout << "Comando Incompleto!!!\n"; 
}