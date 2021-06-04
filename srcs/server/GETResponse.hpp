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
#include "logger/Logger.hpp"
#include "ErrorResponse.hpp"

class GETResponse : public Response
{
	enum PageState
	{
		INDEX_HTML, AUTOINDEX, TARGET
	};
	private:
		enum PageState mPageState;
		std::string mContentLocation;

		GETResponse();
	public:
		static std::string const TAG;
		GETResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		GETResponse(GETResponse const & copy);
		GETResponse &operator=(GETResponse const & rhs);
		virtual ~GETResponse();

		void appendResponseHeader(std::string const & responseBody);
		std::string appendResponseBody();

		bool isDirectory(const char *target);
		void initState();
		void initContentLocation();
		std::string makeAutoIndexContent();
		std::string readContentLocation();

		virtual void run();
		void errorExcept();
};

#endif
