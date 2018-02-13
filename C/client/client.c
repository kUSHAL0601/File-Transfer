// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
	while(1)
	{
		struct sockaddr_in address;
		int sock = 0, valread;
		struct sockaddr_in serv_addr;
		//	char *file_name = "a.txt";
		char file_name[1000]={};
		char buffer[1024] = {0};
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			printf("\n Socket creation error \n");
			return -1;
		}

		memset(&serv_addr, '0', sizeof(serv_addr)); // to make sure the struct is empty. Essentially sets sin_zero as 0
		// which is meant to be, and rest is defined below

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

		// Converts an IP address in numbers-and-dots notation into either a 
		// struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
		if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
		{
			printf("\nInvalid address/ Address not supported \n");
			return -1;
		}

		if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  // connect to the server address
		{
			printf("\nConnection Failed \n");
			return -1;
		}
		printf("Enter file name:");
		scanf("%s",file_name);
		if(strcmp(file_name,"q")==0)
		{
			send(sock,"CLOSE",5,0);
			close(sock);
			return -1;
		}

		else{
			send(sock , file_name , strlen(file_name) , 0 );  // send the message.
			//printf("Hello message sent\n");
			int flag=0;
			FILE * dest=fopen(file_name,"w");
			while(1)
			{
				valread = read( sock , buffer, 10);  // receive message back from server, into the buffer
				if(strcmp(buffer,"ERROR")==0){flag=1;break;}
				fprintf(dest,"%s",buffer );
				//printf("%s",buffer );
				if(strcmp(buffer,"\0")==0)break;
			}
			fclose(dest);
			if(flag==0)
				printf("File transmission completed @ %s\n",file_name);
			else
				printf("Such a file doesn't exsist at server side\n");
			close(sock);
		}
	}
	return 0;
}
