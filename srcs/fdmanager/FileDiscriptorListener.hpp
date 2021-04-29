#ifndef FILE_DISCRIPTOR_LISTENER_HPP
# define FILE_DISCRIPTOR_LISTENER_HPP

class FileDiscriptorListener
{
	public:
		virtual ~FileDiscriptorListener() {}
		virtual void onReadSet() = 0;
		virtual void onWriteSet() = 0;
		virtual void onExceptSet() = 0;
};

#endif
