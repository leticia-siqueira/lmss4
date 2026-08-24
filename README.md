# Ordenador de Processos 

O ProcessFlow, é um orquestrador de processos feito em C. 
A ideia é simular um terminal, como por exemplo o powershell:
você cadastra "tarefas" (programas do sistema, tipo `ls`, `sort`, `cat`...) e manda
o ProcessFlow executar elas — sozinhas, em sequência, em paralelo ou até em pipe,
igualzinho um shell de verdade faria por baixo dos panos.

Rubricas utilizadas:
- README + Makefile 
- Relatório 
- Execução simples + exit 
- Sequencial + paralelo 
- Pipe 
- Redirecionamento 
- Background + jobs + wait 
- workflowFile + workdir 
- Erros + outras situações 

## Como rodar

Limpar os arquivos compilados:
make clean

Compilar:
make

Rodar no modo interativo:
./processflow

Rodar no modo workflow:
./processflow workflow.pf

Pra sair:
exit
