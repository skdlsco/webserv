#ifndef COMMON_DIRECTIVES_HPP
# define COMMON_DIRECTIVES_HPP

#include <iostream>
#include <string>
#include <vector>

class CommonDirectives
{
	private:
		std::string mIndexFile;
		std::string mRoot;
		bool mAutoIndex;
		
	public:
		static std::string const TAG;
		CommonDirectives();
		CommonDirectives(CommonDirectives const & copy);
		CommonDirectives &operator=(CommonDirectives const & rhs);
		virtual ~CommonDirectives();

		std::string getIndexFile() const;
		std::string getRoot() const;
		bool isAutoIndex() const;

		void setIndexFile(std::string indexFile);
		void setRoot(std::string root);
		void setAutoIndex(bool autoIndex);
};

#endif