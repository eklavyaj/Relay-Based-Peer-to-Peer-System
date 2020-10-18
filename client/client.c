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
    //checking if arguments provided are ok
    if (argc != 3)  
    { 
        printf("USAGE: %s <server_IP> <client_Port>", argv[0]);
        exit(0);
    }
        
    //connect client to server first
    int sockectToServer;
    sockectToServer = CONNECT(argv[1], argv[2]);

    // sending REQUEST for peer info file to server
    char *requestMessage = "REQUEST FOR PEER INFO", buffer[256];
    int count;

    count = send(sockectToServer, requestMessage, strlen(requestMessage), 0);
    if (count < strlen(requestMessage))
        ERROR("FAILURE IN REQUESTING PEER FILE");

    // fill buffer with 0
    bzero(buffer, 256);

    //receive data through buffer
    count = recv(sockectToServer, buffer, 255, 0);

    if (count < 0)
        ERROR("FAILURE IN OBTAINED PEER FILE\n");


    printf("\nRECEIVED THE FOLLOWING FROM THE SERVER: \n%s\n", buffer);
    printf("CLOSING CONNECTION WITH THE SERVER.\n\n");
    
    //close socket to server
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

    //peerInfo2.txt contains the copied information from peerInfo.txt (information of peers stored on the server)
    peerInfo = fopen("peerInfo2.txt", "r");
    int sockectToPeer, result = 0;
    char temp;
    
    // going through each peer, creating a connection socket and then checking if that peer has the file or not
    while (fscanf(peerInfo, "%s%c%s", peerName, &temp, port) != EOF)
    {
        printf("ATTEMPTING TO CONNECT TO PEER -  %s:%s ...\n",peerName, port);
        //connect
        sockectToPeer = CONNECT(peerName, port);
        printf("CONNECTED TO A PEER SUCCESSFULLY!\n");

        // if the file has not been found till now, check if the most recently connected peer has the file
        if (result == 0)
        	result = CHECK(sockectToPeer, fileName);
        // if file has already been found, close connection with the peers and print a message saying the same
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


// function that closes the socket connection between client and peer. 
void CLOSE(int socketToPeer)
{
    // request message
	char request[] = "REQUEST TO CLOSE";

    // sending request to close socket
    int count = send(socketToPeer, request, strlen(request), 0);
    printf("MESSAGE SENT TO PEER: %s\n", request);

    if (count != strlen(request))
        ERROR("ERROR IN SENDING REQUEST TO PEER");

    count = close(socketToPeer);
    if (count < 0)
        ERROR("ERROR WHILE CLOSING CONNECTION WITH PEER\n");

    return ;
}

// function that checks if the peer directory has the desired file.
int CHECK(int socketToPeer, char *fileName)
{
    //Sending File Request to the Peer
    char request[100];
    sprintf(request, "REQUEST FILE by CLIENT :%s", fileName);

    // sending request 
    int count = send(socketToPeer, request, strlen(request), 0);
    printf("MESSAGE SENT TO PEER: %s\n", request);

    if (count != strlen(request))
        ERROR("ERROR IN SENDING REQUEST TO PEER");
    
    char buffer[256];
    bzero(buffer, 256);
    //receiving the response message and storing in buffer
    count = recv(socketToPeer, buffer, 255, 0);

    if (count < 0)
        ERROR("ERROR WHILE RECEIVING PEER RESPONSE\n");

    //if file not found
    if (strcmp(buffer, "FILE NOT FOUND") == 0) 
    {
        printf("FILE NOT FOUND AT THIS PEER.\n\n");
        count = close(socketToPeer);

        if (count < 0)
            ERROR("ERROR WHILE CLOSING CONNECTION WITH PEER\n");

        return 0;
    }

    // if file found
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

// function to connect to socket
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
