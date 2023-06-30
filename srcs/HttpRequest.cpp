#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{

}

HttpRequest::~HttpRequest()
{

}

std::string		HttpRequest::getMethod() const
{
	return (_method);
}

std::string		HttpRequest::getUrl() const
{
	return (_url);
}

std::string		HttpRequest::getProtocol() const
{
	return(_protocol);
}

std::string		HttpRequest::getContentType() const
{
	return (_content_type);
}

std::vector<std::string>	HttpRequest::getCgiEnvs() const
{
	return (_cgi_envs);
}

std::string	parse_line(std::string &request, std::string start, std::string end)
{
	size_t		requestLineEnd;
	size_t		requestLineStart;
	std::string requestLine;
	
	// Find the start and end of the request line
	requestLineStart = request.find(start);
	if (requestLineStart != request.npos)
	{
		requestLineEnd = request.find(end, requestLineStart);
		requestLine = request.substr(requestLineStart,(requestLineEnd - requestLineStart));
	}
	else
		requestLine = "";

	return (requestLine);
}

void	HttpRequest::add_cgi_envs(std::string request, std::string str, std::string env, size_t size, char c)
{
	std::string	requestLine;
	std::string	value;

	requestLine = parse_line(request, str, "\r\n");
	if (requestLine != "")
	{
		std::istringstream iss(requestLine);
		getline(iss, value, c);
		getline(iss, value, c);
		_cgi_envs.push_back(requestLine.replace(0, size, env));
	}
	else
	{
		value += env + "NULL";
		_cgi_envs.push_back(value);
	}
}

