# Trabalho de Tópicos em Programção de redes

A aplicação foi versionada utilizando o GIT, no repositório remoto [github][repo]
 on GitHub.

### Instalação

Essa aplicação requer um sistema Linux para ser compilada, seus pre-requisitos podem ser corrigidos com o seguinte comando;

```sh
$ sudo apt-get install build-essentials make
```

### Development

O maior contribuidor do repositorio, foi o desenvolvedor do projeto [Kleyton S. Leite] [kleyton], com varias dúvidas sanadas pelo professor da disciplina, e outras relacionadas ao C++, com um grande amigo, [Miguel Nischor][miguel].

#### Building for source
Para gerar a aplicação é necessário compilar o código. Esses executáveis estão na pasta bin.
Para compilar é necessário alguns comandos:

#### Cliente
O cliente é responsável  por uma interface com o usuário e se conectar com o servidor. Para compilar:
```sh
$ make -f makeclient 
```
#### Servidor
O servidor trata as operações solicitadas, porém não implementa nenhuma interface com o usuário. Para compilar:
```sh
$ make -f makeclient 
```

### Como usar?
O servidor pode ser iniciado com o seguinte comando:
```sh
$ ./server.run
```
E o cliente:
```sh
$ ./client.run 
```

São implementado os comandos:
* connect: Espera o ip do servidor como entrada;
* ls: Solicita ao servidor a impressão de arquivos da pasta atual;
* cd: Muda o servidor de diretório;
* mkdir: Cria uma pasta no diretório atual do servidor;
* get: Baixa um arquivo do repositório atual do servidor;
* put: Envia um arquivo do cliente ao diretório atual do servidor;
* quit: Fecha conexão com o servidor.



   [repo]: <https://github.com/joemccann/dillinger>
   [miguel]: <https://github.com/mgnischor>
   [kleyton]: <https://github.com/kleiton67>

