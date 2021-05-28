#ifndef POST_RESPONSE_HPP
# define POST_RESPONSE_HPP

#include "Response.hpp"
#include "utils/Auth.hpp"
#include "utils/HTTP.hpp"

class POSTResponse : public Response
{
	private:
		static const int BUFFER_SIZE = 1024;
		POSTResponse();

		std::string mFileName;
		std::string mBody;

		void checkAuthorization();
		void createFileName(std::string const & path);
		void checkTarget();
		void writeFile();
		void appendResponseHeader(std::string &responseContent);
		void appendResponseBody(std::string &responseContent);
	public:
		static std::string const TAG;
		POSTResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		virtual ~POSTResponse();
		POSTResponse(POSTResponse const & copy);
		POSTResponse &operator=(POSTResponse const & rhs);

		std::string *getResponse();
};

#endif
