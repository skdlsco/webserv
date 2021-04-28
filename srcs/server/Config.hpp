#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>

class Config
{
	private:
		int mPort;

		Config();
	public:
		Config(int port);
		Config(Config const & copy);
		Config &operator=(Config const & copy);
		virtual ~Config();

		int getPort() const;
};

#endif
