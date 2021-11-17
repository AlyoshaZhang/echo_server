#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>

#define SERVER_PORT 666
#define IP "1.1.1.1"

void perror_exit(const char* des){
	fprintf(stderr, "%s error, reason: %s\n", des, strerror(errno));
     	exit(1);
}

int main(void){
	int sock;
	struct sockaddr_in server_addr;	

	int ret;

	sock = socket(AF_INET, SOCK_STREAM, 0);					//创建一个信箱   socket
	if(sock == -1){
		perror_exit("create socket");
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;	//选择协议族
	inet_pton(AF_INET, IP, &server_addr.sin_addr.s_addr);	
	//server_addr.sin_addr.s_addr = htonl(IP);			//监听本地所有IP 地址
	server_addr.sin_port = htons(SERVER_PORT);				//绑定端口号

	ret = bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));	//绑定服务器的地址
	if(ret == -1){
		perror_exit("bind");	
	

	}

	ret = listen(sock, 128);							//最多可以接入多少用户
	if(ret == -1){
		perror_exit("listen");
        }

	printf("等待客户端连接......\n");

	int done = 1;

	while(done){
		struct sockaddr_in client;
		int client_sock;
		char client_ip[64];
		char buf[256];
		int len;  //接受到的数据长度

		socklen_t client_addr_len;
		client_addr_len = sizeof(client);
		client_sock = accept(sock, (struct sockaddr*)&client, &client_addr_len);
		
		printf("client ip: %s\t prot: %d\n",inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)), ntohs(client.sin_port));
		
		//读取客户端发送的数据
		len = read(client_sock, buf, sizeof(buf)- 1);
		
		buf[len] = '\0';

		printf("recive: %s\n len: %d\n", buf, len);
		
		len = write(client_sock, buf, len);

		printf("write finished. len: %d \n", len);
		close(client_sock);	

		
	}






	close(sock);
	return 0;
}

