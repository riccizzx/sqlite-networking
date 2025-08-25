//RUN THE PROJECT WITH CMAKE
#include "socket.h"

using namespace server;

int main()
{
	Sv sv;
	try {
;		sv.Initialize();
		sv.Config();
		sv.handleClient();
		//void *handleRequest();
	}
	catch (const std::exception& e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
