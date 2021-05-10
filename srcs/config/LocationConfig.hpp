#ifndef LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP

#include <iostream>
#include <string>
#include <set>
# include "CommonDirective.hpp"

class LocationConfig
{
	private:
		CommonDirective			mCommonDirective;
		std::set<std::string>	mAllowMethodList;
		std::set<std::string>	mCGIExtensionList;
		std::string				mCGIPath;

	public:
		static std::string const TAG;
		LocationConfig();
		LocationConfig(LocationConfig const & copy);
		LocationConfig &operator=(LocationConfig const & rhs);
		virtual ~LocationConfig();

		CommonDirective getCommonDirective() const;
		std::set<std::string> getAllowMethodList() const;
		std::set<std::string> getCGIExtensionList() const;
		std::string getCGIPath() const;

		void addAllowMethod(std::string allowMethod);
		void addCGIExtension(std::string CGIExtension);
		void setCGIPath(std::string CGIPath);
};

#endif