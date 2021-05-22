#ifndef COMMON_DIRECTIVE_HPP
# define COMMON_DIRECTIVE_HPP

#include <iostream>
#include <string>
#include <vector>

class CommonDirective
{
	private:
		std::string mIndexFile;
		std::string mRoot;
		bool mIsAutoIndex;
		
	public:
		static std::string const TAG;
		CommonDirective();
		CommonDirective(CommonDirective const & copy);
		CommonDirective &operator=(CommonDirective const & rhs);
		virtual ~CommonDirective();

		std::string const &getIndexFile() const;
		std::string const &getRoot() const;
		bool isAutoIndex() const;

		void setIndexFile(std::string const & indexFile);
		void setRoot(std::string const & root);
		void setAutoIndex(bool autoIndex);
};

#endif