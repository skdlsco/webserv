#ifndef DELETE_RESPONSE_HPP
# define DELETE_RESPONSE_HPP

#include <iostream>
#include <string>
#include "Response.hpp"
#include "ErrorResponse.hpp"
#include "file/File.hpp"
#include "utils/String.hpp"
class DELETEResponse : public Response
{
	private:
		DELETEResponse();
		std::string mDeleteTarget;

		void deleteFile();
		void checkTarget();
		void appendResponseHeader();
		void appendResponseBody();
	public:
		static std::string const TAG;
		DELETEResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		DELETEResponse(DELETEResponse const & copy);
		DELETEResponse &operator=(DELETEResponse const & rhs);
		virtual ~DELETEResponse();
		void errorExcept();

		virtual void run();
};

#endif