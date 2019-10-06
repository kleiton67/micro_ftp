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
    case caractereDep: 
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
			std::cout << "Comando GET\n";
			get(getData(msg));
        }
        else if(aux.compare("PUT") == 0)
        {
            std::cout << "Comando PUT\n";
			put(getData(msg));
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
			std::cout << "Comando desconhecido!!!\n";
		}
    }
}

std::string Ftp::receiveAllMsg()
{
	std::string msg;
	std::string r_msg;

	msg = receiveMsg();
	while(nextMessage(msg))
	{
		msg.clear();
		msg = receiveMsg();
		r_msg.append(getData(msg));
	}
	r_msg.append(getData(msg));

	return r_msg;
}

bool Ftp::receiveMsgRecordFile(std::string caminho){
	//Faz manipulações internas em arquivos binarios
	std::fstream file(caminho, std::ios::binary|std::ios::out);
	if(file){
		std::string msg;
		msg = receiveMsg();
		while(nextMessage(msg))
		{
			//Para cada mensagem recebida
			//Extrair dados e colocar no arquivo
			file << getData(msg);
			msg.erase();
			msg = receiveMsg();
		}
		file << getData(msg);
		file.close();
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
			if(sent == (TAM_DATA-1))
			{
				sentData(true, cmd, msg);
				sent = 0;
				msg.erase();
			}
		}
		//Envia o resto que faltou
		sentData(false, cmd, msg);

		//Ultima parte do arquivo
	}
	else
	{
		std::cout << "SENTFILE:Erro no arquivo: " << caminho << "\n";
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
   char data[TAM_DATA+TAM_CAB];
   std::string msg;
   read(sock, data, TAM_DATA+TAM_CAB);
   msg.append(data);
   return msg;
}

bool Ftp::sentData(bool more, std::string cmd, std::string msg)
{
    std::string sentMsg;
    char data[TAM_DATA+TAM_CAB];
    if(msg.size() <= TAM_DATA){
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
    
    memset(data, caractereDep, TAM_CAB+TAM_DATA);
    strcpy(data, sentMsg.c_str());
    write(sock, data, TAM_DATA+TAM_CAB);

    return true;
}



bool Ftp::sentCompleteData(std::string cmd, std::string msg)
{
	/*
		Não é muita usada por guardar tudo em memória
	*/
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
            sentCompleteData(ERRO, "LS:Error");
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
            sentCompleteData(ERRO, "CD:ERROR");
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
		sentCompleteData(ERRO, "MKDIR:ERROR");
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
	//Envia primeira mensagem com o nome se achou o arquivo
	//Caso contrario envia uma mensagem de erro
	std::fstream arquivo(file);
	if(arquivo)
	{
		sentCompleteData(GET, file);
		std::cout << "Enviando arquivo: " << file << "\n";
		//sentFile(file, GET);
		//sentFileBin(file, GET);
		sentFileBinArq(file, GET);
	}
	else
	{
		sentCompleteData(ERRO, "GET:ERROR");
	}
}

bool Ftp::put(std::string file)
{
	//Responde ao usuario
	//Pois oque pode ocorrer é o arquivo ja existir
	//no server
	std::fstream arq(file);
	if(!arq)
	{
		sentCompleteData(PUT, "PUT:OK");
		std::cout << "PUT:Recebendo arquivo!!!\n";
		receiveMsgRecordFile(file);
		std::cout << "PUT:Arquivo Recebido do Cliente!!!\n";
	}
	else
	{
		sentCompleteData(ERRO, "PUT:ERROR");
		return false;
	}
	return true;
	
}

bool Ftp::sentFileBinArq(std::string caminho, std::string cmd)
{
	int sent = 0;
	std::fstream file(caminho, std::ios::binary|std::ios::in);
	if(file){
		char buffer[TAM_DATA];
		char  temp[1];
		char *palavra = new char[TAM_DATA+TAM_CAB];
		std::cout << "sentFileBinArq: Enviando file\n";
		while(file.tellg()!=EOF)
		{
			//std::cout << "sentFileBinArq: Enviando file1\n";
			file.read(temp, 1);
			//std::cout << temp;
			buffer[sent] = *temp; 
			sent++;
			if(sent ==(TAM_DATA - 1))
			{
				//std::cout << "sentFileBinArq: Enviando file2\n";
				//Envia no tamnho de TAM_DATA
				makeWordchar(palavra, cmd.c_str(), (char*)"NM", buffer, TAM_DATA);
				write(sock, palavra, TAM_DATA+TAM_CAB);
				//send(sock, palavra, TAM_DATA+TAM_CAB, 0);
				sent = 0;
			}
		}
		std::cout << "sentFileBinArq: Enviando file3\n";
		//Envia o resto
		makeWordchar(palavra, cmd.c_str(), (char*)"FM", buffer, sent);
		print(palavra, 30);
		write(sock, palavra, TAM_DATA+TAM_CAB);
		//send(sock, palavra, TAM_DATA+TAM_CAB, 0);
		std::cout << "SentFileBin: Enviado com sucesso: " << 
		caminho << "\n";
		return true;
	}
	else
	{
	std::cout << "SentFileBin: Problema ao abrir o arquivo " << 
		caminho << "\n";
		return false;
	}
	
}

bool Ftp::receiveBinRecordFile(std::string caminho)
{
	std::fstream file(caminho, std::ios::binary|std::ios::out);
	if(file)
	{
		char * palavra = new char[TAM_DATA+TAM_CAB];
		char * data = new char[TAM_DATA];
		memset(palavra, caractereDep, TAM_DATA+TAM_CAB);
		std::cout << "RcvBin: Entrada na funcao\n";
		//read(sock, palavra, TAM_DATA+TAM_CAB);
		recv(sock, palavra, TAM_CAB+TAM_DATA, 0);
		std::cout << "RcvBin: Recebido Primeiro mensagem\n";
		while(nextMessage(palavra))
		{
			getData(palavra, data);
			file.write(data, TAM_DATA);
			//read(sock, palavra, TAM_DATA+TAM_CAB);
			recv(sock, palavra, TAM_CAB+TAM_DATA, 0);
		}
		getData(palavra, data);
		int t_msg;
		t_msg = getTamanho(palavra);
		std::cout << "RcvBin: Tamanho da mensagem: " << t_msg << "\n";
		file.write(data, t_msg);
		std::cout << "RcvBin: Recebido ultimo mensagem " 
		<< "\n";
		file.close();
		return true;
	}
	else
	{
		std::cout << "Problema ao manipular arquivo!!!\n";
		return false;
	}
}