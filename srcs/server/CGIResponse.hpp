#ifndef CGI_RESPONSE_HPP
# define CGI_RESPONSE_HPP

#include "server/Response.hpp"
#include "utils/CGI.hpp"

class CGIResponse : public Response
{
	private:
		CGIResponse();

		std::string mPathInfo;
		std::string mDocumentRoot;
		std::string mScriptName;

		void initCGIInfo();
		std::string getCGIVariableContentType();
		std::string getCGIVariableUserAgent();
		char *createCGIVariable(enum web::CGIEnv::CGIEnv cgiEnv);
		char **createCGIEnv();
		void freeEnv(char **env);
	public:
		static std::string const TAG;

		CGIResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		CGIResponse(CGIResponse const & copy);
		CGIResponse &operator=(CGIResponse const & rhs);
		virtual ~CGIResponse();

		std::string *getResponse();
};

#endif
