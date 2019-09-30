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
			/*
				Cria arquivo e envia ao cliente
			*/
			std::cout << "Comando LS\n";
            if(getTamanho(msg) < 13)
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
			std::cout << "Comando CD\n";
			std::string dados;
			dados = getData(msg);
			cd(dados);
        }
        else if(aux.compare("GET") == 0)
        {
			
        }
        else if(aux.compare("PUT") == 0)
        {
            
        }
        else if(aux.compare("MKDIR") == 0)
        {
			std::cout << "Comando MKDIR\n";
			mkdir(getData(msg));
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

std::string Ftp::receiveAllMsg()
{
	char data[TAM_DATA];
	std::string msg;
	std::string r_msg;

	msg = receiveMsg();
	while(nextMessage(msg))
	{
		msg.clear();
		msg = receiveMsg();
	}
	r_msg.append(getData(msg));

	return r_msg;
}

bool Ftp::receiveMsgRecordFile(std::string caminho){
	//Faz manipulações internas em arquivos binarios
	std::fstream file(caminho, std::ios::binary|std::ios::out);
	if(file){
		std::string msg;
		//std::cout << "Receive: Recebendo Primeira mensagem!!!\n";
		msg = receiveMsg();
		//std::cout << "Receive: Entrando no Laco\n";
		while(nextMessage(msg))
		{
			//Para cada mensagem recebida
			//Extrair dados e colocar no arquivo
			file << getData(msg);
			msg.clear();
			msg = receiveMsg();
			//std::cout << "Receive: Recebendo mensagem!!!\n";
		}
		file << getData(msg);
		file.close();
		//std::cout << "Receive: Fim da recepcao do arquivo!!!\n";
		return true;
	}
	else
	{
		std::cout << "Erro no arquivo: " << caminho << "\n";
		return false;
	}
	return false;
}

bool Ftp::sentFile(std::string caminho, std::string cmd)
{
	std::fstream file(caminho, std::ios::binary|std::ios::in);
	if(file){
		char c;
		int sent = 0;
		std::string msg;
		while (file.get(c)){
			msg.push_back(c);
			sent+=1;
			//Se atingir o tamanho maximo
			// da mensagem enviar o arquivo
			if(sent == 1400)
			{
				sentData(true, cmd, msg);
			}
		}

		//Envia o resto que faltou
		sentData(false, cmd, msg);

		//Ultima parte do arquivo
	}
	else
	{
		std::cout << "Erro no arquivo: " << caminho << "- sentFile\n";
		return false;
	}
	return false;
}

std::string Ftp::receiveMsg()
{
    /*
        Receber apenas dados
		Precisa implementar a obtenção de dados de varias 
			mensagens
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
	//Nome do arquivo para armazenar funcoes do ls
	std::string name;
	name = ".ls-"+ std::to_string(sock);

    std::vector<std::string > v;
    DIR* dirp = opendir(caminho.c_str());
    if (dirp == NULL) {
            printf ("Error CD: Cannot open directory '%s'\n", 
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
    //Enviar dados ems string
    //sentCompleteData("ls", s);
	//Gravar dados em um arquivo
	//E enviar para o get iniciar transferencia
	//ao cliente
	std::fstream file(name, std::ios::out);
	file << s;
	file.close();
	sentFile(name, LS);
	//Remover arquivo
	rmdir(name.c_str());
    return true;
}

bool Ftp::cd(std::string name)
{
	//Navega pelos diretorios
	DIR* dirp = opendir(name.c_str());
    if (dirp == NULL) {
            printf ("Error LS: Cannot open directory '%s'\n", 
								name.c_str());
            sentCompleteData(CD, "CD:Error");
            return false;
        }
	else
	{
		sentCompleteData(CD, "CD:OK");
	}
	return true;
}

bool Ftp::mkdir(std::string name)
{
	std::string diretorio;
	diretorio = "mkdir -p " + name; 
	const int dir_err = system(diretorio.c_str());
	if (-1 == dir_err)
	{
		printf("Error creating directory!n");
		sentCompleteData(MKDIR, "MKDIR:Error");
		exit(1);
	}
	else
	{
		std::cout << "Diretorio: " << name << "criado com sucesso!!!\n";
		sentCompleteData(MKDIR, "MKDIR:OK");
	}
	
}

bool Ftp::get(std::string file)
{

}

bool Ftp::put()
{

}