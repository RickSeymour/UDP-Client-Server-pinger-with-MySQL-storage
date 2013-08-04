#define SENSOR_ID "666"

#define SRV_ADDR "192.168.45.135"
#define SRV_PORT 9930
#define BUFLEN 512
#define NPACK 10

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
using namespace std;


void err(const char* str)
{
	perror(str);
	exit(1);
}


int random(int max)
{
	return rand()%max;
}





int main()
{
	struct sockaddr_in serv_addr;
	int sockfd, slen=sizeof(serv_addr);
	char buf[BUFLEN];

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		err("socket");

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SRV_PORT);
	if (inet_aton(SRV_ADDR, &serv_addr.sin_addr)==0)
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	while(1)
	{

		int rnd = random(100);

		sprintf(buf, "%s|%d", SENSOR_ID,rnd);

		printf("Sensor (%s) sending data (%d) to %s:%d\n",SENSOR_ID,rnd,SRV_ADDR,SRV_PORT );

		if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&serv_addr, slen)==-1)
			err("sendto()");

		sleep(4);
	}

	close(sockfd);
	return 0;
}
