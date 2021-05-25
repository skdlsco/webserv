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
		class POSTResponseFDListener : public FileDiscriptorListener
		{
			private:
				POSTResponse &mResponse;
				POSTResponseFDListener();
				POSTResponseFDListener(POSTResponseFDListener const & copy);
				POSTResponseFDListener &operator=(POSTResponseFDListener const & copy);
			public:
				POSTResponseFDListener(POSTResponse &POSTResponse);
				virtual ~POSTResponseFDListener();

				void onReadSet();
				void onWriteSet();
				void onExceptSet();
		};
	private:
		static const int BUFFER_SIZE = 1024;
		POSTResponse();

		int mFD;
		POSTResponseFDListener mFDListener;
		std::string mFileName;
		std::string mBody;

		void checkAuthorization();
		void createFileName(std::string targetFile);
		void checkTarget();
		bool isFolderExist(std::string dir);
		bool isFileExist(std::string dir);
		void setError(int errorCode);
		void openFile();
	protected:
		virtual std::string createResponseHeader();
		virtual std::string createResponseBody();
	public:
		static std::string const TAG;
		POSTResponse(ServerManager &serverManager, const ServerConfig * serverConfig,
						const LocationConfig * locationConfig);
		virtual ~POSTResponse();
		POSTResponse(POSTResponse const & copy);
		POSTResponse &operator=(POSTResponse const & rhs);

		virtual void onRepeat();
};

#endif
