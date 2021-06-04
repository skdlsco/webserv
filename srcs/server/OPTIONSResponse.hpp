#ifndef OPTIONS_RESPONSE_HPP
# define OPTIONS_RESPONSE_HPP

#include <iostream>
#include <string>
#include "Response.hpp"
#include "ErrorResponse.hpp"
#include "utils/Time.hpp"

class OPTIONSResponse : public Response
{
	private:
		OPTIONSResponse();

		std::string mContentLocation;
	public:
		static std::string const TAG;
		OPTIONSResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		OPTIONSResponse(OPTIONSResponse const & copy);
		OPTIONSResponse &operator=(OPTIONSResponse const & rhs);
		virtual ~OPTIONSResponse();

		void appendResponseHeader();
		void appendResponseBody();

		virtual void run();
		void errorExcept();
};

#endif
