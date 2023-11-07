#include <WinSock2.h>
#pragma comment(lib,"ws2_32")
#include <iostream>
#pragma warning(disable:4996)
//MAKEWORD(2,2) //0x0202
using namespace std;
IN_ADDR GetIP() {
	char localhostname[MAX_PATH];
	IN_ADDR addr = { 0, };
	if (gethostname(localhostname, MAX_PATH) == SOCKET_ERROR)
		return addr;
	HOSTENT* ptr = gethostbyname(localhostname);
	while (ptr && ptr->h_name) 
	{
		if (ptr->h_addrtype == PF_INET) {
			memcpy(&addr, ptr->h_addr_list[0], ptr->h_length);
			break;
		}
		ptr++;
	}
	return addr;
}
int main() {
	WSADATA wsadata;
	
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	unsigned int a = 0x12345678;
	printf("%#x", htonl(a));
	
	/*u_int naddr = inet_addr("192.168.32.0");
	u_int haddr = ntohl(naddr);
	printf("%d.%d.%d.%d", haddr >> 24, (u_char)(haddr >> 16), (u_char)(haddr >> 8), (u_char)(haddr));*/
	IN_ADDR m_addr = GetIP();
	printf("%s", inet_ntoa(m_addr));



	WSACleanup();

	return 0;
}