#pragma once

#ifdef  __cplusplus
extern "C"
{
#endif

int setupAddress(
  const char *host,
  const char *port,
	struct addrinfo **output,
	int socktype,
	int protocolFamily
);

void printAddressStructures(struct addrinfo *addr);
void printAddress(struct addrinfo *addr);

#ifdef  __cplusplus
};
#endif
