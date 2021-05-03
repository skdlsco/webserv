#include "CommonDirectives.hpp"

const std::string CommonDirectives::TAG = "CommonDirectives";

CommonDirectives::CommonDirectives()
: mRoot(""), mClientMaxBodySize(""), mAutoIndex(false)
{

}

CommonDirectives::CommonDirectives(CommonDirectives const & copy)
{
	this->mIndexList = copy.mIndexList;
	this->mRoot = copy.mRoot;
	this->mClientMaxBodySize = copy.mClientMaxBodySize;
	this->mAutoIndex = copy.mAutoIndex;
}

CommonDirectives &CommonDirectives::operator=(CommonDirectives const & rhs)
{
	if (this != &rhs)
	{
		CommonDirectives(rhs);
	}
	return (*this);
}

CommonDirectives::~CommonDirectives()
{

}

std::vector<std::string> CommonDirectives::getIndexList() const
{
	return (mIndexList);
}

std::string CommonDirectives::getRoot() const
{
	return (mRoot);
}

std::string CommonDirectives::getClientMaxBodySize() const
{
	return (mClientMaxBodySize);
}

bool CommonDirectives::isAutoIndex() const
{
	return (mAutoIndex);
}

void CommonDirectives::addIndex(std::string index)
{
	mIndexList.push_back(index);
}

void CommonDirectives::setRoot(std::string root)
{
	mRoot = root;
}

void CommonDirectives::setClientMaxBodySize(std::string clientMaxBodySize)
{
	mClientMaxBodySize = clientMaxBodySize;
}







