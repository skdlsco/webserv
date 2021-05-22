#include "LocationConfig.hpp"

std::string const LocationConfig::TAG = "LocationConfig";

LocationConfig::LocationConfig()
: mCommonDirective(), mAllowMethodList(), mCGIExtensionList(),
	mCGIPath(""), mAuthUserName(""), mAuthUserPassword("")
{
	/* to avoid nested name specifier */
	mAllowMethodList.push_back(web::method[web::GET]);
	mAllowMethodList.push_back(web::method[web::HEAD]);
	mAllowMethodList.push_back(web::method[web::OPTIONS]);
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

std::string const &LocationConfig::getIndexFile() const
{
	return (mCommonDirective.getIndexFile());
}

std::string const &LocationConfig::getRoot() const
{
	return (mCommonDirective.getRoot());
}

bool LocationConfig::isAutoIndex() const
{
	return (mCommonDirective.isAutoIndex());
}

std::vector<std::string> LocationConfig::getAllowMethodList() const
{
	return (mAllowMethodList);
}

std::vector<std::string> LocationConfig::getCGIExtensionList() const
{
	return (mCGIExtensionList);
}

std::string const &LocationConfig::getCGIPath() const
{
	return (mCGIPath);
}

std::string const &LocationConfig::getAuthUserName() const
{
	return (mAuthUserName);
}

std::string const &LocationConfig::getAuthUserPassword() const
{
	return (mAuthUserPassword);
}

void LocationConfig::setIndexFile(std::string const & indexFile)
{
	mCommonDirective.setIndexFile(indexFile);
}

void LocationConfig::setRoot(std::string const & root)
{
	mCommonDirective.setRoot(root);
}

void LocationConfig::setAutoIndex(bool autoIndex)
{
	mCommonDirective.setAutoIndex(autoIndex);
}

void LocationConfig::setAuthUserName(std::string const & userName)
{
	mAuthUserName = userName;
}

void LocationConfig::setAuthUserPassword(std::string const & userPassword)
{
	mAuthUserPassword = userPassword;
}

void LocationConfig::addAllowMethod(std::string const & allowMethod)
{
	mAllowMethodList.push_back(allowMethod);
}

void LocationConfig::addCGIExtension(std::string const & CGIExtension)
{
	mCGIExtensionList.push_back(CGIExtension);
}

void LocationConfig::setCGIPath(std::string CGIPath)
{
	mCGIPath = CGIPath;
}
