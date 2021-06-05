#ifndef FILE_HPP
# define FILE_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include "utils/String.hpp"

class File
{
	public:
		enum State
		{
			CONTENT_LEFT, CONTENT_NOT_LEFT, DONE
		};

	private:
		static const int BUFFER_SIZE = 1024;
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
		std::string getLine();

		int getFD() const;
		enum State getState() const;
		std::string const &getFilePath() const;
		std::string const &getBuffer() const;
		bool isStateDone() const;

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
