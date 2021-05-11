#include "CommonDirective.hpp"

std::string const CommonDirective::TAG = "CommonDirective";

CommonDirective::CommonDirective()
: mIndexFile(""), mRoot("/"), mIsAutoIndex(false)
{

}

CommonDirective::CommonDirective(CommonDirective const & copy)
{
	*this = copy;
}

CommonDirective &CommonDirective::operator=(CommonDirective const & rhs)
{
	if (this != &rhs)
	{
		this->mIndexFile = rhs.mIndexFile;
		this->mRoot = rhs.mRoot;
		this->mIsAutoIndex = rhs.mIsAutoIndex;
	}
	return (*this);
}

CommonDirective::~CommonDirective()
{

}

std::string CommonDirective::getIndexFile() const
{
	return (mIndexFile);
}

std::string CommonDirective::getRoot() const
{
	return (mRoot);
}


bool CommonDirective::isAutoIndex() const
{
	return (mIsAutoIndex);
}

void CommonDirective::setIndexFile(std::string indexFile)
{
	mIndexFile = indexFile;
}

void CommonDirective::setRoot(std::string root)
{
	mRoot = root;
}

void CommonDirective::setAutoIndex(bool isAutoIndex)
{
	mIsAutoIndex = isAutoIndex;
}