#ifndef PUT_RESPONSE_HPP
# define PUT_RESPONSE_HPP

#include "Response.hpp"
#include "utils/Auth.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class PUTResponse : public Response
{
	private:
		static const int BUFFER_SIZE = 1024;
		PUTResponse();

		std::string mFileName;
		std::string mBody;

		void checkAuthorization();
		void createFileName(std::string targetFile);
		void checkTarget();
		bool isFolderExist(std::string path);
		bool isFileExist(std::string path);
		void writeFile();
		void appendResponseHeader(std::string &responseContent);
		void appendResponseBody(std::string &responseContent);
	public:
		static std::string const TAG;
		PUTResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		virtual ~PUTResponse();
		PUTResponse(PUTResponse const & copy);
		PUTResponse &operator=(PUTResponse const & rhs);

		std::string *getResponse();
};

#endif
