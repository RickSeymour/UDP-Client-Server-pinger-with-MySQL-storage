#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <time.h>

using namespace std;


#define BUFLEN 512
#define NPACK 10
#define PORT 9930
#define DELIM "|"

#define DB_HOST "192.168.45.135"
#define DB_USER "root"
#define DB_PASS ""
#define DB_NAME "remote"
#include <mysql.h>


void err(const char* s)
{
	perror(s);
	exit(EXIT_FAILURE);
}








int main(void)
{

	MYSQL *conn;
	// MYSQL_RES *res;
	// MYSQL_ROW row;
	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS , DB_NAME, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}


	if (mysql_query(conn, "TRUNCATE TABLE data"))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}



    struct sockaddr_in my_addr, cli_addr;
    int sockfd;
    socklen_t slen=sizeof(cli_addr);
    char buf[BUFLEN];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      err("socket");
    else
      printf("Server : Socket() successful\n");

    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr* ) &my_addr, sizeof(my_addr))==-1)
      err("bind");
    else
      printf("Server : bind() successful\n");

    while(1)
    {
        if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
            err("recvfrom()");
        printf("Received packet from %s:%d\nData: %s\n\n",
               inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);

        char* sensor = strtok (buf,DELIM);

        // printf("%s\n", sensor);

        char* value = strtok (NULL,DELIM);

        // printf("%s\n", value);

		ostringstream query_o;

        query_o << "INSERT INTO data (sensor_id,created_at,data) VALUES('" << sensor <<"','"<< time(0) <<"','" << value << "')";

		string str = query_o.str();
		// printf("%s\n", str.c_str());

		if (mysql_query(conn, str.c_str()))
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}



    }

	mysql_close(conn);

    close(sockfd);
    return 0;
}
