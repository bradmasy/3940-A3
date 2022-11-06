// #include "Socket.hpp"
// #include "ServerSocket.hpp"
#include "Thread.hpp"
#include "ServerThread.hpp"
#include "ServerSocket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <stddef.h>
#include <sys/socket.h>
#include <sys/dir.h>
#include <resolv.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
// #include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include "ClientServlet.hpp"


using namespace std;

/**
    main function.
*/
int main() {
    // ClientServlet *client = new ClientServlet();
    // client->getUserInput();
    // client->POSTRequest();

    struct sockaddr_in server;
    int msgsock;
    char buf[1024];
    //struct hostent *host;
    char *host = "127.0.0.1"; // local host address
    int rval;



    // socket creation
    // int listening = socket(AF_INET,SOCK_STREAM,0);
    // if(listening == -1){
    //     std::cerr<<"Cant create a scocket";
    //     return -1;
    // }else {
    //     cout << "Socket Created..."<<endl;
    // }


    // server.sin_family = AF_INET;
    // server.sin_addr.s_addr = INADDR_ANY;
    // server.sin_port = htons(8888);
   // server.sin_port = htons(54000); // can try this too if theres errors, refers to endianess
    //inet_pton(AF_INET,"0.0.0.0", &server.sin_addr);

    // int bound = bind(listening, (struct sockaddr*)&server, sizeof(server)) ;
    // cout << "bound: " << bound <<endl;

    // if(bound == - 1){
    //     perror("Cannot Bind");
    //     return -2;
    // }

    // listen(listening, 5);

    ServerSocket *socket = new ServerSocket(8889); // socket creation.

    while(socket != NULL){          
        Socket *currentSocket = socket -> Accept(); 
        ServerThread* serverThread = new ServerThread(currentSocket);
        serverThread->readSocket();
        // serverThread -> run();

        cout << "MAIN: AFTER RUN" << endl;

        Request* request   = serverThread -> getRequest(); /// serVerthread return req
        cout << "MAIN: AFTER REQUEST" << endl;
        Response* response = serverThread -> getResponse();

        cout << "MAIN: BEFORE SEND BACK" << endl;

        string responseStr = response-> sendBack("");
        cout << "MAIN: AFTER SEND BACK" << endl;
        char* resPtr       = const_cast<char *>(responseStr.c_str());
        currentSocket -> sendResponse(resPtr);
        
        close(currentSocket->getSocket());
    }
    close(socket->getPort());


cout<<"here"<<endl;
    return 0;
}

