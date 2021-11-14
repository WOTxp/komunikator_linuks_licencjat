#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>

using namespace std;

int main()
{

   //create a socket
   int listening = socket(AF_INET, SOCK_STREAM, 0);
   if(listening == -1)
   {
        cout<<"nie udalo sie utworzyc socketu";
        return -1;
   }
   //Bind the socket to a IP/port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));

   //Tell Winsock the sock is for listening
   listen(listening, SOMAXCONN);


   //Wait for a connection
   sockaddr_in client;
   socklen_t clientSize = sizeof(client);

   int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

   char host[NI_MAXHOST]; //client's remote name
   char service[NI_MAXSERV]; //service the client is connect on

   memset(host, 0, NI_MAXHOST);
   memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV,0) == 0)
    {
        cout << host <<"connected on"<< service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on " << ntohs(client.sin_port) << endl;
    }

   //Close the listening socket
   close(listening);

   //while receiving- display message, echo message
   char buf[4096];

   while(true)
   {
        memset(buf, 0, 4096);

        //Wait for client to send data
        int byteReceived = recv(clientSocket, buf, 4096, 0);
        if(byteReceived == -1)
        {
            cout<<"Error in recv(). Quitting" << endl;
            break;
        }
        if(byteReceived == 0)
        {
            cout<<"Client disconnected"<<endl;
            break;
        }
    cout << string(buf, 0, byteReceived) << endl;

   //Echo message back to client
   send(clientSocket, buf, byteReceived + 1, 0);

   }

   //Close the socket
   close(clientSocket);
    return 0;
}
