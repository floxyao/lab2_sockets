// Server side C/C++ program to demonstrate Socket programming using multithreads
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

//Function signatures
void* receive_thread(void *arg);
void* send_thread(void *arg);


//global variables
int server_fd, new_socket, valread;
//server_fd integer to hold socket descriptor it contains information about our socket
// int opt = 1;
char msg[50];
char buffer[1024] = {0};


int main(int argc, char const *argv[])
{
	struct sockaddr_in address;
	int addrlen = sizeof(address);

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
    // Send/Receive threads
    //===================================================

		int rc;

		//create threads
		for(int i = 0; i < NUM_THREADS; i++){
			if(i == 0){ //create sending thread
				printf("main(): creating send thread \n");
				rc = pthread_create(&threads[i], NULL, &send_thread, NULL);

				if(rc){
					printf("Error: unable to create thread, %d \n", rc);
					exit(-1);
				}
			}
			else{ //create receiving thread
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


	return 0;
}


//===============================================================================================
//function:    receive_thread()
//input(s):    arg
//output(s):   none
//description: This function specifies what the receiving thread will be doing
//===============================================================================================
void* receive_thread(void *arg){
	for(;;){
	    //wait for message in buffer (wait for message)

	    valread = read( new_socket , buffer, 1024);
			if(valread < 0){
					perror("Error: ");//check for error
			}

		  //print buffer
		  printf("\nClient: %s\n",buffer);

		  //clean buffer
		  bzero(buffer, sizeof(buffer));
		  //add exit condition
		// }

	}
}

//===============================================================================================
//function:    send_thread()
//input(s):    arg
//output(s):   none
//description: This function specifies what the sending thread will be doing
//===============================================================================================
void* send_thread(void *arg){
	int sc;
	for(;;){

		scanf (" %[^\n]%*c", msg);

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

			}
		}

	}
}
