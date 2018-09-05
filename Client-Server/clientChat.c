#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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
main Function

Inputs: raw server address, port number
Returns: 0
Call: ./clientChat 127.0.0.1 9898
*/
int main(int argc, char *argv[]) {
	/*
	sockfd: socket file descriptor
	portno: Port number
	buffer: message buffer
	serv_addr: contains server local address and port number
	server: server address
	*/
	int sockfd, portno;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[255];
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n",argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);
	// Initializing socket with IPv4 TCP protocols
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("Error opening Socket.");
	}

	// Getting server address from raw address
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "Error, no such host");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	// sending request to accept the connection to the server
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("Connetion Failed");
	} else {
		printf("Connection Established!\n");
	}

	while (1) {
		printf("You: ");
		// Clearing chat buffer
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		// sending the message
		int n;
		n = write(sockfd, buffer, strlen(buffer));
		// if the sent message is "Bye"
		// terminates the connection
		if (n < 0) {
			error("Error on writing");
		}
		int i = strncmp("Bye", buffer, 3);
		if (i == 0)	{
			break;
		}

		bzero(buffer, 255);
		// recieving the incoming message
		n = read(sockfd, buffer, 255);
		if (n < 0) {
			error("Error on reading");
		}
		
		printf("Server: %s\n", buffer);
		// If the message from server is "Bye", 
		// terminate the connection
		i = strncmp("Bye", buffer, 3);
		if (i == 0)	{
			break;
		}
	}

	// Closing the socket
	close(sockfd);
	return 0;

}
