#include "CommonDirectives.hpp"

std::string const CommonDirectives::TAG = "CommonDirectives";

CommonDirectives::CommonDirectives()
: mIndexFile(""), mRoot("/"), mAutoIndex(false)
{

}

CommonDirectives::CommonDirectives(CommonDirectives const & copy)
{
	this->mIndexFile = copy.mIndexFile;
	this->mRoot = copy.mRoot;
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

std::string CommonDirectives::getIndexFile() const
{
	return (mIndexFile);
}

std::string CommonDirectives::getRoot() const
{
	return (mRoot);
}


bool CommonDirectives::isAutoIndex() const
{
	return (mAutoIndex);
}

void CommonDirectives::setIndexFile(std::string indexFile)
{
	mIndexFile = indexFile;
}

void CommonDirectives::setRoot(std::string root)
{
	mRoot = root;
}

void CommonDirectives::setAutoIndex(bool autoIndex)
{
	mAutoIndex = autoIndex;
}