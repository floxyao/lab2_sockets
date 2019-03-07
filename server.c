//===============================================================================================
//HW 2:     Socket Programming
//Author:   Florence Yao
//Course:   CECS-327
//Date:     2-25-19
//Info:     Server side C/C++ program to demonstrate Socket programming. TCP is changed to non-
//          blocking socket and runs a user input process in a separate thread. Sleep is used
//          due to the loop running too fast to type inputs
//===============================================================================================
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#define SENDER_NAME "server: "
#define PORT 8080
#define NUM_THREADS 2
#define MSG_LEN 100
#define BUF_SIZE 1024

//GLOBAL
int     new_socket;
struct  sockaddr_in address;
int     addrlen = sizeof(address);
char    msg[MSG_LEN];
char    buffer[BUF_SIZE] = {0};

//==========================================================================
// Concat two strings
//==========================================================================
void concat(char* p, char *q){
   int c, d;
   c = 0;
   while (p[c] != '\0') {
      c++;      
   }
   d = 0;
   while (q[d] != '\0') {
      p[c] = q[d];
      d++;
      c++;    
   }
   p[c] = '\0';
}

//==========================================================================
// User Input Thread
// Gets the message input from user.
//==========================================================================
void* input_thread(void* arg){
    for(;;){
        char label[] = SENDER_NAME;
        fgets(msg, MSG_LEN, stdin);
        concat(label, msg); //concat label to message to identify sender
        strcpy(msg, label);
    }                                      
    pthread_exit(NULL);
    return NULL;
}

//==========================================================================
// Send/Receive Socket
// Sends and receives messages to client. 
// *note sleep() is used because without it, the loop appears to be too fast
// and messages aren't being sent correctly
//==========================================================================
void* message_thread(void* arg){
    for(;;){
        int valread = recv( new_socket , buffer, BUF_SIZE, 0);
        send(new_socket , msg , strlen(msg) , 0);
        memset(msg, 0, MSG_LEN);
        printf("%s", buffer);                         //display message
        bzero(buffer, sizeof(buffer));                //flush buffer
        sleep(1);                                     //introduce delay or else loops too fast (?)                                  
    }
    close(new_socket);
    pthread_exit(NULL);
    return NULL; //silence
}

//==========================================================================
// Main connects sockets, creates and join threads
// notes: AF_INET = domain of socket
//        SOCK_STREAM = type of socket (TCP/UDP)
//        0 = default protocol, TCP   */
//==========================================================================
int main(int argc, char const *argv[])
{
    int server_fd;
    /*-------------------------------
     Creating socket file descriptor
    --------------------------------*/
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
    }
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	/*-------------------------------------------------------------------
      Bind - once you have a socket, bind() it to a port on your machine
    -------------------------------------------------------------------*/
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
    }
    else{
        printf("\nConnected!\n\n");
    }

    /*-------------------------------------------------------------------------
      Listen - original socket server_fd listens for more incoming connections
    -------------------------------------------------------------------------*/
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	}

    /*-------------------------------------------------------------
     Accept - someone is waiting on you to accept their connection
     new_socket = the connection between you and the client
    --------------------------------------------------------------*/
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen)) < 0)
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	}

    /*----------------------------------
     Change the socket to non-blocking
    -----------------------------------*/
    fcntl(new_socket, F_SETFL, O_NONBLOCK);
    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    /*--------------------
     Create/Join threads
    ---------------------*/
    int rc;
    pthread_t threads[NUM_THREADS];
    //printf("main(): creating input thread \n");
    rc = pthread_create(&threads[0], NULL, &input_thread, NULL);
    if(rc){
        printf("Error: unable to create thread, %d \n", rc);
        exit(-1);
    }
    //printf("main(): creating message thread \n");
    rc = pthread_create(&threads[1], NULL, &message_thread, NULL);
    if(rc){
        printf("Error: unable to create thread, %d \n", rc);
        exit(-1);
    }
    for(int i = 0; i < NUM_THREADS; i++){
        rc = pthread_join(threads[i], NULL);
        if(rc){
            printf("Joining Thread Error: %d \n", rc);
        }
    }

    return 0;
}
