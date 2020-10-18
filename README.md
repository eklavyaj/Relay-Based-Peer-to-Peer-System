DIRECTORY TREE:

```
Group_49
│   README.md
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
```
Prototypes for the Files:

 Client : <executable code> <Server IP address> <Server Port Number>
 Server : <executable code> <Server IP address> 
 Peer(s): <executable code> <Server IP address> <Server Port Number>
```

To run the Application do the following steps:
<ol>
<li>Open 5 terminal windows- one for server, one for client and three for peers in respective directories.</li> <br>
<li>Start SERVER on the terminal with working directory as 'server' : <br>
    gcc server.c -o server <br>
    ./server 8000 <br>
    
</li>
<li>Start PEER NODE 1 on the terminal with working directory as 'peer1' : <br>
    gcc peer.c -o peer1  <br>
    ./peer 127.0.0.1 8000 <br>
</li>
<li>Start PEER NODE 2 on the terminal with working directory as 'peer2' : <br>
    
    gcc peer.c -o peer2 <br>
    ./peer 127.0.0.1 8000 <br>
    
</li>
<li>Start PEER NODE 3 on the terminal with working directory as 'peer3' : <br>
    
    gcc peer.c -o peer3 <br>
    ./peer 127.0.0.1 8000 <br>
    
</li>
<li>Start PEER CLIENT on the terminal with working directory as 'client' : <br>
    gcc client.c -o client <br>
    ./client 127.0.0.1 8000  <br>
</li>



</ol>