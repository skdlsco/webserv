#ifndef POST_RESPONSE_HPP
# define POST_RESPONSE_HPP

#include "Response.hpp"
#include "utils/Auth.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class POSTResponse : public Response
{
	private:
		static const int BUFFER_SIZE = 1024;
		POSTResponse();

		int mFD;
		std::string mFileName;
		std::string mBody;

		void checkAuthorization();
		void createFileName(std::string targetFile);
		void checkTarget();
		bool isFolderExist(std::string dir);
		bool isFileExist(std::string dir);
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
