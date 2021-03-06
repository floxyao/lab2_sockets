//===============================================================================================
//HW 2:     Socket Programming
//Author:   Florence Yao
//Course:   CECS-327
//Date:     2-25-19
//Info:     Client side C/C++ program to demonstrate Socket programming. TCP is changed to non-
//          blocking socket and runs a user input process in a separate thread. Sleep is used
//          due to the loop running too fast to type inputs
//===============================================================================================
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#define SENDER_NAME "client: "
#define IP_ADDR "127.0.0.1"
#define PORT 8080 
#define NUM_THREADS 2
#define MSG_LEN 100
#define BUF_SIZE 1024

//GLOBAL
int       sock;
struct    sockaddr_in client_addr;
struct    sockaddr_in serv_addr;     
socklen_t CLADDR_LEN = sizeof(client_addr);
int       inet_pton(); //get rid of warning
char      msg[MSG_LEN];
char      buffer[BUF_SIZE] = {0};

//==========================================================================
// Concatenation of two strings
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
        int valread = recv( sock , buffer, BUF_SIZE, 0);
        send(sock , msg , strlen(msg) , 0);
        memset(msg, 0, MSG_LEN);                      //clear message
        printf("%s",buffer);                          //display message
        bzero(buffer, sizeof(buffer));                //flush buffer
        sleep(1);                                     //introduce delay or else loops too fast (?) 
    }
    close(sock); 
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
    /*---------------------------------
     Creating socket file descriptor
    ----------------------------------*/
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	}
    
	memset(&serv_addr, '0', sizeof(serv_addr));
    
    /*---------------------------------------------------------------------
     Specify address for socket to connect to
     Specify family of address so it knows what type of address
     Specify port; htons() converts the PORT to correct data format
     so structure understands the port number and where we need connect to
    ----------------------------------------------------------------------*/
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    /*----------------------------------------------------------
	 Convert IPv4 and IPv6 addresses from text to binary form
     serv_addr = struct holding info about address
     sin_addr is a struct itself containing the address itself
    -----------------------------------------------------------*/
    if(inet_pton(AF_INET, IP_ADDR, &serv_addr.sin_addr)<=0)
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

    /*-------------------------------------------------------
     Connect to the socket
     sock      = our socket
     serv_addr = cast our address to the right struct type
     size      = size of addr
     Returns an integer to indicate success/failure
    -------------------------------------------------------*/
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
    }
    else{
        printf("\nConnection Established");
        printf("\nIP: %s",IP_ADDR);
        printf("\nPORT: %d\n\n",PORT);
    }
    
    /*-----------------------------------
     Change the socket to non-blocking
    -----------------------------------*/
    fcntl(sock, F_SETFL, O_NONBLOCK);
    
    /*---------------------
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
    //join threads
    for(int i = 0; i < NUM_THREADS; i++){
        rc = pthread_join(threads[i], NULL);
        if(rc){
            printf("Joining Thread Error: %d \n", rc);
        }
    }
    
    
    return 0;
}
