#ifndef CONFIG_VALIDATOR_HPP
# define CONFIG_VALIDATOR_HPP

# include <iostream>
# include <string>

class ConfigValidator
{
	private:
		std::string mfilePath;
		ConfigValidator();

	public:
		ConfigValidator(std::string filePath);
		ConfigValidator(ConfigValidator const & copy);
		ConfigValidator &operator=(ConfigValidator const & rhs);
		virtual ~ConfigValidator();

		bool isScopeMatched();
		bool isConfigSequenceMatched();
		bool isHostAndPortOverlapped();
		bool isLocationURIOverlapped();

};

#endif