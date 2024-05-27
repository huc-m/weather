#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>

#define LATITUDE ""
#define LONGITUDE ""

int main(){
	const char get[]="GET /v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE  "&current=temperature_2m,precipitation,cloud_cover,wind_speed_10m&timezone=auto&forecast_days=1 HTTP/1.1\r\nHost: api.open-meteo.com \r\n\r\n";
	const struct sockaddr_in server_address = {AF_INET, 20480, 3798149308};
	char txt[1000];
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
		connect(socketfd, (struct sockaddr*)&server_address,sizeof(server_address));
		send(socketfd, get, sizeof(get), 0);
		read(socketfd, txt, 998);
		close(socketfd);

	char *list[] = {"time", "temperature_2m", "precipitation", "cloud_cover", "wind_speed_10m", ""};
	char *dims[5]; dims[0] = list[5];
	int i; char *c = txt, *value;

	for(i=1; i<5; ++i) {
		c = strstr(c+1, list[i]); 
		dims[i] = c+="\x00\x11\x10\x0e\x11"[i];
		*(c = strchr(c, '"')) = '\0';
	}
	for(i=0; i<5; ++i) {
		c = strstr(c+1, list[i]);
		value = c+="\x12\x10\x0f\x0d\x10"[i];
		*(c = strchr(c, "\",,,}"[i])) = '\0';
		printf("\n%14.*s : %s%s", "\x04\x0b\x0d\x0a\x0a"[i], list[i], value, dims[i]);
	}
	printf("\n");
}
