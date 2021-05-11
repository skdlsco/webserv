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
	public:
		static std::string const TAG;

		Request();
		virtual ~Request();
		Request(Request const & copy);
		Request &operator=(Request const & rhs);

		void analyzeBuffer(char * buffer);

		// request-line -> 형식만 체크. (method sp target sp http-version\r\n) error 있음. (형식.)
		// header -> 대소문자 구분 x (fieldname:(sp)content\r\n) error 있음. (중복, 형식)
		// body -> default string, chunked (읽는 방법이 변경된다.) body에서 헤더에 관한 정보가 필요하다. content length.. chunked..
		// enum AnalyzeLevel. ready, requestline header, body, done xxx;

		enum AnalyzeLevel getAnalyzeLevel() const;
		std::string getBuffer() const;
		std::string getBody() const;
		std::string getMethod() const;
		std::string getTarget() const;
		std::map<std::string, std::string> getField() const;
		int getErrorCode() const;
};

#endif
