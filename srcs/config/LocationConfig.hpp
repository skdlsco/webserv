#ifndef LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include "utils/Method.hpp"
#include "CommonDirective.hpp"

class LocationConfig
{
	private:
		CommonDirective mCommonDirective;
		std::vector<std::string> mAllowMethodList;
		std::vector<std::string> mCGIExtensionList;
		std::string mCGIPath;
		std::string mAuthUserName;
		std::string mAuthUserPassword;

	public:
		static std::string const TAG;
		LocationConfig();
		LocationConfig(LocationConfig const & copy);
		LocationConfig &operator=(LocationConfig const & rhs);
		virtual ~LocationConfig();

		std::string const &getIndexFile() const;
		std::string const &getRoot() const;
		bool isAutoIndex() const;

		std::vector<std::string> getAllowMethodList() const;
		std::vector<std::string> getCGIExtensionList() const;
		std::string const &getCGIPath() const;
		std::string const &getAuthUserName() const;
		std::string const &getAuthUserPassword() const;

		void setIndexFile(std::string const & indexFile);
		void setRoot(std::string const & root);
		void setAutoIndex(bool autoIndex);
		void setAuthUserName(std::string const & userName);
		void setAuthUserPassword(std::string const & userPassword);

		void addAllowMethod(std::string const & allowMethod);
		void addCGIExtension(std::string const & CGIExtension);
		void setCGIPath(std::string CGIPath);
};

#endif