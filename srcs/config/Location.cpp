#include "Location.hpp"

Location::Location()
: mCGIPath(""), mCommonDirectives()
{

}

Location::Location(Location const & copy)
{
	mMethodList = copy.mMethodList;
	mCGIExtensionList = copy.mCGIExtensionList;
	mCGIPath = copy.mCGIPath;
	mCommonDirectives = copy.mCommonDirectives;
}

Location &Location::operator=(Location const & rhs)
{
	if (this != &rhs)
	{
		Location(rhs);
	}
	return (*this);
}

Location::~Location()
{

}

std::vector<std::string> Location::getMethodList() const
{
	return (mMethodList);
}

std::vector<std::string> Location::getCGIExtensionList() const
{
	return (mCGIExtensionList);
}

std::string Location::getCGIPath() const
{
	return (mCGIPath);
}

void Location::addMethod(std::string method)
{
	mMethodList.push_back(method);
}

void Location::addCGIExtension(std::string extension)
{
	mCGIExtensionList.push_back(extension);
}

void Location::setCGIPath(std::string path)
{
	mCGIPath = path;
}