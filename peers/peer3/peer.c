#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int CONNECT(char *address, char *portNo);
void ERROR(char *message);


int main(int argc, char **argv)
{
    int status;

    // check if the arguments are valid or not
    if (argc != 3)
    {
        printf("USAGE: %s <server_IP> <server_Port>\n", argv[0]);
        exit(0);
    }

    // create a connection socket to server
    int peerSocket; 
    peerSocket = CONNECT(argv[1], argv[2]);

    char *sendBuffer = "REQUEST by PEER", rcvBuffer[256];
    //sending request to connect to server
    status = send(peerSocket, sendBuffer, strlen(sendBuffer), 0);
    printf("MESSAGE SENT BY PEER: %s\n", sendBuffer);

    // printf("size fo sendBuffer: %d\n", (int) strlen(sendBuffer));

    if (status < strlen(sendBuffer))
        ERROR("ERROR IN SENDING MESSAGE TO SERVER\n");

    // fill rcvBuffer with 0
    bzero(rcvBuffer, 256);
    // receive message into rcvBuffer
    status = recv(peerSocket, rcvBuffer, 255, 0);
    if (status < 0)
        ERROR("ERROR IN RECEIVING MESSAGE TO SERVER\n"); 

    printf("MESSAGE RECEIVED FROM SERVER: %s\n", rcvBuffer);

    // compare response 
    char c = rcvBuffer[13];
    if (c == 'r')
    {
        printf("CORRECT MESSAGE FROM SERVER\nCLOSING CONNECTION\n");
        status = close(peerSocket);
        if (status < 0)
            ERROR("ERROR CLOSING CONNECTION WITH SERVER\n");

        printf("PEER PORT: %s\n\n", &rcvBuffer[16]);

    }

    else
        ERROR("WRONG MESSAGE RECEIVED FROM SERVER\n");

// --------------------------------------------------------------------------------------------------

    // Now the peer node becomes a server
    int serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0)
        ERROR("SOCKET COULDNT BE CREATED\n");
    
    // Creating Address struct for Server

    int serverPort = atoi(&rcvBuffer[16]);
    struct sockaddr_in serverAddr, clientAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(serverPort);

    int size = sizeof(serverAddr);
    status = bind(serverSocket, (struct sockaddr *) &serverAddr, size);
    if (status < 0)
        ERROR("ERROR WHILE BINDING\n");

    status = listen(serverSocket, 5);
    if (status < 0)
        ERROR("ERROR WHILE LISTENING ON THE SOCKET\n");

    printf("SERVER RUNNING ON PEER NODE...\n");

    // Accepting connection from the client
    size = sizeof(clientAddr);
    int socketToClient = accept(serverSocket, (struct sockaddr *) &clientAddr, &size);
    if (socketToClient < 0)
        ERROR("ERROR IN ACCEPTING CLIENT CONNECTION\n");

    char buffer[256];
    bzero(buffer, 256);
    
    //receive request message from client
    status = recv(socketToClient, buffer, sizeof(buffer), 0);
    if (status < 0)
        ERROR("ERROR IN RECEIVING MESSAGE FROM CLIENT\n");

    printf("MESSAGE REVEIVED FROM CLIENT: %s\n", buffer);

    char check[] = "REQUEST FILE by CLIENT :";

    int i, flag = 1;
    for(i=0;i<strlen(check);i++)
    {
		if(check[i]==buffer[i]) 
            continue; 
		else 
        {
			flag=0;
			break;
		}
	}

    if (flag == 0)
    {
        flag = 2;
        char check2[] = "REQUEST TO CLOSE";
        // int i, flag = 1;
        for(i=0;i<strlen(check2);i++)
        {
            if(check2[i]==buffer[i]) 
                continue; 
            else 
            {
                flag=0;
                break;
            }
        }

    }

    if (flag == 0)
        ERROR("WRONG MESSAGE RECEIVED FROM CLIENT\n");

    
    else if (flag == 1)
    {
        printf("CLIENT SEEKS FILE: %s\n", &buffer[strlen(check)]);

        FILE *peerInfo = fopen(&buffer[strlen(check)], "r");
        
        // check if file present in peer directory
        if (peerInfo == NULL)
        {
            printf("REQUESTED FILE NOT WITH PEER\n");
            
            sendBuffer = "FILE NOT FOUND";
            status = send(socketToClient, sendBuffer, strlen(sendBuffer), 0);
            if (status < 0)
                ERROR("ERROR IN SENDING MESSAGE TO CLIENT\n");
        }

        else
        {
            printf("REQUESTED FILE FOUND AT PEER\n");
            sendBuffer = "FILE FOUND";
            status = send(socketToClient, sendBuffer, strlen(sendBuffer), 0);
            if (status < 0)
                ERROR("ERROR IN SENDING MESSAGE TO CLIENT\n");

             //send the file
            fseek(peerInfo, 0, SEEK_END);
            long fsize = ftell(peerInfo);
            fseek(peerInfo, 0, SEEK_SET); //send the pointer to beginning of the file

            char *string = malloc(fsize + 1);
            fread(string, fsize, 1, peerInfo);
            fclose(peerInfo);

            // send the file data to client
            status = send(socketToClient, string, strlen(string), 0);
            if (status < 0)
                ERROR("ERROR IN SENDING FILE DATA TO CLIENT\n");

        }

        status = close(socketToClient);
        if (status < 0)
            ERROR("ERROR IN CLOSING DATA SOCKET\n");

        status = close(serverSocket);
        if (status < 0)
            ERROR("ERROR IN CLOSING SERVER SOCKET\n");
    } 

    else
    {
        status = close(socketToClient);
        if (status < 0)
            ERROR("ERROR WHILE CLOSING SOCKET TO PEER\n");
        printf("FILE ALREADY FOUND ON A PREVIOUS PEER, CLOSING THE PEER SERVER\n");
    }


}

//connect to socket at given address and port number
int CONNECT(char *address, char *portNo)
{
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        ERROR("ERROR IN CREATING A SOCEKT\n");

    int port = atoi(portNo);
    struct sockaddr_in Addr;
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr(address);
    Addr.sin_port = htons(port);

    int status = connect(sock, (struct sockaddr *) &Addr, sizeof(Addr));
    if (status < 0)
        ERROR("ERROR WHILE CONNECTING TO SOCKET\n");

    return sock;
}

// function that raises error and exits the execution if error is raised
void ERROR(char *message)
{
    printf("%s", message);
    exit(0);
}