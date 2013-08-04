
all:
	cd c; \
	/bin/rm -f server server.o client client.o; \
	g++ -o server.o -c server.c  `mysql_config --cflags --libs`; \
	g++ -o server server.o `mysql_config --cflags --libs`; \
	g++ -Wall -o client.o -c client.c; \
	g++ -o client client.o;
