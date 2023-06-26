#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>

#define PORT 1234
#define BACKLOG 1
#define Max 10 //maximun member
#define MAXSIZE 1024

int fdt[Max] = {0};
char mes[1024];

void SendToClient(int fd, char *buf, int Size)
{
    int i;
    for (i = 0; i < Max; i++)
    {
        printf("fdt[%d]=%d\n", i, fdt[i]);
        if ((fdt[i] != 0) && (fdt[i] != fd))
        {
            send(fdt[i], buf, Size, 0); //send message to other
            printf("Send message to %d\n", fdt[i]);
        }
    }
}

void *pthread_service(void *sfd)
{
    int fd = *(int *)sfd;
    while (1)
    {
        int numbytes;
        int i;
        numbytes = recv(fd, mes, MAXSIZE, 0); //receive socket
        if (numbytes <= 0)                    
        {
            printf("============recieve over============\n");
            for (i = 0; i < Max; i++)
            {
                if (fd == fdt[i])
                {
                    fdt[i] = 0; //reset fd location
                }
                printf("fdt[%d]=%d\n", i, fdt[i]);
            }
            printf("numbytes=%d\n", numbytes);
            printf("Exit! fd=%d\n", fd);
            break;
        }
        printf("============recieving============\n");
        printf("Receive message from %d,size=%d\n", fd, numbytes);
        SendToClient(fd, mes, numbytes);
        bzero(mes, MAXSIZE); //reset message buff
    }
    close(fd);
}

int main()
{
    //variable set up
    int listenfd, connectfd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    int sin_size;
    sin_size = sizeof(struct sockaddr_in);
    int number = 0;
    int fd;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) //create socket
    {
        perror("Creating socket failed.");
        exit(1);
    }

    bzero(&server, sizeof(server)); // make all server parameter to zero

    server.sin_family = AF_INET;         // IPV4
    server.sin_port = htons(PORT);       // change port
    server.sin_addr.s_addr = INADDR_ANY; 
    if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) // combine IP address、port、socket together
    {
        perror("Bind error.");
        exit(1);
    }

    if (listen(listenfd, BACKLOG) == -1) // listen assignment port
    {
        perror("Listen() error\n");
        exit(1);
    }
    printf("Waiting for client....\n");

    while (1)
    {
        // receive client request
        // sockfd continue listen，and clientfd prepare recv() and send()
        if ((fd = accept(listenfd, (struct sockaddr *)&client, &sin_size)) == -1)
        {
            perror("Accept() error\n");
            exit(1);
        }

        if (number >= Max) //cannot be over the max size
        {
            printf("No more client is allowed\n");
            close(fd);
        }

        int i;

        for (i = 0; i < Max; i++) //save user's file descriptor
        {
            if (fdt[i] == 0)
            {
                fdt[i] = fd;
                break;
            }
        }
        pthread_t tid; //theread ID
        pthread_create(&tid, NULL, (void *)pthread_service, &fd); //set up new thread
        number = number + 1;
    }

    close(listenfd);
}
