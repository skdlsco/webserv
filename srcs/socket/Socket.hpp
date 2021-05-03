#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <exception>
#include <unistd.h>
#include <string>

class Socket
{
	private:
		int mSocketFD;

		Socket(Socket const & copy);
		Socket &operator=(Socket const & copy);
	public:
		static std::string const TAG;

		Socket();
		~Socket();

		void bind(int family, int addr, int port);
		void bind(int port);
		void listen(int backlog);
		int accept(struct sockaddr_in *clientAddr, unsigned int len);
		int accept(struct sockaddr_in &clientAddr);

		int getSocketFD() const;

		class SocketException : public std::exception
		{
			private:
				std::string mMessage;
			public:
				virtual ~SocketException() throw();
				SocketException(std::string mMessage) throw();
				virtual const char* what() const throw();
		};
};

#endif
