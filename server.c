// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<pthread.h>
#include<errno.h>

#define NUM_THREADS 2
#define PORT 8080

// using namespace std;

//Function signatures
void* receive_thread(void *arg);
void* send_thread(void *arg);

//global variables
int server_fd, new_socket, valread;
//server_fd integer to hold socket descriptor it contains information about our socket
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char msg[50];
//char buffer[1024] = {0};
char buffer[1024] = {0};
// char *hello = "Hello from server";



int main(int argc, char const *argv[])
{

	pthread_t threads[NUM_THREADS];//array with pthread objects

    //===================================================
  	// Creating socket file descriptor
    //===================================================
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    //AF_INET = domain of socket
    //SOCK_STREAM = type of socket (TCP/UDP-datagram)
    //0 = default protocol, TCP
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

    //===================================================
    // SetSockOpt
    // Forcefully attaching socket to the port 8080
    //===================================================
//    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
//                                                &opt, sizeof(opt)))
//    {
//        perror("setsockopt");
//        exit(EXIT_FAILURE);
//    }
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );


    //===================================================
    // Bind
    // Forcefully attaching socket to the port 8080
    //===================================================
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
    }
    else{
        printf("\nConnected!\n\n");
    }
    //===================================================
    // Listen
    //===================================================
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

    //===================================================
    // Accept
    //===================================================
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

    //===================================================
    // Send/Receive
    //===================================================

		int rc;

		//create threads
		for(int i = 0; i < NUM_THREADS; i++){
			if(i == 0){
				printf("main(): creating send thread \n");
				rc = pthread_create(&threads[i], NULL, &send_thread, NULL);
				if(rc){
					printf("Error: unable to create thread, %d \n", rc);
					exit(-1);
				}
			}
			else{
				printf("main(): creating Receive thread \n");
				rc = pthread_create(&threads[i], NULL, &receive_thread, NULL);
				if(rc){
					printf("Error: unable to create thread, %d \n", rc);
					exit(-1);
				}
			}
		}

		//join threads
		for(int i = 0; i < NUM_THREADS; i++){
			rc = pthread_join(threads[i], NULL);
			if(rc){
				printf("Joining Thread Error: %d \n", rc);
			}
		}

    // for(;;){
    //     //wait for message in buffer (wait for message)
		//
    //     printf("client: ");
    //     valread = read( new_socket , buffer, 1024);
		//
		// 		printf("valread: %d\n", valread);
		//
    //     //print buffer
    //     printf("%s\n",buffer);
		//
    //     //put message in buffer (send message to client)
    //     //printf(":");
    //     //scanf ("%[^\n]%*c", msg);
    //     //send(new_socket , msg , strlen(msg) , 0 );
		//
    //     //clean buffer
    //     bzero(buffer, sizeof(buffer));
    //     //add exit condition
    // }

		// for(;;){//testing purposes
    //     //wait for message in buffer (wait for message)
		// 		if(){
    //     printf("client: ");
    //     valread = read( new_socket , buffer, 1024);
		//
		// 		printf("valread: %d\n", valread);
		//
    //     //print buffer
    //     printf("%s\n",buffer);
		// 	}
		// 	else(){
		//
    //     //put message in buffer (send message to client)
    //     //printf(":");
    //     //scanf ("%[^\n]%*c", msg);
    //     //send(new_socket , msg , strlen(msg) , 0 );
		// 	}

        //clean buffer
        // bzero(buffer, sizeof(buffer));
        //add exit condition
    // }

    //valread = read( new_socket , buffer, 1024);
    //printf("%s\n",buffer );
    //send(new_socket , hello , strlen(hello) , 0 );
    //printf("Hello message sent\n");
	return 0;
}

void* receive_thread(void *arg){
	for(;;){
	    //wait for message in buffer (wait for message)

	    // printf("\nclient: ");
	    valread = read( new_socket , buffer, 1024);
			if(valread < 0){
					perror("Error: ");//check for error
			}

			if(valread == 0){
				sleep(1);
			}
			else{
				// printf("valread: %d\n", valread);

		    //print buffer
		    printf("\nClient: %s\n",buffer);
				// printf("Message to send: ");

		    //put message in buffer (send message to client)
		    //printf(":");
		    //scanf ("%[^\n]%*c", msg);
		    //send(new_socket , msg , strlen(msg) , 0 );

		    //clean buffer
		    bzero(buffer, sizeof(buffer));
		    //add exit condition
			}

	}
}

void* send_thread(void *arg){
	int sc;
	for(;;){
		printf("Message to send: ");
		scanf (" %[^\n]%*c", msg);

		// if(msg[0] == '\0'){
		// 	printf("Empty Message");
		// }
		if(msg[0] == ' '){
			printf("msg length: %ld", strlen(msg));
			printf("Empty Message");
		}
		else{
			sc = send(new_socket , msg , strlen(msg) , 0 );
			if(sc < 0){
				perror("Error: ");
			}
			else{
				printf("Message sent \n");
				// bzero(msg, sizeof(msg));
			}
		}

	}
}
