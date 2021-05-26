#ifndef GET_RESPONSE_HPP
# define GET_RESPONSE_HPP

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "Response.hpp"
#include "file/File.hpp"
#include "utils/Time.hpp"

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
		void run();
		std::string createResponseHeader();
		std::string createResponseBody();
	public:
		static std::string const TAG;
		GETResponse(ServerManager &serverManager, const ServerConfig * serverConfig,
					const LocationConfig * locationConfig);
		GETResponse(GETResponse const & copy);
		GETResponse &operator=(GETResponse const & rhs);
		virtual ~GETResponse();

		bool isDirectory(const char *target);
		std::string makeAutoIndexContent();
		std::string readIndexPageContent();
		std::string readTargetContent();
};

#endif