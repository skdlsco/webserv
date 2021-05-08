#include "Socket.hpp"

std::string const Socket::TAG = "Socket";

Socket::Socket()
{
	mSocketFD = socket(PF_INET, SOCK_STREAM, 0);
	if (mSocketFD < 0)
		throw Socket::SocketException("socket open failed");
}

Socket::~Socket()
{
	close(mSocketFD);
}


void Socket::bind(int family, int addr, int port)
{
	struct sockaddr_in socketAddr;

	socketAddr.sin_family = family;
	socketAddr.sin_addr.s_addr = htons(addr); // TODO htons
	socketAddr.sin_port = htons(port);
	if (::bind(mSocketFD, (struct sockaddr *)&socketAddr, sizeof(socketAddr)) < 0)
		throw Socket::SocketException("socket bind failed");
}

void Socket::bind(int port)
{
	bind(AF_INET, INADDR_ANY, port);
}

void Socket::listen(int backlog)
{
	if (::listen(mSocketFD, backlog) < 0)
		throw Socket::SocketException("socket listen failed");
}

int Socket::accept(struct sockaddr_in *clientAddr, unsigned int len)
{
	int fd;

	fd = ::accept(mSocketFD, (struct sockaddr *)clientAddr, &len);
	if (fd < 0)
		throw Socket::SocketException("socket accept failed");
	return (fd);
}

int Socket::accept(struct sockaddr_in &clientAddr)
{
	return (accept(&clientAddr, sizeof(clientAddr)));
}

int Socket::getSocketFD() const
{
	return (mSocketFD);
}

Socket::SocketException::SocketException(std::string message) throw()
{
	mMessage = "SOCKET ERROR: " + message;
}

Socket::SocketException::~SocketException() throw()
{

}

const char* Socket::SocketException::what() const throw()
{
	return (mMessage.c_str());
}
