// 2 commandline inputs: server_IP & server_PortNo

https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
https://github.com/ashantanu/Relay-Based-Peer2Peer-system-Application/blob/master/client.c
https://gist.github.com/nolim1t/126991/ae3a7d36470d2a81190339fbc78821076a4059f7

#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

void ERROR(char *message)
{
    printf("%s", message);
    exit();
}

int CONNECT(char *address, int portNo);

int main(int argc, char **argv) 
{
    if (argc < 3)   
        ERROR("Usage: %s <server_IP> <client_Port>", argv[0]);
        
    
    int sockectToServer;
    sockectToServer = CONNECT(argv[1], argv[2]);

    // sending REQUEST for peer file to server
    char *requestMessage = "REQUEST : peer info", buffer[256];
    int count;

    count = send(sockectToServer, requestMessage, sizeof(requestMessage), 0);
    if (count < sizeof(requestMessage))
        ERROR("Failure in requesting Peer File");

    bzero(buffer, 256);
    count = recv(sockectToServer, buffer, 255, 0);

    if (count < 0)
        ERROR("Failure in obtained Peer File");

    printf("Recieved the following from the server - \n%s\n", buffer);
    printf("Closing connecction with the Server....\n");
    status = close(sockectToServer);

    if (status < 0)
        ERROR("Failure in closing the connection");

    // Saving the obtained data in a local file
    FILE *peerInfo = fopen("peerInfo.txt", "w");
    printf(peerInfo, "%s", buffer);
    fclose(peerInfo);


    // Asking user to give the required File name
    char fileName[100];
    printf("Enter the desired File Name: ");
    scanf("%s", fileName);

    // Contacting each Peer to check if they have the required file
    char peerName[INET_ADDRSTRLEN];
    int port;

    peerInfo = fopen("perrInfo.txt", "r");
    int socketToPeer, result;
    while (fscanf(peerInfo, "%s %d", peerName, &port) != EOF)
    {
        printf("Attempting to connect to peer%s:%d ...\n", peername, port);
        sockectToPeer = CONNECT(peerName, port);
        result = CHECK(socketToPeer, fileName);

        if (result == 0)
            continue;
        else 
            break;
    }

    if (result == 1)
        printf("File was Found!\n");
    

    else
        printf("File couldn't be found at any Peer....\n");

    return 0;

}


int CHECK(int socketToPeer, char *fileName)
{
    //Sending File Request to the Peer
    char request[100];
    sprintf(request, "REQUEST : FILE : %s", fileName);

    int count = send(socketToPeer, request, sizeof(request), 0);

    if (count < sizeof(request))
        ERROR("ERROR IN SENDING REQUEST TO PEER");
    
    char buffer[256];
    bzero(buffer, 256);
    count = recv(socketToPeer, buffer, 255, 0);

    if (count < 0)
        ERROR("ERROR WHILE RECEIVING PEER RESPONSE");

    if (strcmp(buffer, "File NOT FOUND") == 0) 
    {
        printf("File NOT FOUND at this Peer ... \n");
        count = close(socketToPeer);

        if (count < 0)
            ERROR("ERROR WHILE CLOSING CONNECTION WITH PEER\n");

        return 0;

        
    }

    else if (strcmp(buffer, "File FOUND") == 0)
    {
        printf("This Peer has the File !!!\n");

        count = recv(socketToPeer, buffer, 255, 0);
        if (count < 0)
            ERROR("ERROR OCCURED WHILE RECEIVING THE FILE\n");

        printf("File's content: %s", buffer);

        count = close(socketToPeer);
        if (count < 0)
            ERROR("ERROR WHILE CLOSING CONNECTION WITH PEER\n");

        // Saving the File on Client's machine
        FILE *file = fopen("DesiredFile.txt", "w");
        fprintf(file, "%s", buffer);
        fclose(file);

        printf("File saved on system !!!\n");

        return 1;

    }



}

int CONNECT(char *address, int portNo)
{

}










// // Creating a TCP Socket
// int socketId = socket(AF_INET, SOCK_STREAM, 0);
// if (socketId == 0) {
//     ERROR("socket creation error");
//     exit();
// }

// // For specifying the Address, a special data type is used


// bcopy((char *)server->h_addr, 
// (char *)&serv_addr.sin_addr.s_addr, server->h_length);
