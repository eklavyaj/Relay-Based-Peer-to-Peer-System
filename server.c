// 1 commandline input: server_PortNo

// Refernce Links
// https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
// https://github.com/ashantanu/Relay-Based-Peer2Peer-system-Application/blob/master/client.c
// https://gist.github.com/nolim1t/126991/ae3a7d36470d2a81190339fbc78821076a4059f7

#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

void ERROR(char *message)
{
    printf("%s", message);
}

int main(int argc, char **argv)
{
    if (argc < 2)
        ERROR("Usage: %s <server_portNo>\n", argv[0]);

    // Opening a File where the Peer information will be stored
    FILE *peerInfo = fopen("peerInfo.txt", "w");

    // Creating a Socket for Server
    int serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)
    if (serverSocket < 0)
        ERROR("SOCKET COULDNT BE CREATED\n");
    
    // Creating Address struct for Server
    char *serverPort = argv[1]
    struct sockaddr_in serverAddr, peerAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htnol(INADDR_ANY);
    serverAddr.sin_port = htons(serverPort);

    int status = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))
    if (status < 0)
        ERROR("ERROR WHILE BINDING\n");

    status = listen(serverSocket, 4);
    if (status < 0)
        ERROR("ERROR WHILE LISTENING ON THE SOCKET");

    int peerSocket;
    char rcvBuffer[100], sendBuffer[50];
    for (int i = 0; i < 3; i++)
    {
        peerSocket = accept(serverSocket, (struct sockaddr *) &peerAddr, sizeof(peerAddr));
        if (peerSocket < 0)
            ERROR("ERROR WHILE ACCEPTING PEER");

        status = recv(peerSocket, rcvBuffer, sizeof(rcvBuffer));
        if (status < 0)
            ERROR("ERROR WHILE RECEIVING DATA FROM PEER\n");

        if (strcmp(rcvBuffer, "Request: peer") == 0);
        {
            printf("Data received from peer: %s\n", rcvBuffer);
            fprintf(peerInfo, "%s\n", rcvBuffer);

            

        }
        else
            ERROR("PEER SENDING WRONG REQUEST\n");
        
    }

    printf("Data recceived from all Peers\n");
    fclose(peerInfo);
    // status = close(serverSocket);

    // if (status < 0)
    //     ERROR("ERROR WHILE CLOSING THE SERVER SOCKET\n");


    // Wait for the client to make contact
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, sizeof(clientAddr));



}