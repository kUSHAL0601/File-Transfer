#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;  // sockaddr_in - references elements of the socket address. "in" for internet
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// This is to lose the pesky "Address already in use" error message
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
				&opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;  // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc. 
	address.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address - listens from all interfaces.
	address.sin_port = htons( PORT );    // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
				sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// Port bind is done. You want to wait for incoming connections and handle them in some way.
	// The process is two step: first you listen(), then you accept()
	printf("Server Listening....\n");
	while(1)
	{
		if (listen(server_fd, 5) < 0) // 3 is the maximum size of queue - connections you haven't accepted
		{
			perror("can't listen");
			exit(EXIT_FAILURE);
		}

		// returns a brand new socket file descriptor to use for this single accepted connection. Once done, use send and recv
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
						(socklen_t*)&addrlen))<0)
		{
			perror("not acceptable");
			exit(EXIT_FAILURE);
		}
		char ip[100]={};
		inet_ntop(AF_INET, &(address.sin_addr), ip, INET_ADDRSTRLEN);
		printf("Connection established with %s\n",ip);
		valread = read( new_socket , buffer, 1024);  // read infromation received into the buffer
		if(strcmp(buffer,"CLOSE")==0){
			printf("%s Program closed\n",ip);
			close(new_socket);
			continue;
		}
		printf("%s\n",buffer );
		FILE * fptr=fopen(buffer,"r");
		if(fptr==NULL)
		{
			write(new_socket,"ERROR",5);
			perror("File doesn't exist");
		}
		else
		{
			char ch[10]={};
			while(fscanf(fptr,"%c",ch)!=EOF)
			{
				printf("%s",ch);
				//send(new_socket,"H",1,0);
				write(new_socket , ch , sizeof(ch) );
			}
			write(new_socket , "\0" , sizeof("\0") );
			printf("File %s sent successfully\n",buffer);
			fclose(fptr);

		}
		printf("%s disconnected\n",ip);
		close(new_socket);
	}
	return 0;
}
