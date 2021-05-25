#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>
#include "logger/Logger.hpp"
#include "utils/String.hpp"
#include "utils/Method.hpp"
#include "utils/Config.hpp"
#include "config/ServerConfig.hpp"

class Request
{
	typedef std::map<std::string, std::string>::iterator FieldIter;
	public:
		enum AnalyzeLevel
		{
			REQUEST_LINE, HEADER, BODY, DONE
		};
	private:
		static std::string const HTTP_VERSION;

		std::vector<ServerConfig *> const &mConfigVec;
		ServerConfig *mConfig;
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

		Request();
		Request(Request const & copy);
		Request &operator=(Request const & rhs);

		void badRequest();
		void appendChunkedBody();
		void appendContentBody();
		void checkHost();
		void checkContentLength();
		void checkTransferEncoding();
		void checkHeaderForBody();
		void analyzeHeader();
		void analyzeRequestLine(std::string line);
		void analyzeHeaderField(std::string line);
		void analyzeBody();
		bool isValidMethod(std::string method);
		bool isValidTarget(std::string target);
	public:
		static std::string const TAG;

		Request(std::vector<ServerConfig *> const & configVec);
		virtual ~Request();

		void analyzeBuffer(char * buffer);

		std::vector<ServerConfig *> const &getConfigVec() const;
		ServerConfig *getConfig() const;
		enum Request::AnalyzeLevel getAnalyzeLevel() const;
		std::string getBuffer() const;
		std::string getBody() const;
		std::string getMethod() const;
		std::string getTarget() const;
		std::map<std::string, std::string> getField() const;
		int getErrorCode() const;
};

#endif
