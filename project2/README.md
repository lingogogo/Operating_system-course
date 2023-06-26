1.program target
Using socket and TCP to create a private chatroom(simple version)
it cannot memory last talk, so it has to improve.
2.
#build up
make

#setup
3.
./server in the first terminal

server has to setup first to deal with other client request connecting.

3.
./client in the other terminal and input correct IP address, you can check it by linux call:ifconfig
and input the right IP.
Usually, we use localhost IP 127.0.0.1 and other you can input 127.0.0.2 ......
ex.
#command input
./client 127.0.0.1

#clean
Using following command:
make clean

and it will delete all the execution file.
#========================

#Note
Max member is 10 users(it can change)
