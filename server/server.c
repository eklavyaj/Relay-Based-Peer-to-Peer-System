#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

int CONNECT(char *address, char *portNo);
void ERROR(char *message)
{
    printf("%s", message);
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("USAGE: %s <server_portNo>\n", argv[0]);
    	exit(0);
    }
    // printf("here");
    // Opening a File where the Peer information will be stored
    FILE *peerInfo = fopen("peerInfo.txt", "w+");

    // Creating a Socket for Server
    int serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0)
        ERROR("SOCKET COULDNT BE CREATED\n");
    
    // Creating Address struct for Server
    int serverPort = atoi(argv[1]);
    
    struct sockaddr_in serverAddr, peerAddr, clientAddr;
    bzero((char *) &serverAddr, sizeof(serverAddr));

    printf("SERVER LISTENING ON PORT: %s\n", argv[1]);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(serverPort);

    int status = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (status < 0)
        ERROR("ERROR WHILE BINDING\n");


    status = listen(serverSocket, 5);
    if (status < 0)
        ERROR("ERROR WHILE LISTENING ON THE SOCKET");

    int peerSocket, peerPort;
    char rcvBuffer[256], sendBuffer[50], peerName[INET_ADDRSTRLEN];;

    int size;

    for (int i = 0; i < 3; i++)
    {
    	bzero(rcvBuffer, 256);
    	size = sizeof(peerAddr);
        peerSocket = accept(serverSocket, (struct sockaddr *) &peerAddr, &size);
        if (peerSocket < 0)
            ERROR("ERROR WHILE ACCEPTING PEER");

        status = recv(peerSocket, rcvBuffer, sizeof(rcvBuffer), 0);
        if (status < 0)
            ERROR("ERROR WHILE RECEIVING DATA FROM PEER\n");
        printf("MESSAGE RECEIVED FROM PEER: %s\n", rcvBuffer);

        if (strcmp(rcvBuffer, "REQUEST by PEER") == 0)
        {
            // why add 200??
            peerPort = ntohs(peerAddr.sin_port)+200;
            if (inet_ntop(AF_INET,&peerAddr.sin_addr.s_addr,peerName,sizeof(peerName)) != NULL)
            {
                fprintf(peerInfo, "%s %d\n", peerName, peerPort);
                printf("PEER'S INFO: %s %d\n", peerName, peerPort);

                sprintf(sendBuffer, "RESPONSE: peer: %d", peerPort);
                printf("SENDING REPLY: %s\n\n", sendBuffer);
                status = send(peerSocket, sendBuffer, sizeof(sendBuffer), 0);

                if (status < sizeof(sendBuffer))
                    ERROR("ERROR WHILE RESPONDING TO PEER\n");

            }
            else
                ERROR("UNABLE TO GET ADDRESS\n");
        }



        // if (strcmp(rcvBuffer, "REQUEST by PEER") != 0);
        // 	ERROR("PEER SENDING WRONG REQUEST\n");

        // else
        //     ERROR("PEER SENDING WRONG REQUEST\n");

        status = close(peerSocket);
        if (status < 0)
            ERROR("ERROR WHILE CLOSING PEERSOCKET\n");
    }

    printf("DATA RECEIVED FROM ALL PEERS\n\n");
    // fclose(peerInfo);


    // Wait for the client to make contact
    size = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &size);
    status = recv(clientSocket, rcvBuffer, 255, 0);
    if (strcmp(rcvBuffer, "REQUEST FOR PEER INFO") == 0)
    {

        fseek(peerInfo, 0, SEEK_END);
        long fsize = ftell(peerInfo);
        printf("FILE SIZE= %ld\n", fsize);
        fseek(peerInfo, 0, SEEK_SET);//send the pointer to beginning of the file

        // char *string = malloc(fsize + 1);
        char *string = (char *) malloc(sizeof(char)*(fsize+1));
        fread(string, fsize, 1, peerInfo);
        fclose(peerInfo);

        string[fsize] = 0;
        printf("SERVER HAS THE FOLLOWING INFO:\n%s", string);

        status = send(clientSocket, string, strlen(string), 0);
        if (status < 0)
            ERROR("ERROR WILL WRTING TO SOCKET\n");
    }
    else
        ERROR("WRONG REQUEST RECEIVED FROM CLIENT\n");

    status = close(clientSocket);
    if (status < 0)
        ERROR("ERROR WHILE CLOSING CLIENTSOCKET\n");

    status = close(serverSocket);
    if (status < 0)
        ERROR("ERROR WHILE CLOSING SERVERSOCKET\n");

}
