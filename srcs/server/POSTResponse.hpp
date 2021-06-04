#ifndef POST_RESPONSE_HPP
# define POST_RESPONSE_HPP

#include "Response.hpp"
#include "utils/Auth.hpp"
#include "utils/String.hpp"
#include "server/ErrorResponse.hpp"

class POSTResponse : public Response
{
	private:
		static const int BUFFER_SIZE = 8192;
		POSTResponse();

		std::string mFileName;
		std::string mBody;

		void errorExcept();
		void checkAuthorization();
		void createFileName(std::string const & path);
		void checkTarget();
		void writeFile();
		void appendResponseHeader();
	public:
		static std::string const TAG;
		POSTResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		virtual ~POSTResponse();
		POSTResponse(POSTResponse const & copy);
		POSTResponse &operator=(POSTResponse const & rhs);

		virtual void run();
};

#endif
