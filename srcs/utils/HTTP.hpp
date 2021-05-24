#ifndef HTTP_HPP
# define HTTP_HPP

#include <string>

namespace web
{
	std::string getStatusMessage(int statusCode);
	std::string getErrorPage(int statusCode);

	std::string const STATUS_MESSAGE_100 = "Continue";
	std::string const ERROR_PAGE_100 =
	"<html>\r\n<head><title>Continue</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Continue</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_101 = "Switching Protocols";
	std::string const ERROR_PAGE_101 =
	"<html>\r\n<head><title>Switching Protocols</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Switching Protocols</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_103 = "Early Hints";
	std::string const ERROR_PAGE_103 =
	"<html>\r\n<head><title>Early Hints</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Early Hints</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_200 = "OK";
	std::string const ERROR_PAGE_200 =
	"<html>\r\n<head><title>OK</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>OK</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_201 = "Created";
	std::string const ERROR_PAGE_201 =
	"<html>\r\n<head><title>Created</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Created</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_202 = "Accepted";
	std::string const ERROR_PAGE_202 =
	"<html>\r\n<head><title>Accepted</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Accepted</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_203 = "Non-Authoritative Information";
	std::string const ERROR_PAGE_203 =
	"<html>\r\n<head><title>Non-Authoritative Information</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Non-Authoritative Information</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_204 = "No Content";
	std::string const ERROR_PAGE_204 =
	"<html>\r\n<head><title>No Content</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>No Content</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_205 = "Reset Content";
	std::string const ERROR_PAGE_205 =
	"<html>\r\n<head><title>Reset Content</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Reset Content</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_206 = "Partial Content";
	std::string const ERROR_PAGE_206 =
	"<html>\r\n<head><title>Partial Content</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Partial Content</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_300 = "Multiple Choices";
	std::string const ERROR_PAGE_300 =
	"<html>\r\n<head><title>Multiple Choices</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Multiple Choices</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_301 = "Moved Permanently";
	std::string const ERROR_PAGE_301 =
	"<html>\r\n<head><title>Moved Permanently</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Moved Permanently</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_302 = "Found";
	std::string const ERROR_PAGE_302 =
	"<html>\r\n<head><title>Found</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Found</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_303 = "See Other";
	std::string const ERROR_PAGE_303 =
	"<html>\r\n<head><title>See Other</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>See Other</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_304 = "Not Modified";
	std::string const ERROR_PAGE_304 =
	"<html>\r\n<head><title>Not Modified</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Not Modified</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_307 = "Temporary Redirect";
	std::string const ERROR_PAGE_307 =
	"<html>\r\n<head><title>Temporary Redirect</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Temporary Redirect</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_308 = "Permanent Redirect";
	std::string const ERROR_PAGE_308 =
	"<html>\r\n<head><title>Permanent Redirect</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Permanent Redirect</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_400 = "Bad Request";
	std::string const ERROR_PAGE_400 =
	"<html>\r\n<head><title>Bad Request</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Bad Request</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_401 = "Unauthorized";
	std::string const ERROR_PAGE_401 =
	"<html>\r\n<head><title>Unauthorized</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Unauthorized</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_402 = "Payment Required";
	std::string const ERROR_PAGE_402 =
	"<html>\r\n<head><title>Payment Required</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Payment Required</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_403 = "Forbidden";
	std::string const ERROR_PAGE_403 =
	"<html>\r\n<head><title>Forbidden</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Forbidden</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_404 = "Not Found";
	std::string const ERROR_PAGE_404 =
	"<html>\r\n<head><title>Not Found</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Not Found</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_405 = "Method Not Allowed";
	std::string const ERROR_PAGE_405 =
	"<html>\r\n<head><title>Method Not Allowed</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Method Not Allowed</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_406 = "Not Acceptable";
	std::string const ERROR_PAGE_406 =
	"<html>\r\n<head><title>Not Acceptable</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Not Acceptable</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_407 = "Proxy Authentication Required";
	std::string const ERROR_PAGE_407 =
	"<html>\r\n<head><title>Proxy Authentication Required</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Proxy Authentication Required</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_408 = "Request Timeout";
	std::string const ERROR_PAGE_408 =
	"<html>\r\n<head><title>Request Timeout</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Request Timeout</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_409 = "Conflict";
	std::string const ERROR_PAGE_409 =
	"<html>\r\n<head><title>Conflict</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Conflict</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_410 = "Gone";
	std::string const ERROR_PAGE_410 =
	"<html>\r\n<head><title>Gone</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Gone</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_411 = "Length Required";
	std::string const ERROR_PAGE_411 =
	"<html>\r\n<head><title>Length Required</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Length Required</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_412 = "Precondition Failed";
	std::string const ERROR_PAGE_412 =
	"<html>\r\n<head><title>Precondition Failed</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Precondition Failed</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_413 = "Payload Too Large";
	std::string const ERROR_PAGE_413 =
	"<html>\r\n<head><title>Payload Too Large</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Payload Too Large</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_414 = "URI Too Long";
	std::string const ERROR_PAGE_414 =
	"<html>\r\n<head><title>URI Too Long</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>URI Too Long</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_415 = "Unsupported Media Type";
	std::string const ERROR_PAGE_415 =
	"<html>\r\n<head><title>Unsupported Media Type</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Unsupported Media Type</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_416 = "Range Not Satisfiable";
	std::string const ERROR_PAGE_416 =
	"<html>\r\n<head><title>Range Not Satisfiable</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Range Not Satisfiable</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_417 = "Expectation Failed";
	std::string const ERROR_PAGE_417 =
	"<html>\r\n<head><title>Expectation Failed</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Expectation Failed</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_418 = "I'm a teapot";
	std::string const ERROR_PAGE_418 =
	"<html>\r\n<head><title>I'm a teapot</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>I'm a teapot</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_422 = "Unprocessable Entity";
	std::string const ERROR_PAGE_422 =
	"<html>\r\n<head><title>Unprocessable Entity</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Unprocessable Entity</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_425 = "Too Early";
	std::string const ERROR_PAGE_425 =
	"<html>\r\n<head><title>Too Early</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Too Early</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_426 = "Upgrade Required";
	std::string const ERROR_PAGE_426 =
	"<html>\r\n<head><title>Upgrade Required</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Upgrade Required</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_428 = "Precondition Required";
	std::string const ERROR_PAGE_428 =
	"<html>\r\n<head><title>Precondition Required</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Precondition Required</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_429 = "Too Many Requests";
	std::string const ERROR_PAGE_429 =
	"<html>\r\n<head><title>Too Many Requests</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Too Many Requests</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_431 = "Request Header Fields Too Large";
	std::string const ERROR_PAGE_431 =
	"<html>\r\n<head><title>Request Header Fields Too Large</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Request Header Fields Too Large</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_451 = "Unavailable For Legal Reasons";
	std::string const ERROR_PAGE_451 =
	"<html>\r\n<head><title>Unavailable For Legal Reasons</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Unavailable For Legal Reasons</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_500 = "Internal Server Error";
	std::string const ERROR_PAGE_500 =
	"<html>\r\n<head><title>Internal Server Error</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Internal Server Error</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_501 = "Not Implemented";
	std::string const ERROR_PAGE_501 =
	"<html>\r\n<head><title>Not Implemented</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Not Implemented</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_502 = "Bad Gateway";
	std::string const ERROR_PAGE_502 =
	"<html>\r\n<head><title>Bad Gateway</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Bad Gateway</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_503 = "Service Unavailable";
	std::string const ERROR_PAGE_503 =
	"<html>\r\n<head><title>Service Unavailable</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Service Unavailable</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_504 = "Gateway Timeout";
	std::string const ERROR_PAGE_504 =
	"<html>\r\n<head><title>Gateway Timeout</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Gateway Timeout</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_505 = "HTTP Version Not Supported";
	std::string const ERROR_PAGE_505 =
	"<html>\r\n<head><title>HTTP Version Not Supported</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>HTTP Version Not Supported</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_506 = "Variant Also Negotiates";
	std::string const ERROR_PAGE_506 =
	"<html>\r\n<head><title>Variant Also Negotiates</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Variant Also Negotiates</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_507 = "Insufficient Storage";
	std::string const ERROR_PAGE_507 =
	"<html>\r\n<head><title>Insufficient Storage</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Insufficient Storage</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_508 = "Loop Detected";
	std::string const ERROR_PAGE_508 =
	"<html>\r\n<head><title>Loop Detected</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Loop Detected</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_510 = "Not Extended";
	std::string const ERROR_PAGE_510 =
	"<html>\r\n<head><title>Not Extended</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Not Extended</h1></center>\r\n" \
	"</body></html>\r\n";
	std::string const STATUS_MESSAGE_511 = "Network Authentication Required";
	std::string const ERROR_PAGE_511 =
	"<html>\r\n<head><title>Network Authentication Required</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Network Authentication Required</h1></center>\r\n" \
	"</body></html>\r\n";

	std::string const STATUS_MESSAGE_UNDEFINED = "Undefined";
	std::string const ERROR_PAGE_UNDEFINED =
	"<html>\r\n<head><title>Undefined</title></head>\r\n" \
	"<body>\r\n" \
	"<center><h1>Undefined</h1></center>\r\n" \
	"</body></html>\r\n";
};

#endif
