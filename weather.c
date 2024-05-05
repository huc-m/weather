#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>

#define LATITUDE ""
#define LONGITUDE ""

int main(){
	const char get[]="GET /v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE  "&current=temperature_2m,precipitation,cloud_cover,wind_speed_10m&timezone=auto&forecast_days=1 HTTP/1.1\r\nHost: api.open-meteo.com \r\n\r\n";
	const struct sockaddr_in server_address = {AF_INET, 20480, 3798149308};
	char txt[1000]; txt[999] = '\0';
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
		connect(socketfd, (struct sockaddr*)&server_address,sizeof(server_address));
		send(socketfd, get, sizeof(get), 0);
		read(socketfd, txt, 998);
		close(socketfd);
	char *c = strstr(txt,"\r\n\r\n"), *value;
	int i;
	char* list[] = {"time", "temperature_2m", "precipitation", "cloud_cover", "wind_speed_10m", ""};
	char* dims[5]; dims[0] = list[5];
	
	for(i=1; i<5; ++i) {
		c = strstr(c, list[i]); c = strchr(c, ':'); c = strchr(c, '"');
		dims[i] = ++c; c = strchr(c, '"'); *c = '\0'; c++;
	}
	printf("\n");
	for(i=0; i<5; ++i) {
		c = strstr(c, list[i]); c = strchr(c, i > 0 ? ':' : 'T');
		value = ++c; c = strchr(c, "\",,,}"[i]); *c = '\0'; c++;
		printf("%14s : %s%s\n", list[i], value, dims[i]);
	}
	printf("\n");
}
