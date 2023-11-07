
int main()
{
#ifdef WIN32
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif
	sockaddr_in addr;

#ifdef WIN32
	WSACleanup();
#endif
}