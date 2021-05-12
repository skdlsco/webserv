#ifndef CONFIG_VALIDATOR_HPP
# define CONFIG_VALIDATOR_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <exception>
# include "utils/Directive.hpp"
# include "utils/String.hpp"

class ConfigValidator
{
	private:
		std::string mFilePath;
		std::map<std::string, size_t> mCountServerDirective;
		std::map<std::string, size_t> mCountLocationDirective;
		std::vector<std::string> mEachConfigLine;

	public:
		static std::string const TAG;
		static std::string const DEFAULT_IP;
		static std::string const DEFAULT_PORT;
		static std::string const DEFAULT_SERVER_NAME;

		/* to avoid inaccess error */
		ConfigValidator();
		ConfigValidator(std::string const & filePath);
		ConfigValidator(ConfigValidator const & copy);
		ConfigValidator &operator=(ConfigValidator const & rhs);
		virtual ~ConfigValidator();

		bool isConfigValidate();

		bool isScopeMatched();
		bool isConfigSequenceMatched();
		bool isServerInfoAlreadyExisted();
		bool isLocationURIAlreadyExisted();

		bool isValidateServerDirective(size_t & lineIndex);
		bool isValidateLocationDirective(size_t & lineIndex);
		bool hasMandatoryDirective(size_t flag);
		bool hasEachDirectiveOnlyOne(size_t flag);

		void initializeCountServerDirective();
		void initializeCountLocationDirective();

		void readConfigFileByLine();

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