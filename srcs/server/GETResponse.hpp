#ifndef GET_RESPONSE_HPP
# define GET_RESPONSE_HPP

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Response.hpp"
#include "file/File.hpp"

class GETResponse : public Response
{
	enum state
	{
		INDEX_HTML, AUTOINDEX, TARGET
	};
	private:
		GETResponse();
		std::string mContentLocation;
		std::string mBody;
		enum state mState;
	protected:
		std::string createResponseHeader();
		std::string createResponseBody();
	public:
		static std::string const TAG;
		GETResponse(ServerManager &serverManager, const ServerConfig * serverConfig,
					const LocationConfig * locationConfig);
		GETResponse(GETResponse const & copy);
		GETResponse &operator=(GETResponse const & rhs);
		virtual ~GETResponse();

		bool isDirectory();
		std::string makeAutoIndexContent();
		std::string readIndexFile();
		std::string readTargetFile();
};

#endif