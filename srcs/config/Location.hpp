#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
# include "CommonDirectives.hpp"

class Location
{
	private:
		std::vector<std::string>	mMethodList;
		std::vector<std::string>	mCGIExtensionList;
		std::string					mCGIPath;
		CommonDirectives			mCommonDirectives;

	public:
		Location();
		Location(Location const & copy);
		Location &operator=(Location const & rhs);
		virtual ~Location();

		std::vector<std::string> getMethodList() const;
		std::vector<std::string> getCGIExtensionList() const;
		std::string getCGIPath() const;

		void addMethod(std::string method);
		void addCGIExtension(std::string extension);
		void setCGIPath(std::string path);
};

#endif