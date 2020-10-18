#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
void ERROR(char *message);
int CONNECT(char *address, char *portNo);
int CHECK(int socketToPeer, char *fileName);
void CLOSE(int socketToPeer);


int main(int argc, char **argv) 
{
    if (argc != 3)  
    { 
        printf("USAGE: %s <server_IP> <client_Port>", argv[0]);
        exit(0);
    }
        
    
    int sockectToServer;
    sockectToServer = CONNECT(argv[1], argv[2]);

    // sending REQUEST for peer file to server
    char *requestMessage = "REQUEST FOR PEER INFO", buffer[256];
    int count;

    count = send(sockectToServer, requestMessage, strlen(requestMessage), 0);
    if (count < strlen(requestMessage))
        ERROR("FAILURE IN REQUESTING PEER FILE");

    bzero(buffer, 256);
    count = recv(sockectToServer, buffer, 255, 0);

    if (count < 0)
        ERROR("FAILURE IN OBTAINED PEER FILE\n");

    printf("\nRECEIVED THE FOLLOWING FROM THE SERVER: \n%s\n", buffer);
    printf("CLOSING CONNECTION WITH THE SERVER.\n\n");
    int status = close(sockectToServer);

    if (status < 0)
        ERROR("FAILURE IN CLOSING THE CONNECTION\n");

    // Saving the obtained data in a local file
    FILE *peerInfo = fopen("peerInfo2.txt", "w");
    fprintf(peerInfo, "%s", buffer);
    fclose(peerInfo);


    // Asking user to give the required File name
    char fileName[100];
    printf("ENTER NAME OF THE DESIRED FILE: ");
    scanf("%s", fileName);

    // Contacting each Peer to check if they have the required file
    char peerName[INET_ADDRSTRLEN];
    char port[256];

    peerInfo = fopen("peerInfo2.txt", "r");
    int sockectToPeer, result = 0;
    char temp;
    while (fscanf(peerInfo, "%s%c%s", peerName, &temp, port) != EOF)
    // while (fscanf(peerInfo, "%s", port) != EOF)

    {
        printf("ATTEMPTING TO CONNECT TO PEER -  %s:%s ...\n",peerName, port);
        sockectToPeer = CONNECT(peerName, port);
        printf("CONNECTED TO A PEER SUCCESSFULLY!\n");


        if (result == 0)
        	result = CHECK(sockectToPeer, fileName);
        else if (result == 1){
            printf("FILE ALREADY FOUND IN A PREVIOUS PEER, HENCE CLOSING SOCKET...\n");
        	CLOSE(sockectToPeer);
        }
        printf("\n");  
    }


    if (result == 1)
        printf("\nFILE WAS FOUND!\n");
    
    else
        printf("FILE COULD NOT BE FOUND AT PEER.\n\n");

    return 0;

}


void CLOSE(int socketToPeer)
{
	char request[] = "REQUEST TO CLOSE";

    int count = send(socketToPeer, request, strlen(request), 0);
    printf("MESSAGE SENT TO PEER: %s\n", request);

    if (count != strlen(request))
        ERROR("ERROR IN SENDING REQUEST TO PEER");

    count = close(socketToPeer);
    if (count < 0)
        ERROR("ERROR WHILE CLOSING CONNECTION WITH PEER\n");

    return ;
}


int CHECK(int socketToPeer, char *fileName)
{
    //Sending File Request to the Peer
    char request[100];
    sprintf(request, "REQUEST FILE by CLIENT :%s", fileName);

    int count = send(socketToPeer, request, strlen(request), 0);
    printf("MESSAGE SENT TO PEER: %s\n", request);

    if (count != strlen(request))
        ERROR("ERROR IN SENDING REQUEST TO PEER");
    
    char buffer[256];
    bzero(buffer, 256);
    count = recv(socketToPeer, buffer, 255, 0);

    if (count < 0)
        ERROR("ERROR WHILE RECEIVING PEER RESPONSE\n");

    if (strcmp(buffer, "FILE NOT FOUND") == 0) 
    {
        printf("FILE NOT FOUND AT THIS PEER.\n\n");
        count = close(socketToPeer);

        if (count < 0)
            ERROR("ERROR WHILE CLOSING CONNECTION WITH PEER\n");

        return 0;
    }

    else if (strcmp(buffer, "FILE FOUND") == 0)
    {
        printf("THIS PEER HAS THE FILE!\n");

        count = recv(socketToPeer, buffer, 255, 0);
        if (count < 0)
            ERROR("ERROR OCCURED WHILE RECEIVING THE FILE\n");

        printf("CONTENTS OF THE FILE: \n%s\n", buffer);

        count = close(socketToPeer);
        if (count < 0)
            ERROR("ERROR WHILE CLOSING CONNECTION WITH PEER\n");

        // Saving the File on Client's machine
        FILE *file = fopen("DesiredFile.txt", "w");
        fprintf(file, "%s", buffer);
        fclose(file);

        printf("FILE SAVED ON SYSTEM!\n\n");

        return 1;
    }
}


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

void ERROR(char *message)
{
    printf("%s", message);
    exit(0);
}
