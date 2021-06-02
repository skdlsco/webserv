#ifndef CGI_RESPONSE_HPP
# define CGI_RESPONSE_HPP

#include "server/Response.hpp"
#include "utils/CGI.hpp"
#include <signal.h>
#include <sys/wait.h>

class CGIResponse : public Response
{
	private:
		class ReadListener : public FileDiscriptorListener
		{
			private:
				CGIResponse &mResponse;
				ReadListener();
				ReadListener(ReadListener const & copy);
				ReadListener &operator=(ReadListener const & copy);
			public:
				ReadListener(CGIResponse &response);
				virtual ~ReadListener();

				void onReadSet();
				void onWriteSet();
				void onExceptSet();
		};
	private:
		class WriteListener : public FileDiscriptorListener
		{
			private:
				CGIResponse &mResponse;
				WriteListener();
				WriteListener(WriteListener const & copy);
				WriteListener &operator=(WriteListener const & copy);
			public:
				WriteListener(CGIResponse &response);
				virtual ~WriteListener();

				void onReadSet();
				void onWriteSet();
				void onExceptSet();
		};
	public:
		enum CGIResponseState
		{
			READY, ON_WORKING, DONE, ERROR
		};
	private:
		static const int BUFFER_SIZE = 8192;
		CGIResponse();

		ServerManager &mServerManager;
		std::string mScriptFileName;
		std::string mPathInfo;
		std::string mDocumentRoot;
		std::string mScriptName;
		std::string mCGIResponse;

		std::string mRequestBody;
		size_t mWriteIdx;
		bool mIsCGIReadHeader;

		std::map<std::string, std::string> mResponseHeader;
		std::string mResponseBody;

		char **mEnv;
		int mInPipe[2];
		int mOutPipe[2];
		pid_t mPid;
		enum CGIResponseState mState;
		WriteListener mWriteListener;
		ReadListener mReadListener;

		void initCGIInfo();
		std::string getCGIVariableContentType();
		std::string getCGIVariableUserAgent();
		std::string dashToUnderBar(std::string str);
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

		CGIResponse(ServerManager &serverManager, const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		CGIResponse(CGIResponse const & copy);
		CGIResponse &operator=(CGIResponse const & rhs);
		virtual ~CGIResponse();

		std::string *getResponse();
		void run();
		enum CGIResponseState getState() const;
};

#endif
