// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include<pthread.h>
#include<errno.h>
#include<bits/stdc++.h>

#define PORT 8080
using namespace std;

int main(int argc, char const *argv[]) 
{ 
	// cout<<typeid(PORT).name()<<"\n";
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char *hello = "Hello from client1"; 
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 


	// cout<<argc<<"\n";
	// for(int i=0;i<argc;i++)
	// {
	// 	cout<<argv[i]<<"\n";
	// }
	

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	struct sockaddr_in local_address;
	// socklen_t a=sizeof(sockaddr_in);
	socklen_t *addr_size=(socklen_t *)malloc(sizeof(socklen_t));
	*addr_size = sizeof(local_address);
	int gs=getsockname(sock,(struct sockaddr *)&local_address, addr_size);
	if(gs==-1)
	{
		cout<<(errno);
	}
	string s;
	cout<<"return value of getsockname "<<gs<<"\n";
	cout<<"port number of client "<<local_address.sin_port<<"\n";
	cout<<"port number of server "<<serv_addr.sin_port<<"\n";

	while(true)
	{

		cin>>s;
		hello=&s[0];
		send(sock , hello , strlen(hello) , 0 );
		// hello=NULL; 
		// printf("Hello message sent\n"); 
		// valread = read( sock , buffer, 1024); 
		// printf("%s\n",buffer ); 
		// buffer[1024]={0};
		
	}
	
	return 0; 
} 
