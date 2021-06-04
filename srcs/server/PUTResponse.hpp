#ifndef PUT_RESPONSE_HPP
# define PUT_RESPONSE_HPP

#include "Response.hpp"
#include "utils/Auth.hpp"
#include "utils/String.hpp"
#include "server/ErrorResponse.hpp"

class PUTResponse : public Response
{
	private:
		static const int BUFFER_SIZE = 8192;
		PUTResponse();

		std::string mFileName;
		std::string mBody;

		void errorExcept();
		void checkAuthorization();
		void checkTarget();
		void writeFile();
		void appendResponseHeader();
	public:
		static std::string const TAG;
		PUTResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		virtual ~PUTResponse();
		PUTResponse(PUTResponse const & copy);
		PUTResponse &operator=(PUTResponse const & rhs);

		virtual void run();
};

#endif
