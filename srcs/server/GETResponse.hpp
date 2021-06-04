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

class GETResponse : public Response
{
	enum PageState
	{
		INDEX_HTML, AUTOINDEX, TARGET
	};
	private:
		GETResponse();
	protected:
		enum PageState mPageState;
		std::string mContentLocation;
		void createResponseHeader(std::string const & responseBody, std::string & responseContent);
		std::string createResponseBody();
	public:
		static std::string const TAG;
		GETResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		GETResponse(GETResponse const & copy);
		GETResponse &operator=(GETResponse const & rhs);
		virtual ~GETResponse();

		bool isDirectory(const char *target);
		void initState();
		void initContentLocation();
		std::string makeAutoIndexContent();
		std::string readContentLocation();

		virtual std::string *getResponse();
};

#endif
