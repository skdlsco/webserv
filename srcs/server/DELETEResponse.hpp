#ifndef DELETE_RESPONSE_HPP
# define DELETE_RESPONSE_HPP

#include <iostream>
#include <string>
#include "Response.hpp"
#include "file/File.hpp"
#include "utils/HTTP.hpp"

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
		void deleteFile();
		void checkTarget();
		void appendResponseHeader(std::string &responseContent);
		void appendResponseBody(std::string &responseContent);
};

#endif