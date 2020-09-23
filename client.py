from socket import *
# importa todas as fun ̧c~oes providas pela biblioteca socket
serverName = '8.8.8.8' # ajuste de vari ́aveis
serverPort = 53
clientSocket = socket(AF_INET, SOCK_DGRAM)
# cria o socket# AF_INET: IPv4# SOCK_DGRAM: UDP
message = input('Input lowercase sentence:')
message_bytes = bytes(message, 'utf-8')
# coleta a mensagem digitada pelo usu ́ario
clientSocket.sendto(message_bytes,(serverName, serverPort))
# envia a mensagem (nao se estabelece conexao!)modifiedMessage, 
print("antes de esperar")
print(clientSocket.recvfrom(2048))
print("DEPOIS DE ESPERAR")
# espera resposta do servidorprint modifiedMessage# imprime 
mensagemclientSocket.close()