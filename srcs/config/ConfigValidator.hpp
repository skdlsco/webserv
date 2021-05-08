#ifndef CONFIG_VALIDATOR_HPP
# define CONFIG_VALIDATOR_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <map>
# include <vector>
# include <exception>
# include "../utils/Directive.hpp"

class ConfigValidator
{
	private:
		size_t mLineIndex;
		std::string mFilePath;
		std::map<std::string, size_t> mCountServerDirective;
		std::map<std::string, size_t> mCountLocationDirective;
		std::vector<std::string> mEachConfigLine;
		ConfigValidator();

	public:
		static std::string const TAG;
		ConfigValidator(std::string filePath);
		ConfigValidator(ConfigValidator const & copy);
		ConfigValidator &operator=(ConfigValidator const & rhs);
		virtual ~ConfigValidator();

		bool isConfigValidate();
		void configSplitByEnter();
		bool isScopeMatched();
		bool isConfigSequenceMatched();
		bool isHostAndPortPairAlreadyExisted();
		bool isLocationURIAlreadyExisted();

		void initializeCountServerDirective();
		void initializeCountLocationDirective();

		class ConfigValidatorException : public std::exception
		{
			private:
				std::string mMessage;

			public:
				virtual ~ConfigValidatorException() throw();
				ConfigValidatorException(std::string message) throw();
				virtual const char* what() const throw();
		};

};

#endif 