#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>
#include "logger/Logger.hpp"
#include "utils/String.hpp"
#include "utils/Method.hpp"

enum AnalyzeLevel
{
	REQUEST_LINE, HEADER, BODY, DONE
};

class Request
{
	private:
		static std::string const HTTP_VERSION;

		enum AnalyzeLevel mAnalyzeLevel;
		std::string mBuffer;
		std::string mBody;
		std::string mMethod;
		std::string mTarget;
		std::map<std::string, std::string> mField;
		// ischunked?
		// contentlength?
		int mErrorCode;

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

		enum AnalyzeLevel getAnalyzeLevel() const;
		std::string getBuffer() const;
		std::string getBody() const;
		std::string getMethod() const;
		std::string getTarget() const;
		std::map<std::string, std::string> getField() const;
		int getErrorCode() const;
};

#endif