void	HttpRequest::cgi_envs_parser(std::string request, std::string html)
{
	std::cout << "\nCGI_ENVS_PARSER FUNCTION\n\n";
	(void)request;

	std::string		requestLine;
	std::string		key;
	std::string		value;
	std::string		saved;

	std::cout << request << "\n";
	std::cout << "enviroment variables\n" << "\n";
	
	/*SET REDIRECT_STATUS ENV*/
	// setenv("REDIRECT_STATUS", "true", 1);
	_cgi_envs.push_back("REDIRECT_STATUS=TRUE");
	std::cout << *(_cgi_envs.end() - 1) << "\n";

	/* SSL HAS TO BE CHECKED INTO THE FILE PARSER*/
	// _cgi_envs.push_back("AUTH_TYPE=NULL");
	// setenv("AUTY_TYPE", "NULL", 1);
	_cgi_envs.push_back("AUTY_TYPE=NULL");
	std::cout << *(_cgi_envs.end() - 1) << "\n";
	
	/*CONTENT_LENGTH*/
	key = "Content-Length: ";
	add_cgi_envs(request, key, "CONTENT_LENGTH=", key.size(), ' ');
	std::cout << *(_cgi_envs.end() - 1) << "\n";
	
	/*CONTENT_TYPE*/
	requestLine = parse_line(request, "Content-Type: ", "\r\n");
	if (requestLine != "")
	{
		std::string			tmp0, tmp1;
		std::istringstream	iss2(requestLine);
		getline(iss2, tmp0, ' ');
		getline(iss2, tmp0, ' ');
		std::istringstream	iss3(tmp0);
		getline(iss3, tmp1, ';');
		value = "CONTENT_TYPE=" + tmp1;
		_cgi_envs.push_back(value);	
	}
	std::cout << *(_cgi_envs.end() - 1) << "\n";

	/*GATWAY_INTERFACE*/ //?????????????????????????????????
	_cgi_envs.push_back("GATWAY_INTERFACE=CGI/7.4");

	/*HTTP_ACCEPT*/
	key = "Accept: ";
	add_cgi_envs(request, key, "HTTP_ACCEPT=", key.size(), ' ');
	std::cout << *(_cgi_envs.end() - 1) << "\n";

	/*HTTP_ACCEPT_ENCODING*/
	key = "Accept-Encoding: ";
	add_cgi_envs(request, key, "HTTP_ACCEPT_ENCODING=", key.size(), ' ');
	std::cout << *(_cgi_envs.end() - 1) << "\n";

	/*HTTP_ACCEPT_LANGUAGE*/
	key = "Accept-Language: ";
	add_cgi_envs(request, key, "HTTP_ACCEPT_LANGUAGE=", key.size(), ' ');
	std::cout << *(_cgi_envs.end() - 1) << "\n";

	/*HTTP_HOST, SERVER_NAME, SERVER_PORT*/
	requestLine = parse_line(request, "Host: ", "\r\n");
	if (requestLine != "")
	{
		std::string		tmp0, tmp1;
		std::istringstream	iss0(requestLine);
		getline(iss0, tmp0, ' ');
		getline(iss0, tmp0, ' ');
		value = "HTTP_HOST=" + tmp0;
		_cgi_envs.push_back(value);
		std::cout << *(_cgi_envs.end() - 1) << "\n";
		std::istringstream	iss1(tmp0);
		getline(iss1, tmp1, ':');
		// value = "SERVER_NAME=localhost";
		// _cgi_envs.push_back(value);
		// std::cout << *(_cgi_envs.end() - 1) << "\n";
		getline(iss1, tmp1, ':');
		value = "SERVER_PORT=" + tmp1;
		_cgi_envs.push_back(value);
		std::cout << *(_cgi_envs.end() - 1) << "\n";
	}
	else
	{
		std::cout << "requestline == vazio\n";
		_cgi_envs.push_back("HTTP_HOST=NULL");
		std::cout << *(_cgi_envs.end() - 1) << "\n";
		// _cgi_envs.push_back("SERVER_NAME=NULL");
		// std::cout << *(_cgi_envs.end() - 1) << "\n";
		_cgi_envs.push_back("SERVER_PORT=NULL");
		std::cout << *(_cgi_envs.end() - 1) << "\n";
	}

	/*HTTP_USER_AGENT*/
	key = "User-Agent: ";
	add_cgi_envs(request, key, "HTTP_USER_AGENT=", key.size(), ' ');
	std::cout << *(_cgi_envs.end() - 1) << "\n";

	/*PATH_INFO, QUERY_STRING*/
	size_t	end = request.find("\r\n");
	requestLine = request.substr(0, end);
	{
		std::string	mthd, addr, prtl, tmp0, tmp1, tmp2;
		std::cout << "requestLine: " << requestLine << "\n";
		std::istringstream iss0(requestLine);
		iss0 >> mthd >> addr >> prtl;
		std::cout << "_URL: " << addr << "\n";
		size_t pos = addr.find("?");
		if (pos != addr.npos)
		{
			size_t start = addr.find("/");
			size_t end = addr.find("?");
			value = addr.substr(start, (end - start));
			key = "PATH_INFO=" + value;
			saved = value;
			_cgi_envs.push_back(key);
			std::cout << *(_cgi_envs.end() - 1) << "\n";
			std::istringstream iss2(addr);
			getline(iss2, tmp0, '?');
			getline(iss2, tmp0, '?');
			key = "QUERY_STRING=" + tmp0;
			_cgi_envs.push_back(key);
			std::cout << *(_cgi_envs.end() - 1) << "\n";
		}
		else
		{
			size_t start =_url.find("/");
			value = _url.substr(start, (_url.size() - start));
			key = "PATH_INFO=" + value;
			_cgi_envs.push_back(key);
			std::cout << *(_cgi_envs.end() - 1) << "\n";
		}
		// key = "REQUEST_METHOD=" + mthd;
		// _cgi_envs.push_back(key);
		// std::cout << *(_cgi_envs.end() - 1) << "\n";
		key = "SERVER_PROTOCOL=" + prtl;
		_cgi_envs.push_back(key);
		std::cout << *(_cgi_envs.end() - 1) << "\n";
	}

	requestLine = parse_line(request, "Origin: ", "\r\n");
	if (requestLine != "")
	{
		std::istringstream iss(requestLine);
		getline(iss, value, ' ');
		getline(iss, value, ' ');
		key = "REMOTE_HOST=" + value;
		_cgi_envs.push_back(key);
	}
	else
		_cgi_envs.push_back("REMOTE_HOST=NULL");

	/*SCRIPT_NAME*/
	key = "SCRIPT_NAME=" + html;
	_cgi_envs.push_back(key);
	std::cout << *(_cgi_envs.end() - 1) << "\n";
	
	/*SCRIPT_FILENAME*/
	// key = "SCRIPT_FILENAME=" + saved;
	// _cgi_envs.push_back(key);

	// setenv("REMOTE_ADDR", "127.0.0.1", 1);

	key = "DOCUMENT_ROOT=/home/fausto/42SP/webserv_git";
	_cgi_envs.push_back(key);
	std::cout << *(_cgi_envs.end() - 1) << "\n";
}

void		HttpRequest::request_parser(std::string request)
{

	std::cout << "\nREQUEST_PARSER FUNCTION\n";

	std::string		tmp;
	std::string		requestLine;

	/*METHOD, URL, PROTOCOL*/
	size_t requestLineEnd = request.find("\r\n");
	requestLine = request.substr(0, requestLineEnd);
	std::istringstream iss0(requestLine);
	iss0 >> _method >> _url >> _protocol;

	size_t pos = _url.find("?");
	if (pos != _url.npos)
	{
		std::istringstream iss0(_url);
		getline(iss0, _url, '?');
	}

	/*CONTENT_TYPE*/
	pos = request.find("Content-Type: ");
	if (pos != request.npos)
	{
		requestLine = parse_line(request, "Content-Type: ", "\r\n");
		std::istringstream iss0(requestLine);
		getline(iss0, tmp, ' ');
		getline(iss0, tmp, ' ');
		std::istringstream iss1(tmp);
		getline(iss1, _content_type, ';');
		_content_type += "\r\n";
	}

	// cgi_envs_parser(request);
}