#ifndef CGI_RESPONSE_HPP
# define CGI_RESPONSE_HPP

#include "server/Response.hpp"
#include "utils/CGI.hpp"
#include <signal.h>
#include <sys/wait.h>

class CGIResponse : public Response
{
	private:
		static const int BUFFER_SIZE = 8192;
		CGIResponse();

		std::string mScriptFileName;
		std::string mPathInfo;
		std::string mDocumentRoot;
		std::string mScriptName;
		std::string mCGIResponse;

		std::map<std::string, std::string> mResponseHeader;
		std::string mResponseBody;

		char **mEnv;
		int mInPipe[2];
		int mOutPipe[2];
		pid_t mPid;

		void initCGIInfo();
		std::string getCGIVariableContentType();
		std::string getCGIVariableUserAgent();
		char *createCGIVariable(enum web::CGIEnv::CGIEnv cgiEnv);
		char **createCGIEnv();
		void freeEnv(char **env);
		void forkCGI();
		void execveCGI();
		void runCGI();
		void sendBody();
		void readCGI();
		bool responseToHeader();
		void appendResponseHeader(std::string &responseContent);
		std::string *createResponseContent();
	public:
		static std::string const TAG;

		CGIResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		CGIResponse(CGIResponse const & copy);
		CGIResponse &operator=(CGIResponse const & rhs);
		virtual ~CGIResponse();

		std::string *getResponse();
};

#endif
