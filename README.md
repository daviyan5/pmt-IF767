# PMT - String Matching CLI

*Projeto da disciplina de Processamento de Cadeia de Caracteres (IF767)*

PMT é uma ferramenta de casamento de padrões paralelizada, inspirada no comando "**grep**" dos sistemas _UNIX/Linux_.

## Dependências
_GNU Compiler Collection (gcc)_ é requisitado para a execução.
O comando "make" é altamente recomendado.

## Instalação
Na pasta principal da ferramenta, o seguinte comando deve ser executado no terminal:

```bash
make
```
Um arquivo "_pmt_" deve ser criado em ./bin.

## Execução
A ferramenta é executada na pasta ./bin, de acordo com:

```bash
./pmt [options] pattern textfile [textfile...]
```
Mais detalhes sobre as opções podem ser adquiridos com 

```bash
./pmt -h
./pmt --help
```

Para limpeza dos outputs e do executável do programa:
```bash
make clean
```
