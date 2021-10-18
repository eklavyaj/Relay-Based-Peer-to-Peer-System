# Relay Based Peer-to-Peer System 
![image](https://user-images.githubusercontent.com/50804314/137678852-a4a077a1-f11b-43dd-8a77-ece0bb88c6ec.png)

In this application, the aim is to implement a simple Relay based Peer-to-Peer System.

Initially, the Peer_Nodes (peer 1/2/3 as shown in Figure 1) will connect to the Relay_Server using the TCP port already known to them. After successful connection, all the Peer_Nodes provide their information (IP address and PORT) to the Relay_Server and close the connections (as shown in Figure 1). The Relay_Server actively maintains all the received information with it. Now the Peer_Nodes will act as servers and wait to accept
connection from Peer_Clients (refer phase three).

In second phase, the Peer_Client will connect to the Relay_Server using the server’s TCP port already known to it. After successful connection; it will request the Relay_Server for active Peer_Nodes information (as shown in Figure 2). The Relay_Server will response to the Peer_Client with the active Peer_Nodes information currently having with it. On receiving the response message from the Relay_Server, the Peer_Client closes the connection
gracefully.

In third phase, a set of files (say, *.txt) are distributed evenly among the three Peer_Nodes. The Peer_Client will take “file_Name” as an input from the user. Then it connects to the Peer_Nodes one at a time using the response information. After successful connection, the Peer_Client tries to fetches the file from the Peer_Node. If the file is present with the Peer_Node, it will provide the file content to the Peer_Client and the Peer_Client will print the
file content in its terminal. If not, Peer_Client will connect the next Peer_Node and performs the above action. This will continue till the Peer_Client gets the file content or all the entries in the Relay_Server Response are exhausted (Assume only three/four Peer_Nodes in the system).


## Directory Tree:

```
MAIN
│   README.md
|   
│
└───output  //images of terminal output, requested for "sample2.txt", i.e., available at 
|
│
└───server
│   │   server.c        // server code 
│   │   peerInfo.txt    // File created on running server, stores Peer Information. 
│   │   server          // output file 
│      
└───peers
│   │
│   └───peer1
│   │   │   peer.c       // peer Node code 
│   │   │   sample1.txt  // File located at peer Node 1 
│   │   │   peer         // output file 
│   │
│   └───peer2
│   │   │   peer.c       // peer Node code  
│   │   │   sample2.txt  // File located at peer Node 2 
│   │   │   peer         // output file 
│   │   
│   └───peer3
│       │   peer.c       // peer Node code 
│       │   sample3.txt  // File located at peer Node 2 
│       │   peer         // output file 
│      
│   
└───client
    │   client.c          // peer Client code 
    │   client            // output file 
    │   peerInfo2.txt     // Stores Information of peer nodes on peer client machine 
    │   DesiredFile.txt   // Stores the content of the file found at peer Node>
```

## Prototypes for the Files:
```
 Client : <executable code> <Server IP address> <Server Port Number>
 Server : <executable code> <Server IP address> 
 Peer(s): <executable code> <Server IP address> <Server Port Number>
```

## Procedure to run the application:
To run the Application do the following steps:
<ol>
<li>Open 5 terminal windows- one for server, one for client and three for peers in respective directories.</li> 
<li>Start SERVER on the terminal with working directory as 'server'.</li>
<li>Start PEER NODE 1 on the terminal with working directory as 'peer1'.</li>
<li>Start PEER NODE 2 on the terminal with working directory as 'peer2'.</li>
<li>Start PEER NODE 3 on the terminal with working directory as 'peer3'.</li>
<li>Start PEER CLIENT on the terminal with working directory as 'client'.</li>
<li> Client terminal window is going to ask for the name of the file that is to be sought, input the derired file name. </li>
<li> If the file is found, a text file "DesiredText.txt" containing the contents of the sought file will be saved in the directory of the client file. </li>
</ol>

```
//server
gcc server.c -o server 
./server 8000 
```
```
//peer1
gcc peer.c -o peer1  
./peer 127.0.0.1 8000 
```
```
//peer2
gcc peer.c -o peer2 
./peer 127.0.0.1 8000 
```
```
//peer3
gcc peer.c -o peer3 
./peer 127.0.0.1 8000 
```
```
//client
gcc client.c -o client 
./client 127.0.0.1 8000 
```


