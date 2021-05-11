#include "LocationConfig.hpp"

std::string const LocationConfig::TAG = "LocationConfig";

LocationConfig::LocationConfig()
: mCommonDirective(), mAllowMethodList(), mCGIExtensionList(), mCGIPath("")
{
	/* to avoid  nested name specifier */
	mAllowMethodList.insert(web::method[web::GET]);
	mAllowMethodList.insert(web::method[web::HEAD]);
	mAllowMethodList.insert(web::method[web::OPTIONS]);
}

LocationConfig::LocationConfig(LocationConfig const & copy)
{
	*this = copy;
}

LocationConfig &LocationConfig::operator=(LocationConfig const & rhs)
{
	if (this != &rhs)
	{
		mCommonDirective = rhs.mCommonDirective;
		mAllowMethodList = rhs.mAllowMethodList;
		mCGIExtensionList = rhs.mCGIExtensionList;
		mCGIPath = rhs.mCGIPath;
	}
	return (*this);
}

LocationConfig::~LocationConfig()
{
	mAllowMethodList.clear();
	mCGIExtensionList.clear();
}

CommonDirective LocationConfig::getCommonDirective() const
{
	return (mCommonDirective);
}

std::set<std::string> LocationConfig::getAllowMethodList() const
{
	return (mAllowMethodList);
}

std::set<std::string> LocationConfig::getCGIExtensionList() const
{
	return (mCGIExtensionList);
}

std::string LocationConfig::getCGIPath() const
{
	return (mCGIPath);
}

void LocationConfig::addAllowMethod(std::string const & allowMethod)
{
	mAllowMethodList.insert(allowMethod);
}

void LocationConfig::addCGIExtension(std::string const & CGIExtension)
{
	mCGIExtensionList.insert(CGIExtension);
}

void LocationConfig::setCGIPath(std::string CGIPath)
{
	mCGIPath = CGIPath;
}
