// Client side C/C++ program to demonstrate Socket programming using multithreads
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include<pthread.h>
#include<errno.h>

#define PORT 8080
#define NUM_THREADS 2

//Function signatures
void* receive_thread(void *arg);
void* send_thread(void *arg);


//GLOBAL VARIABLES
char msg[50];
char buffer[1024] = {0};
// socklen_t CLADDR_LEN = sizeof(client_addr);
int sock = 0, valread;


int main(int argc, char const *argv[])
{
	struct sockaddr_in client_addr;
	struct sockaddr_in serv_addr;     //This structure contains fields address and port
	int inet_pton();                  //get rid of warning
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

		//===================================================
    // Send/Receive threads
    //===================================================
		int rc;
		pthread_t threads[NUM_THREADS]; //array with pthread objects

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
				printf("main(): creating receive_thread thread \n");
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

	    valread = read( sock , buffer, 1024);
			// perror(errno);//check for error
			if(valread < 0){
					perror("Error: ");//check for error
			}

			//print buffer
		  printf("\nServer: %s\n",buffer);

		  //clean buffer
		  bzero(buffer, sizeof(buffer));
		  //add exit condition



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
		// printf("Message to send: ");
		scanf (" %[^\n]%*c", msg);

		if(msg[0] == ' '){
			printf("Msg length: %ld", strlen(msg));
			printf("Empty Message");
		}
		else{
			sc = send(sock , msg , strlen(msg) , 0 );
			if(sc < 0){
				perror("Error: ");
			}
			else{
				printf("Message sent \n");

			}
		}


	}
}
