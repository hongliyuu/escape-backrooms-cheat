#include "network.h"

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

#include "gvalue.h"

float wait_time = 0.0f;

WSADATA clt_wsadata;
SOCKET clt_socket = INVALID_SOCKET;
sockaddr_in srv_addr;
bool network_ready = false;

constexpr int srv_port = 1314;
const char* srv_ip = "62.234.220.74";

network* network::get()
{
	static network inst;
	return &inst;
}

void network::init()
{
	network_ready = false;
	if (WSAStartup(MAKEWORD(2, 2), &clt_wsadata) != 0)
	{
		printf("WSAStartup(MAKEWORD(2, 2), &clt_wsadata) != 0\n");
		return;
	}

	clt_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clt_socket == INVALID_SOCKET)
	{
		printf("clt_socket == INVALID_SOCKET\n");
		return;
	}
	u_long mode = 1;
	if (ioctlsocket(clt_socket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		closesocket(clt_socket);
		clt_socket = INVALID_SOCKET;
		return;
	}

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(srv_port);
	srv_addr.sin_addr.s_addr = inet_addr(srv_ip);
	network_ready = true;
}

void network::main()
{
	if (!network_ready || clt_socket == INVALID_SOCKET)
	{
		return;
	}

	auto send_heart = [&]()
		{
			wait_time += gvalue::delta_time;
			if (wait_time < 2.0f)
			{
				return;
			}
			wait_time = 0.0f;
			send("heart");
		};

	auto recv_str = [&]()
		{
			char buffer[1024];
			sockaddr_in buf_addr;
			int buf_addr_size = sizeof(buf_addr);
			int recvLen = recvfrom(clt_socket, buffer, sizeof(buffer) - 1, 0, reinterpret_cast<sockaddr*>(&buf_addr), &buf_addr_size);
			if (recvLen != SOCKET_ERROR)
			{
				buffer[recvLen] = '\0';
				handle(buffer);
			}
		};

	send_heart();
	recv_str();
}

void network::send(const std::string& str)
{
	if (!network_ready || clt_socket == INVALID_SOCKET)
	{
		return;
	}

	sendto(
		clt_socket,
		str.c_str(),
		static_cast<int>(strlen(str.c_str())),
		0,
		reinterpret_cast<sockaddr*>(&srv_addr),
		static_cast<int>(sizeof(srv_addr))
	);
}

void network::handle(const std::string& str)
{
	//printf("%s\n", str.c_str());

	const size_t pos = str.find('|');
	if (pos == std::string::npos)
	{
		return;
	}

	gvalue::max_version = str.substr(0, pos);
	gvalue::log = str.substr(pos + 1);
}
