// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <unistd.h>
#define PORT 8080 

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in client_addr;
    socklen_t CLADDR_LEN = sizeof(client_addr);
	int sock = 0, valread;
    
    int inet_pton();                  //get rid of warning
    struct sockaddr_in serv_addr;     //This structure contains fields address and port

    
    //===================================================
    // Creating socket file descriptor
    // Creates one of the end points needed for communication
    // AF_INET     = domain of socket
    // SOCK_STREAM = type of socket (TCP/UDP-datagram)
    // 0           = default protocol, TCP
    //===================================================
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr));
    
    //===================================================
    // Specify address for socket to connect to
    // Specify family of address so it knows what type of address
    // Specify port; htons() converts the PORT to correct data format
    // so structure understands the port number and where we need connect to
    //===================================================
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    //===================================================
	// Convert IPv4 and IPv6 addresses from text to binary form
    // serv_addr = struct holding info about address
    // sin_addr is a struct itself containing the address itself
    //===================================================
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

    //===================================================
    // Connect to the socket
    // sock      = our socket
    // serv_addr = cast our address to the right struct type
    // size      = size of addr
    // Returns an integer to indicate success/failure
    //===================================================
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
    }
    else{
        printf("\nConnection Established\n\n");
    }
    
//    char server_response[256];
//    recv(sock, &server_response, sizeof(server_response), 0);
    
//    else{
//        printf("\nConnection Established\n");
//
//        printf("IP address is: %s\n", inet_ntoa(client_addr.sin_addr));
//        printf("port is: %d\n", (int) ntohs(client_addr.sin_port));
//        //inet_ntop(AF_INET, (struct sockaddr_in *)&(client_addr.sin_addr), client_addr, CLADDR_LEN);
//        //char *c = inet_ntop(AF_INET, (struct sockaddr_in *)&client_addr, &serv_addr, CLADDR_LEN);
//        //printf("IP: %d", *c);
//        //printf("\nPort: %d", PORT);
//    }
    
    //char *hello = "Hello from client";
    //char msg[100];
    char msg[50];
    char buffer[1024] = {0};
    //char buffer[1024] = {0};
    
    
    for(;;){
        //put message in buffer (send message to server)
        printf(":");
        scanf ("%[^\n]%*c", msg);
        
        send(sock , msg , strlen(msg) , 0 );
        
//        if((len = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
//            buffer[len] = '\0';
//            fputs(buffer, stdout);
//        }
        
        //wait for message in buffer (wait for response)
        //printf("server: ");
        //valread = read( sock , buffer, 1024);
        
        //print buffer
        //printf("%s\n",buffer );
        
        //clean buffer
        bzero(buffer, sizeof(buffer));
        //add exit condition
    }
	//send(sock , hello , strlen(hello) , 0 );
	//printf("\nHello message sent\n");
	//valread = read( sock , buffer, 1024);
	//printf("%s\n",buffer );
	return 0; 
}