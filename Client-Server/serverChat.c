#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
sockfd: socket file descriptor
newsockfd: new file descriptor for accepted connection
portno: Port number
buffer: message buffer
serv_addr: contains server local address and port number
cli_addr: contains client address
clilen: size of strct cli_addr
*/
int sockfd, newsockfd, portno;
char buffer[255];
struct sockaddr_in serv_addr, cli_addr;
socklen_t clilen;

void error(const char *);
void listn();
void chat();

/*
Function to print error message
Inputs: Error message
Returns: void
Call: error((char *)message)
*/
void error(const char *msg) {
	perror(msg);
	exit(1);
}

/*
Function to:
	-listen for clients on socket
	-Accept incoming connection
	-Call the chat function to start the client server communication

Inputs: void
Returns: void
Call: listn()
*/
void listn() {
	//Listening to atmost 4 backlogs
	listen(sockfd, 4);
	printf("Accepting Connections...\n");
	clilen = sizeof(cli_addr);

	// Accepting incoming connections
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0) {
		error("Error on accept");
	} else 
		printf("Connection Established!\n");

	chat();
}

/*
Function to:
	-deploy client serve communication
	-message transmission is done, one message at a time

Inputs: void
Returns: void
Call: chat()
*/
void chat() {
	while (1) {
		// Clearing chat buffer
		bzero(buffer, 255);
		// recieving the incoming message
		int n;
		n = read(newsockfd, buffer, 255);
		if (n < 0) {
			error("Error on reading");
		}
		// If the message from client is "Bye", 
		// prompts to terminate the connection
		int i = strncmp("Bye", buffer, 3);
		if (i == 0)	{
			printf("Client: %s\nClient left! End portal? (y/n): ", buffer);
			bzero(buffer, 255);
			char e;
			scanf("%c",&e);
			while ((getchar()) != '\n');
			if (e == 'y')	{
				break;
			} else {
				// If connection termination cancelled
				// start listing for incoming connections
				listn();
				break;
			}
		} else {
			printf("Client: %s\nServer: ", buffer);
		}
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);

		// sending the message
		n = write(newsockfd, buffer, strlen(buffer));
		if (n < 0) {
			error("Error on writing");
		}

		// if the sent message is "Bye"
		// terminates the connection
		i = strncmp("Bye", buffer, 3);
		if (i == 0)	{
			break;
		}
	}
}

/*
main Function

Inputs: port number
Returns: 0
Call: ./serverChat 9898
*/
int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr,"Port number not provided. Program terminated.\n");
		exit(1);
	}
	
	// Initializing socket with IPv4 TCP protocols
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("Error opening Socket.");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// Binding the socket to localhost aka 127.0.0.1
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Binding Failed");

	// Initial call to listen
	listn();

	// Closing all the sockets
	close(newsockfd);
	close(sockfd);
	return 0;

}
