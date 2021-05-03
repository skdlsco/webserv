#ifndef COMMONDIRECTIVES_HPP
# define COMMONDIRECTIVES_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class CommonDirectives
{
	private:
		std::vector<std::string> mIndexList;
		std::string mRoot;
		std::string mClientMaxBodySize;
		bool mAutoIndex;
		
	public:
		static const std::string TAG;
		CommonDirectives();
		CommonDirectives(CommonDirectives const & copy);
		CommonDirectives &operator=(CommonDirectives const & rhs);
		virtual ~CommonDirectives();

		std::vector<std::string> getIndexList() const;
		std::string getRoot() const;
		std::string getClientMaxBodySize() const;
		bool isAutoIndex() const;

		void addIndex(std::string index);
		void setRoot(std::string root);
		void setClientMaxBodySize(std::string clientMaxBodySize);
		void setAutoIndex(bool autoIndex);
};

#endif