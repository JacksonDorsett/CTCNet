#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <tcnetpackets.h>

#define BROADCAST_PORT 60000
#define BROADCAST_IP "255.255.255.255"
#define BUFFER_LEN 1024

TCNetOptInPacket initOptInPacket(){
    TCNetOptInPacket packet;
    packet.header.protocolVersionMajor = 3;
	packet.header.protocolVersionMinor = 3;
	memcpy(packet.header.header, "TCN", 3);
	packet.header.messageType = 2;
	packet.header.sequenceNumber = 0;
	packet.header.nodeType = 0;
	packet.header.nodeOptions = 0;
	packet.nodeCount = 1;
	packet.nodeListenerPort = 65023;
	memcpy(&packet.vendorName, "ECLIPTEK ENTRN.", TCNET_VENDOR_NAME_LENGTH);
	memcpy(&packet.applicationDeviceName, "The Ligma Node", TCNET_DEVICE_NAME_LENGTH);
}

int main() {
    int sockfd;
    struct sockaddr_in addr;
    int broadcast = 1;
    char buffer[BUFFER_LEN] = "Hello, Broadcast!";

    TCNetOptInPacket optin = initOptInPacket();
    uint32_t globalTimer = 0;
    uint32_t optinTimer = 0;
    // Create socket
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(EXIT_FAILURE);
    }

    // Configure settings for broadcasting
    addr.sin_family = AF_INET;
    addr.sin_port = htons(BROADCAST_PORT);
    addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    // Send the message every 1000 ms
    while (1) {
        if (sendto(sockfd, buffer, BUFFER_LEN, 0, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("sendto");
            exit(EXIT_FAILURE);
        }

        printf("Broadcast message sent\n");

        sleep(1);  // wait for 1 second
    }

    close(sockfd);
    return 0;
}
