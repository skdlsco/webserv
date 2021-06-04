#ifndef ERROR_RESPONSE_HPP
# define ERROR_RESPONSE_HPP

#include <string>
#include <vector>
#include "server/Response.hpp"
#include "file/File.hpp"
#include "utils/Time.hpp"
#include "utils/HTTP.hpp"
#include "logger/Logger.hpp"
class ErrorResponse : public Response
{
	private:
		bool mIsDefault;
		File mFile;

		ErrorResponse();
		ErrorResponse(ErrorResponse const & copy);

		std::string createResponse();
		void setErrorToDefault();
		std::string getAllowMethod();
		void appendResponseHeader();
		void appendResponseBody();
		void createUserErrorPage();
	public:
		static std::string const TAG;

		ErrorResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		static std::string getErrorResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig, int errorCode);

		ErrorResponse &operator=(ErrorResponse const & rhs);
		virtual ~ErrorResponse();
		virtual void run();
};

#endif
