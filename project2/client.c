#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define PORT 1234
#define MAXDATASIZE 1024

char sendbuf[1024]; 
char recvbuf[1024]; 
char name[100];    
int fd;             

void* pthread_recv(void *ptr)
{
    while (1)
    {
        if ((recv(fd, recvbuf, MAXDATASIZE, 0)) == -1) //receive socket loss
        {
            printf("recv() error\n");
            exit(1);
        }
        printf("%s", recvbuf);               //show up receive message
        memset(recvbuf, 0, sizeof(recvbuf)); //reset message
    }
}

int main(int argc, char *argv[])
{
    //variable set up
    int numbytes;
    struct hostent *h;
    struct sockaddr_in server;

    if (argc != 2) //users don't input IP
    {
        printf("Usage: %s <IP Address>\n Please input address\n", argv[0]);
        exit(1);
    }
	
    if ((h = gethostbyname(argv[1])) == NULL) //address error
    {
        printf("%s\n", argv[1]);
        printf("gethostbyname() error\n");
        exit(1);
    }

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // setup socket，AF_INET: IPV4，SOCK_STREAM: TCP
    {
        printf("socket() Error\n");
        exit(1);
    }

    bzero(&server, sizeof(server)); // all server parmeter buff set to zero

    server.sin_family = AF_INET;    // IPV4
    server.sin_port = htons(PORT);                     
    server.sin_addr = *((struct in_addr *)h->h_addr); 

    if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) //connect server socket
    {
        printf("Connect() error, wrong size\n"); //correct input, but wrong size
        exit(1);
    }

    printf("Connect SUCCESS\n");
    char str[] = " Enter the chatroom\n";
    printf("Please enter User name：");
    fgets(name, sizeof(name), stdin); //save user name
    name[0] = toupper(name[0]);//first letter change to uppercase
    send(fd, name, (strlen(name) - 1), 0);
    send(fd, str, (strlen(str)), 0); //send message to server, and server to share the message

    pthread_t tid;                                  //thread ID
    pthread_create(&tid, NULL, pthread_recv, NULL); //set up new thread to get message

    while (1)
    {
        memset(sendbuf, 0, sizeof(sendbuf));    //reset user
        fgets(sendbuf, sizeof(sendbuf), stdin); //save user input
        if (strncmp(sendbuf, "exit", 4) == 0)   //if user input exit, and get out the chatroom
        {
            //send to server and server share all message and it also can tell who walk out the chat
            send(fd, name, (strlen(name) - 1), 0);
            strcpy(sendbuf, " leave group\n");
            send(fd, sendbuf, (strlen(sendbuf)), 0);
            memset(sendbuf, 0, sizeof(sendbuf));
            printf("you have left the group\n");
            send(fd, sendbuf, (strlen(sendbuf)), 0);
            break;
        }
	//send to server and server share to other user. tell other who chat on the room
        send(fd, name, (strlen(name) - 1), 0);
        send(fd, ":", 1, 0);
        send(fd, sendbuf, (strlen(sendbuf)), 0);
    }

    close(fd);
}
