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

std::string const & LocationConfig::getIndexFile() const
{
	return (mCommonDirective.getIndexFile());
}

std::string const & LocationConfig::getRoot() const
{
	return (mCommonDirective.getRoot());
}

bool LocationConfig::isAutoIndex() const
{
	return (mCommonDirective.isAutoIndex());
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

void LocationConfig::setIndexFile(std::string const & indexFile)
{
	mCommonDirective.setIndexFile(indexFile);
}

void LocationConfig::setRoot(std::string const & root)
{
	mCommonDirective.setRoot(root);
}

void LocationConfig::setAutoIndex(bool isAutoIndex)
{
	mCommonDirective.setAutoIndex(isAutoIndex);
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
