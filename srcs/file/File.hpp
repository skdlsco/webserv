#ifndef FILE_HPP
# define FILE_HPP

# define BUFFER_SIZE 1024
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include "utils/String.hpp"

class File
{
	enum State
	{
		CONTENT_LEFT, CONTENT_NOT_LEFT, DONE
	};

	private:
		int mFD;
		enum State mState;
		std::string mFilePath;
		std::string mBuffer;
		File();

	public:
		File(std::string const & filePath);
		File(File const & copy);
		File &operator=(File const & rhs);
		virtual ~File();

		void openFile();
		void closeFile();
		void readFile();
		//void writeFile();
		std::string getLine();

		int getFD();
		enum State getState();
		std::string const &getFilePath();
		bool isStateDone();

		class FileException : public std::exception
		{
			private:
				std::string mMessage;

			public:
				virtual ~FileException() throw();
				FileException(std::string message) throw();
				virtual const char* what() const throw();
		};
};

#endif