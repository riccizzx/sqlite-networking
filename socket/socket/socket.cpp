//RUN THE PROJECT WITH CMAKE
#include "socket.h"

int main()
{
	Sv sv;
	try {
		sv.Initialize();
		sv.Config();
	}
	catch (const std::exception& e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
