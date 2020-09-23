from socket import *
# importa todas as fun ̧c~oes providas pela biblioteca socket
serverName = '1.1.1.1' # ajuste de vari ́aveis
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)
# cria o socket# AF_INET: IPv4# SOCK_DGRAM: UDP
message = input('Input lowercase sentence:')
# coleta a mensagem digitada pelo usu ́ario
clientSocket.sendto(bytes(message,'utf-8'),(serverName, serverPort))
# envia a mensagem (nao se estabelece conexao!)modifiedMessage, 
print("antes de esperar")
serverAddress = clientSocket.recvfrom(4096)
print("DEPOIS DE ESPERAR")
# espera resposta do servidorprint modifiedMessage# imprime 
mensagemclientSocket.close()