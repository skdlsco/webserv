#ifndef ERROR_RESPONSE_HPP
# define ERROR_RESPONSE_HPP

#include <string>
#include <vector>
#include "server/Response.hpp"
#include "file/File.hpp"
#include "utils/Time.hpp"
#include "utils/HTTP.hpp"

class ErrorResponse : public Response
{
	private:
		bool mIsDefault;
		File mFile;

		ErrorResponse();

		void setErrorToDefault();
		std::string getAllowMethod();
		void createResponseHeader(std::string *responseContent);
		void createResponseBody(std::string *responseContent);
		void createUserErrorPage();
	public:
		static std::string const TAG;

		ErrorResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		ErrorResponse(ErrorResponse const & copy);
		ErrorResponse &operator=(ErrorResponse const & rhs);
		virtual ~ErrorResponse();

		virtual std::string *getResponse();
};

#endif
