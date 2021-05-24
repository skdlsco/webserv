#ifndef GET_RESPONSE_HPP
# define GET_RESPONSE_HPP

#include <iostream>
#include <string>
#include "Response.hpp"
#include "utils/Time.hpp"

class GETResponse : public Response
{
	private:
		GETResponse();
		bool mIsAutoIndex;
	protected:
		std::string createResponseHeader();
		std::string createResponseBody();
	public:
		static std::string const TAG;
		GETResponse(ServerManager &serverManager, const ServerConfig * serverConfig,
					const LocationConfig * locationConfig, bool autoIndex);
		GETResponse(GETResponse const & copy);
		GETResponse &operator=(GETResponse const & rhs);
		virtual ~GETResponse();


};

#endif