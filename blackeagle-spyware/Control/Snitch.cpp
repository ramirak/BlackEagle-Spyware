#include "Snitch.h"

#define DEFAULT_PORT "666" 
#define DEFAULT_BUFLEN 512
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1) //this removes the need of mstcpip.h
struct sockaddr_in source, dest;


int snitchServer()
{/**/
	WSADATA wsaData;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult, iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	SOCKET ClientSocket;

	SOCKET sniffer;
	struct in_addr addr;
	int in = 0,i =0;

	char hostname[100];
	struct hostent* local;
	WSADATA wsa;


	//Initialise Winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSAStartup() failed.\n");
		return 1;
	}
	printf("Initialised");

	//Create a RAW Socket
	printf("\nCreating RAW Socket...");
	sniffer = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (sniffer == INVALID_SOCKET)
	{
		printf("Failed to create raw socket.\n");
		return 1;
	}
	printf("Created.");

	//Retrive the local hostname
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
	{
		printf("Error : %d", WSAGetLastError());
		return 1;
	}
	printf("\nHost name : %s \n", hostname);

	//Retrive the available IPs of the local host
	local = gethostbyname(hostname);
	printf("\nAvailable Network Interfaces : \n");
	if (local == NULL)
	{
		printf("Error : %d.\n", WSAGetLastError());
		return 1;
	}

	for (i = 0; local->h_addr_list[i] != 0; ++i)
	{
		memcpy(&addr, local->h_addr_list[i], sizeof(struct in_addr));
		printf("Interface Number : %d Address : %s\n", i, inet_ntoa(addr));
	}


	memset(&dest, 0, sizeof(dest));
	memcpy(&dest.sin_addr.s_addr, local->h_addr_list[in], sizeof(dest.sin_addr.s_addr));
	dest.sin_family = AF_INET;
	dest.sin_port = 0;

	printf("\nBinding socket to local system and port 0 ...");
	if (bind(sniffer, (struct sockaddr*)&dest, sizeof(dest)) == SOCKET_ERROR)
	{
		printf("bind(%s) failed.\n", inet_ntoa(addr));
		return 1;
	}
	printf("Binding successful");

	//Enable this socket with the power to sniff : SIO_RCVALL is the key Receive ALL ;)

	int j = 1;
	printf("\nSetting socket to sniff...");
	if (WSAIoctl(sniffer, SIO_RCVALL, &j, sizeof(j), 0, 0, (LPDWORD)&in, 0, 0) == SOCKET_ERROR)
	{
		printf("WSAIoctl() failed.\n");
		return 1;
	}
	printf("Socket set.");

	//Begin
	printf("\nStarted Sniffing\n");
	printf("Packet Capture Statistics...\n");



	IPV4_HDR* iphdr;


		FILE* logfile;
		fopen_s(&logfile, "liron", "wb");
		if (logfile == NULL) {
			return 0;
		}

		unsigned short iphdrlen;

		iphdr = (IPV4_HDR*)recvbuf;
		iphdrlen = iphdr->ip_header_len * 4;

		memset(&source, 0, sizeof(source));
		source.sin_addr.s_addr = iphdr->ip_srcaddr;

		memset(&dest, 0, sizeof(dest));
		dest.sin_addr.s_addr = iphdr->ip_destaddr;

		fprintf(logfile, "\n");
		fprintf(logfile, "IP Header\n");
		fprintf(logfile, " |-IP Version : %d\n", (unsigned int)iphdr->ip_version);
		fprintf(logfile, " |-IP Header Length : %d DWORDS or %d Bytes\n", (unsigned int)iphdr->ip_header_len, ((unsigned int)(iphdr->ip_header_len)) * 4);
		fprintf(logfile, " |-Type Of Service : %d\n", (unsigned int)iphdr->ip_tos);
		fprintf(logfile, " |-IP Total Length : %d Bytes(Size of Packet)\n", ntohs(iphdr->ip_total_length));
		fprintf(logfile, " |-Identification : %d\n", ntohs(iphdr->ip_id));
		fprintf(logfile, " |-Reserved ZERO Field : %d\n", (unsigned int)iphdr->ip_reserved_zero);
		fprintf(logfile, " |-Dont Fragment Field : %d\n", (unsigned int)iphdr->ip_dont_fragment);
		fprintf(logfile, " |-More Fragment Field : %d\n", (unsigned int)iphdr->ip_more_fragment);
		fprintf(logfile, " |-TTL : %d\n", (unsigned int)iphdr->ip_ttl);
		fprintf(logfile, " |-Protocol : %d\n", (unsigned int)iphdr->ip_protocol);
		fprintf(logfile, " |-Checksum : %d\n", ntohs(iphdr->ip_checksum));
		fprintf(logfile, " |-Source IP : %s\n", inet_ntoa(source.sin_addr));
		fprintf(logfile, " |-Destination IP : %s\n", inet_ntoa(dest.sin_addr));

		fclose(logfile);


		//End
		closesocket(sniffer);
		WSACleanup();
		return 0;
	}






	
