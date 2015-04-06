#pragma once
class lol2daeError : public exception
{
private:
	const char* err_msg;

public:
	const char* what() const throw() { return err_msg; };
	lol2daeError(const char* msg) : err_msg(msg) {};
	~lol2daeError() throw() {};
};

