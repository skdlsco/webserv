#ifndef HEAD_RESPONSE_HPP
# define HEAD_RESPONSE_HPP

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

class HEADResponse : public Response
{
	enum state
	{
		INDEX_HTML, AUTOINDEX, TARGET
	};
	private:
		HEADResponse();
		enum state mState;
		std::string mContentLocation;
		std::string *mResponseContent;
	protected:
		std::string *getResponse();
		void createResponseHeader(std::string const & responseBody);
		std::string createResponseBody();
	public:
		static std::string const TAG;
		HEADResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		HEADResponse(HEADResponse const & copy);
		HEADResponse &operator=(HEADResponse const & rhs);
		virtual ~HEADResponse();

		bool isDirectory(const char *target);
		void setContentLocation();
		std::string makeAutoIndexContent();
		std::string readContentLocation();
};

#endif
