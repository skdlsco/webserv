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
		std::string *mResponseContent;
		bool mIsDefault;
		File mFile;

		ErrorResponse();
		ErrorResponse(ErrorResponse const & copy);
		ErrorResponse &operator=(ErrorResponse const & rhs);

		void setErrorToDefault();
		std::string getAllowMethod();
		void createResponseHeader();
		void createResponseBody();
		void createUserErrorPage();
	public:
		static std::string const TAG;

		ErrorResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		virtual ~ErrorResponse();

		virtual std::string *getResponse();
};

#endif
