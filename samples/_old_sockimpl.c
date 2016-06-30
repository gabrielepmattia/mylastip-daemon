/*
    This file is part of MyLastIPd.

    MyLastIPd is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MyLastIPd is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MyLastIPd.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   mylastipd.c
 * Author: gabry3795
 *
 * Created on June 20, 2016, 3:30 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common.h"

int main(int argc, char** argv) {
    fprintf(stderr, "[INFO] Starting MyLasytIPd\n");
    int ret, sockfd;
    struct sockaddr_in server_addr = {0};

    char buffer[256];

    // Prepare the server information
    uint16_t port_number_no = htons((uint16_t) PORT_NO);
    in_addr_t ip_addr = inet_addr(HOSTNAME);

    // Start the socket
    fprintf(stderr, "[INFO] Opening socket...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) fprintf(stderr, "[ERROR] Error opening socket\n");
    fprintf(stderr, "[INFO] Socket open\n");

    server_addr.sin_addr.s_addr = ip_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port_number_no;

    fprintf(stderr, "[INFO] Trying to connect to %s at port %d...\n", HOSTNAME, PORT_NO);
    do {
        ret = connect(sockfd, (struct sockaddr*) &server_addr, sizeof (struct sockaddr_in));
        if (ret < 0) {
            fprintf(stderr, "[ERROR] Can't connect to server :: %s\n", strerror(errno));
            sleep(2);
        }
    } while (ret < 0);

    fprintf(stderr, "[INFO] Connection established!\n");

    printf("Please enter the message: \n");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    ret = write(sockfd, buffer, strlen(buffer));
    ret = close(sockfd);
    if (ret < 0) fprintf(stderr, "[ERROR] Can't close the socket :: %s\n", strerror(errno));
    return (EXIT_SUCCESS);
}

