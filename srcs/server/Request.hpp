#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>
#include "logger/Logger.hpp"
#include "utils/String.hpp"
#include "utils/Method.hpp"
#include "config/ServerConfig.hpp"

enum AnalyzeLevel
{
	REQUEST_LINE, HEADER, BODY, DONE
};

class Request
{
	typedef std::map<std::string, std::string>::iterator FieldIter;

	private:
		static std::string const HTTP_VERSION;

		const ServerConfig *mConfig;
		enum AnalyzeLevel mAnalyzeLevel;
		std::string mBuffer;
		std::string mBody;
		std::string mMethod;
		std::string mTarget;
		std::map<std::string, std::string> mField;

		bool mHasBody;
		bool mIsChunked;
		bool mIsReadData;
		int mContentLength;
		int mErrorCode;

		void badRequest();
		void appendChunkedBody();
		void appendContentBody();
		void checkContentLength();
		void checkTransferEncoding();
		void checkHeaderForBody();
		void analyzeHeader();
		void analyzeRequestLine(std::string line);
		void analyzeHeaderField(std::string line);
		void analyzeBody();
		bool isValidMethod(std::string method);
	public:
		static std::string const TAG;

		Request();
		virtual ~Request();
		Request(Request const & copy);
		Request &operator=(Request const & rhs);

		void analyzeBuffer(char * buffer);

		const ServerConfig *getConfig() const;
		void setConfig(const ServerConfig *config);
		enum AnalyzeLevel getAnalyzeLevel() const;
		std::string getBuffer() const;
		std::string getBody() const;
		std::string getMethod() const;
		std::string getTarget() const;
		std::map<std::string, std::string> getField() const;
		int getErrorCode() const;
};

#endif
