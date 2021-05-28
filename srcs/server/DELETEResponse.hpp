#ifndef DELETE_RESPONSE_HPP
# define DELETE_RESPONSE_HPP

#include <iostream>
#include <string>
#include "Response.hpp"
#include "file/File.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class DELETEResponse : public Response
{
	private:
		DELETEResponse();
		std::string mDeleteTarget;
	public:
		static std::string const TAG;
		DELETEResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		DELETEResponse(DELETEResponse const & copy);
		DELETEResponse &operator=(DELETEResponse const & rhs);
		virtual ~DELETEResponse();

		std::string *getResponse();
		bool isFolder(std::string path);
		bool isFileExist(std::string path);
		void deleteFile();
		void checkTarget();
		void appendResponseHeader(std::string &responseContent);
		void appendResponseBody(std::string &responseContent);
};

#endif