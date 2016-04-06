.PHONY:all
all:tcp_client tcp_server
tcp_client:tcp_client.c
	gcc -o $@ $^
tcp_server:tcp_server.c
	gcc -o $@ $^  -D _FUN1_
.PHONY:clean
clean:
	rm -rf tcp_client tcp_server

