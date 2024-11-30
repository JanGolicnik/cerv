#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void run_connection(int fd)
{
	char buffer[BUFFER_SIZE] = {0};

	while (1)
	{
		unsigned int bytes_read = read(fd, buffer, BUFFER_SIZE - 1);
		if (bytes_read <= 0)
		{
			printf("Client disconnected!\n");
			return;
		}

		buffer[bytes_read - 1] = '\0';

		const char *msg = "<h1>HEY</h1>\n";

		printf("%s", buffer);
		dprintf(fd, "HTTP/1.0 200 OK\n");
		dprintf(fd, "Content-Type: text/html\n");
		dprintf(fd, "Content-Length: %d\n\n", (int)strlen(msg));
		dprintf(fd, "%s", msg);
	}
}

int main()
{
#define return_and_close(msg, fd) \
	perror(msg);                  \
	close(fd);                    \
	return 1;

	// TODO: parse these from arguments
	const unsigned short PORT = 8001;

	int server_fd = 0;
	struct sockaddr_in address = {0};
	socklen_t address_len = sizeof(address);

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		return_and_close("ERROR: Failed to open socket\n", server_fd);
	}

	int sockopt_val = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &sockopt_val, sizeof(sockopt_val));

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	int bind_result = bind(server_fd, (struct sockaddr *)&address, address_len);
	if (bind_result < 0)
	{
		return_and_close("ERROR: Failed to bind to port\n", server_fd);
	}

	printf("Server is listening on port %d..\n", PORT);

	while (1)
	{
		if (listen(server_fd, 1) < 0)
		{
			return_and_close("ERROR: failed to listen for connections", server_fd);
		}

		int client_fd = accept(server_fd, (struct sockaddr *)&address, &address_len);
		if (client_fd < 0)
		{
			return_and_close("ERROR: Failed to accept connection\n", server_fd);
		}

		printf("Client connected!\n");

		run_connection(client_fd);

		close(client_fd);
	}

	close(server_fd);

	return 0;

#undef return_and_close
}