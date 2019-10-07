/*
    MAIN DO LADO DO CLIENTE
*/

#include "client.h"
#include <vector>

#define PORTCMT 30100

using namespace connection;

int main()
{
	std::cout << "Bem vindo ao serviço FTP versao 1!!!\n";

    while(true)
    {
		Client cliente(PORTCMT);
        //Usuario entra com o comando a executar
		std::cout << "<semconexao>";
		std::string mensagem;
		std::getline(std::cin, mensagem);
		std::vector<std::string > commands;
		commands = cliente.tokenize(mensagem, ' ');

		if(commands.size()> 1 && commands[0] == "connect")
		{
			if(!cliente.connectServer(commands[1]))
				{
					std::cout << "Servidor indisponivel!!!!\n";
					continue;
				}
		}
		else if(commands.size() > 0 && commands[0] == "quit")
		{
			std::cout << "Deixando conexao\n";
			break;
		}
		else
		{
			std::cout << "Comando desconhecido!!!\n";
			continue;
		}

		cliente.comunicacaoServer();
		mensagem.clear();
		
    }
    return EXIT_SUCCESS;   
}