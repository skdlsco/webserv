#include "LocationConfig.hpp"

std::string const LocationConfig::TAG = "LocationConfig";

LocationConfig::LocationConfig()
: mCommonDirectives(), mAllowMethodList({"GET", "HEAD", "OPTIONS"}), mCGIExtensionList(),
	mCGIPath("")
{

}

LocationConfig::LocationConfig(LocationConfig const & copy)
{
	mCommonDirectives = copy.mCommonDirectives;
	mAllowMethodList = copy.mAllowMethodList;
	mCGIExtensionList = copy.mCGIExtensionList;
	mCGIPath = copy.mCGIPath;
}

LocationConfig &LocationConfig::operator=(LocationConfig const & rhs)
{
	if (this != &rhs)
	{
		LocationConfig(rhs);
	}
	return (*this);
}

LocationConfig::~LocationConfig()
{
	mAllowMethodList.clear();
	mCGIExtensionList.clear();
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

void LocationConfig::addAllowMethod(std::string allowMethod)
{
	mAllowMethodList.insert(allowMethod);
}

void LocationConfig::addCGIExtension(std::string CGIExtension)
{
	mCGIExtensionList.insert(CGIExtension);
}

void LocationConfig::setCGIPath(std::string CGIPath)
{
	mCGIPath = CGIPath;
}
