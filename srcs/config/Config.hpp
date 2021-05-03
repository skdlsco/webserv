#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
# include "Location.hpp"
# include "CommonDirectives.hpp"

class Config
{
	private:
		int						mPort;
		std::string				mHost;
		bool					mDefaultServer;
		std::vector<Location *>	mLocations;
		CommonDirectives		mCommonDirectives;
		Config();
	
	public:
		static const std::string TAG;
		Config(int port, std::string const & host, bool isDefaultServer);
		Config(Config const & copy);
		Config &operator=(Config const & copy);
		virtual ~Config();

		/* getter & setter */
		/* make locations getter/setter? */
		int getPort() const;
		std::string getHost() const;
		bool isDefaultServer() const;

		void setPort(int port);
		void setHost(std::string const & host);
		void setDefaultServer(bool defaultServer);
		
		
};

#endif
