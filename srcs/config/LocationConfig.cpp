#include "LocationConfig.hpp"

std::string const LocationConfig::TAG = "LocationConfig";

LocationConfig::LocationConfig()
: mCommonDirective(), mAllowMethodList(), mCGIExtensionList(),
	mCGIPath(""), mAuthUserName(""), mAuthUserPassword(""), mURI("")
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
		mCGIMethodList = rhs.mCGIMethodList;
		mCGIExtensionList = rhs.mCGIExtensionList;
		mCGIPath = rhs.mCGIPath;
		mURI = rhs.mURI;
	}
	return (*this);
}

LocationConfig::~LocationConfig()
{
	mAllowMethodList.clear();
	mCGIMethodList.clear();
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

size_t LocationConfig::getClientMaxBodySize() const
{
	return (mCommonDirective.getClientMaxBodySize());
}

bool LocationConfig::isAutoIndex() const
{
	return (mCommonDirective.isAutoIndex());
}

std::vector<std::string> LocationConfig::getAllowMethodList() const
{
	return (mAllowMethodList);
}

std::vector<std::string> LocationConfig::getCGIMethodList() const
{
	return (mCGIMethodList);
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

std::string const &LocationConfig::getURI() const
{
	return (mURI);
}

void LocationConfig::setIndexFile(std::string const & indexFile)
{
	mCommonDirective.setIndexFile(indexFile);
}

void LocationConfig::setRoot(std::string const & root)
{
	mCommonDirective.setRoot(root);
}

void LocationConfig::setClientMaxBodySize(size_t clientMaxBodySize)
{
	mCommonDirective.setClientMaxBodySize(clientMaxBodySize);
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

void LocationConfig::setURI(std::string const & URI)
{
	mURI = URI;
}

void LocationConfig::addAllowMethod(std::string const & allowMethod)
{
	mAllowMethodList.push_back(allowMethod);
}

void LocationConfig::addCGIMethod(std::string const & CGIMethod)
{
	mCGIMethodList.push_back(CGIMethod);
}

void LocationConfig::addCGIExtension(std::string const & CGIExtension)
{
	mCGIExtensionList.push_back(CGIExtension);
}

void LocationConfig::setCGIPath(std::string CGIPath)
{
	mCGIPath = CGIPath;
}
