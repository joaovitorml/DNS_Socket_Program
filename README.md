# DNS_Socket_Program

## Somativa 1 – Fundamentos de Redes de Computadores

Aluno/Matricula:
- João Vítor Morandi Lemos/160010195
- José Aquiles Guedes de Rezende / 160010331

Professor:

- Tiago Alves

### 1 - Qual sistema operacional foi usado na construção do sistema?
R: Ubuntu 16.04 (João Vítor) e linux mint 20(José Aquiles)

### 2 - Qual ambiente de desenvolvimento foi usado?
R: Visual Studio Code
Compilador: gcc 9.3.0

### 3 - Como construir a aplicação?
R: gcc -o prog client.c
Após o build, é gerado o executável “prog”, na mesma pasta do arquivo em c.

### 4 - Como executar a aplicação?
R: ./prog <hostname> <servidor DNS>
Exemplo: ./prog unb.br 8.8.8.8

### 5 - Quais são as telas (instruções de uso)?
R:

    Input1: ./prog unb.br 8.8.8.8
    Saída no console: unb.br <> unb-br.mail.protection.outlook.com
---
    Input 2: ./prog imagdaskdasdasj.br 1.1.1.1
    Saída no console: Dominio imagdaskdasdasj.br não encontrado
---
    Input 3: ./prog fga.unb.br 8.8.8.8
    Dominio fga.unb.br não possui entrada MX
---
    Input 4: ./prog unb.br 1.2.3.4
    Nao foi possível coletar entrada MX para unb.br

### 6 - Quais são as limitações conhecidas?
R:
* Apenas consultas do tipo MX são realizadas pela aplicação;
* Warning da função “inet_aton” não tratado, mas apenas uma limitação visual, sem
alterações na execução;
* Entradas fora do padrão (com “www” ou “https” na frente) não conseguem ser
encontradas;
* Entradas MX com caracteres não textuais como a do google.com, não são exibidos os
caracteres não textuais;
* Exibe somente uma entrada MX;