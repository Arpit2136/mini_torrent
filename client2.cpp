// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include<pthread.h>
#include<bits/stdc++.h>
#include<cstdlib>
#include<errno.h>

// #define PORT 0

using namespace std;

//server thread variables

int client_as_server_port=0;
int number_of_clients=0;
int server_fd, new_socket, valread;

struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 

char buffer[1024] = {0}; 
pthread_t client_as_server,client_as_client;


void *communicate(void *a);
void check_requests_as_server();
void make_server_code();
void* open_server(void *a);
void* connect_client(void *a);
void start_server_thread();
void start_client_thread(int *a);




void* communicate(void *p_client)
{
	cout<<"in func\n";
	int current_client_number;
	current_client_number=number_of_clients;
	int client=*((int *)p_client);
	free(p_client);
	cout<<"client number "<<number_of_clients<<"\n";

	while(1)
	{

        // if(check_current_connection(client))
        // {
        //     break;
        // }
		cout<<"in while\n";  
		read( client , buffer, 1024);
		if(read==0)
		{
			break;
		} 
		printf("%s",buffer);
		memset(buffer, 0, sizeof(buffer));
		cout<<"  this message if from "<<current_client_number<<"\n";
    // continue;

    // send(client , hello , strlen(hello) , 0 );
    // hello=&s[0];
    // printf("message recieved from client number ");
    // cout<<current_client_number<<"\n";
    // buffer[1024] = {0}; 
	}

	return NULL;
}



void check_requests_as_server()
{

	cout<<"inside check_requests_as_server\n";
	while(true)
	{

		cout<<"Waiting for connections\n";
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
			(socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 
		else
		{
			number_of_clients++;
		}
		cout<<"Connection established at "<<ntohs(address.sin_port)<<"\n";
		int *p_client=(int *)malloc(sizeof(int));
		*p_client=new_socket;
		pthread_t t1;
		pthread_create(&t1,NULL,communicate,p_client);






	}
}




void make_server_code()
{

	// client_as_server_port=8081;
	if(client_as_server_port==0)
	{
		cout<<"invalid port\n";
		return;
	}
	cout<<"inside make_server_code\n";
    // Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 

    // Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
		&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( client_as_server_port ); 

    // Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
		sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 


	check_requests_as_server();


}
void* open_server(void *args)
{

	cout<<"inside open server\n";
	make_server_code();

	return NULL;

}



void* connect_client(void* server_to_connect)
{
	int serv=*((int*)server_to_connect);
	free(server_to_connect);
	cout<<serv<<"\n";
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char *hello = "Hello from client1"; 
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return NULL; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(serv); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return NULL; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		cout<<errno<<"\n";
		printf("\nConnection Failed \n"); 
		return NULL; 
	} 
	cout<<"client con succ\n";
	// cout<<"port number "<<serv_addr.sin_port<<"\n";
	struct sockaddr_in local_address;
	// socklen_t a=sizeof(sockaddr_in);
	socklen_t *addr_size=(socklen_t *)malloc(sizeof(socklen_t));
	*addr_size = sizeof(local_address);
	int gs=getsockname(sock,(struct sockaddr *)&local_address, addr_size);
	cout<<"return value of getsockname "<<gs<<"\n";
	if(gs==-1)
	{
		cout<<errno<<"\n";
	}
	string s;
	client_as_server_port=local_address.sin_port;
	start_server_thread();
	cout<<"port number of client as server "<<local_address.sin_port<<"\n";
	// cout<<"port number of server "<<serv_addr.sin_port<<"\n";
	// string s;

	while(true)
	{



		cin>>s;
		hello=&s[0];
		send(sock , hello , strlen(hello) , 0 );
		// hello=NULL; 
		// printf("Hello message sent\n"); 
		cout<<"waiting for server message\n";
		// valread = read( sock , buffer, 1024); 
		// printf("%s\n",buffer ); 
		buffer[1024]={0};
		
	}
	

	// return NULL;



	return NULL;
}

void start_server_thread()
{

	cout<<"inside server thread\n";
	
	pthread_create(&client_as_server,NULL,open_server,NULL);
	// pthread_join(client_as_server,NULL);

}



void start_client_thread(int to_connect_server)
{	
	int *server_to_connect=(int *)malloc(sizeof(int));
	*server_to_connect=to_connect_server;
	// free(to_connect_server);
	
	pthread_create(&client_as_client,NULL,connect_client,server_to_connect);
	// pthread_join(client_as_client,NULL);

}
int main(int argc, char const *argv[]) 
{ 
	cout<<"connect "<<client_as_server_port<<"\n";


	// if(argc==1)
	// {
	// 	start_server_thread();
	// }


	// if(argc>1)
	// {
	int server_port;
		// =(int*)malloc(sizeof(int));

	server_port=atoi(argv[1]);
	start_client_thread(server_port);
	
	pthread_join(client_as_client,NULL);
	pthread_join(client_as_server,NULL);
	// }
	
	return 0; 
} 
