#ifndef HEAD_RESPONSE_HPP
# define HEAD_RESPONSE_HPP

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "Response.hpp"
#include "GETResponse.hpp"
#include "file/File.hpp"
#include "utils/Time.hpp"
#include "logger/Logger.hpp"

class HEADResponse : public GETResponse
{
	enum state
	{
		INDEX_HTML, AUTOINDEX, TARGET
	};
	private:
		HEADResponse();
	protected:
		virtual std::string *getResponse();
	public:
		static std::string const TAG;
		HEADResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		HEADResponse(HEADResponse const & copy);
		HEADResponse &operator=(HEADResponse const & rhs);
		virtual ~HEADResponse();
};

#endif
