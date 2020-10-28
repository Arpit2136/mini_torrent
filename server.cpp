// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<pthread.h>
#include<bits/stdc++.h>

#define PORT 8080 
using namespace std;

int number_of_clients=0;

int server_fd, new_socket, valread;

struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 

char buffer[1024] = {0}; 

char *hello = "Hello from server"; 
string s="message recieved";


//returns true if connection to client socket is not present else false
// bool is_connection_off(int client_socket)
// {



// } 


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


void start_server_code()
{




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
    address.sin_port = htons( PORT ); 
    
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


}



void check_requests_as_server()
{
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

int main(int argc, char const *argv[]) 
{ 




    start_server_code();
    cout<<"Server port "<<address.sin_port <<"\n";


    check_requests_as_server();


    return 0; 
} 