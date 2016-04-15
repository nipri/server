// code for a server waiting for connections
// namely a stream socket on port 3490, on this host's IP
// either IPv4 or IPv6.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

void main (void) {
	    int socket_desc , client_sock , c , read_size;
	    struct sockaddr_in server , client;
	    char client_message[2000];
	    int server_message[5];

//	    strcpy(server_message, "You are most welcome!\r\n");
//	    strcpy(server_message, "abcde");
	    server_message[0] = 0x0a;
	    server_message[1] = 0x10;

	    //Create socket
	    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	    if (socket_desc == -1)
	    {
	        printf("Could not create socket");
	    }
	    puts("Socket created");

	    //Prepare the sockaddr_in structure
	    server.sin_family = AF_INET;
	    server.sin_addr.s_addr = INADDR_ANY;
	    server.sin_port = htons( 8888 );

	    //Bind
	    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	    {
	        //print the error message
	        perror("bind failed. Error");
	        return 1;
	    }
	    puts("bind done");

	    //Listen
	    listen(socket_desc , 3);

	    //Accept and incoming connection
	    puts("Waiting for incoming connections...");
	    c = sizeof(struct sockaddr_in);

	    //accept connection from an incoming client
	    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	    if (client_sock < 0)
	    {
	        perror("accept failed");
	        return 1;
	    }
	    puts("Connection accepted");

	    	//Receive a message from client
	    	while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
	    	{
	    		printf("Client Sent: %s\r\n", client_message);
	    		//Send the message back to client
	    		write(client_sock, server_message , 5);
	    	}

	    	if(read_size == 0)
	    	{
	    		puts("Client disconnected");
	    		fflush(stdout);
	    	}
	    	else if(read_size == -1)
	    	{
	    		perror("Server recv failed");
	    	}

	    return 0;
	}
