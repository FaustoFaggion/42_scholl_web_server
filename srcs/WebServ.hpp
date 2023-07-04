#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "main.hpp"
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include "ListenerSocket.hpp"
#include "FileParser.hpp"
#include "HttpResponse.hpp"
#include <map>

typedef struct client {
	int				fd;
	time_t			start_connection;
	std::string		response;
} t_client;

class WebServ : protected FileParser, ListenerSocket
{
	private:
		int										_fd_listener;

		std::map<int, t_client>		map_connections;
		int 						_efd;
		int							_nfds;
		struct epoll_event			_ev;
		struct epoll_event			_ep_event [MAX_CONNECTIONS];
		socklen_t 					_addrlen;
		struct sockaddr_storage		_client_saddr; // Can store a IPv4 and IPv6 struct

	public:
		WebServ();
		WebServ(char *file, std::string server_name);
		~WebServ();
	
		int				getFdListener() const;

		void	parse_file(char *file, std::string server_name);
		void	setup_server(int type);
		void	create_listener_socket();
		void	create_connections();
		void	run();
		
		void	delete_timeout_socket();
		void	accept_new_connection();
		void	receive_data(int i);
		void	response(int i);
};

#endif