#ifndef OPTIONS_RESPONSE_HPP
# define OPTIONS_RESPONSE_HPP

#include <iostream>
#include <string>
#include "Response.hpp"
#include "utils/Time.hpp"

class OPTIONSResponse : public Response
{
	private:
		OPTIONSResponse();
	protected:
		std::string mContentLocation;
		std::string *mResponseContent;

		virtual std::string *getResponse();
		void appendResponseHeader();
		void appendResponseBody();
	public:
		static std::string const TAG;
		OPTIONSResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		OPTIONSResponse(OPTIONSResponse const & copy);
		OPTIONSResponse &operator=(OPTIONSResponse const & rhs);
		virtual ~OPTIONSResponse();

};

#endif
