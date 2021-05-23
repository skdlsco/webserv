#ifndef ERROR_RESPONSE_HPP
# define ERROR_RESPONSE_HPP

#include "server/Response.hpp"
#include "file/File.hpp"
#include "utils/Time.hpp"

class ErrorResponse : public Response
{
	private:
		class ErrorResponseFDListener : public FileDiscriptorListener
		{
			private:
				ErrorResponse &mErrorResponse;
				ErrorResponseFDListener();
				ErrorResponseFDListener(ErrorResponseFDListener const & copy);
				ErrorResponseFDListener &operator=(ErrorResponseFDListener const & copy);
			public:
				ErrorResponseFDListener(ErrorResponse &mErrorResponse);
				virtual ~ErrorResponseFDListener();

				void onReadSet();
				void onWriteSet();
				void onExceptSet();
		};
	private:
		bool mIsDefault;
		File mFile;
		ErrorResponseFDListener mFDListener;

		ErrorResponse();
		ErrorResponse(ErrorResponse const & copy);
		ErrorResponse &operator=(ErrorResponse const & rhs);

		void setErrorToDefault();
	protected:
		std::string createResponseHeader();
		std::string createResponseBody();
	public:
		static std::string const TAG;

		ErrorResponse(ServerManager &serverManager, const ServerConfig * serverConfig,
					const LocationConfig * locationConfig);
		virtual ~ErrorResponse();
};

#endif
