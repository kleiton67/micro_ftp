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
		std::cin.clear();
    	std::getline(std::cin, comando);
		if(	!comandoRequerido(comando))
			return false;
		comando = "ERROR";
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
		sentCompleteData(LS, ".");
		receiveMsgRecordFile(".ls");
		//Imprime dados do arquivo
		imprimeFile(".ls");

    }
	else if(out[0] == "close")
	{
		std::cout << "Encerrando conexao com server!!! - Client\n";
		sentCompleteData(CLOSE, "...");
		return false;
	}
	else if(out[0] == "cd")
	{
		if(out.size() >= 2){
			sentCompleteData(CD, out[1]);
			std::string msg;
			msg = getCommand(receiveMsg());
			if(msg.compare(ERRO) == 0)
			{
				std::cout << "cd: Diretorio Não Encontrado!!!\n";
			}
			
		}
		else
		{
			msgIncomplete();
		}
		
	}
	else if(out[0] == "mkdir")
	{
		if(out.size() >= 2)
		{
			//sentCompleteData(MKDIR, local+"/"+out[1]);
			sentCompleteData(MKDIR, out[1]);
			std::string msg;
			msg = getCommand(receiveMsg());
			if(msg.compare(ERRO) != 0)
			{
				std::cout << "mkdir: Diretorio criado com sucesso!!!\n";
			}
			else
			{
				std::cout<<"mkdir: Problemas ao criar o diretorio!!!\n";
			}
			
		}
		else
		{
			msgIncomplete();
		}
		
	}
	else if(out[0] == "get")
	{
		if(out.size()>=2)
		{
			//Envia requisição
			//Aguarda retorno da mensagem com nome
			//Monta o arquivo
			sentCompleteData(GET, out[1]);
			std::string msg;
			msg = getCommand(receiveMsg());
			if(msg.compare(ERRO) != 0)
			{
				std::cout << "get: Recebendo Arquivo : " << out[1] << "\n";
				//receiveMsgRecordFile(out[1]);
				//receiveBinRecord(out[1]);
				receiveBinRecordFile(out[1]);
				std::cout << "get: Arquivo Recebido!!!\n";
			}
			else
			{
				std::cout << "get: Arquivo indisponivel no servidor!!!\n";
			}
			
		}
		else
		{
			msgIncomplete();
		}
		
	}
	else if(out[0] == "put")
	{
		//Envia arquivo ao servidor
		if(out.size()>=2)
		{
			std::fstream file(out[1], std::ios::binary|std::ios::in);
			if(file){
				file.close();
			//Envia mensagem com o dado e nome
				std::cout<<"CLIENT: Envia mensagem de solicitacao de PUT.\n";
				sentCompleteData(PUT, out[1]);
				//sleep(0.5);
				std::string msg;
				msg = getCommand(receiveMsg());
				if(msg.compare(ERRO)!=0)
				{
					//No retorno nao houve problema
					//Inicia transferencia
					//sleep(0.5);	
					std::cout << "CIENT: Enviando dados ao servidor!\n";
					sentFileBinArq(out[1], PUT);
				}
				else
				{
					std::cout << "Arquivo nao encontrado!!!\n";
				}
				
			}
			else
			{
				std::cout << "PUT: Arquivo Nao encontrado!!!";
			}
			
		}
		else
		{
			msgIncomplete();
		}
		
	}
    else
    {
        std::cout << "Comando desconhecido - Client!!!\n";
    }
	return true;
}

void Client::msgIncomplete()
{
	std::cout << "Comando Incompleto - Client!!!\n"; 
}

void Client::imprimeFile(std::string file)
{
	std::ifstream is (file);
	if (is) {
		std::cout << "Files and Directories: \n";
		// get length of file:
		is.seekg (0, is.end);
		int length = is.tellg();
		is.seekg (0, is.beg);	

		char * buffer = new char [length];

		is.read(buffer, length);

		std::cout << buffer;

		std::cout << "\n";

		delete[] buffer;
  }
  return ;
}