#include <iostream>
#include <functional>
#include <string>

int main(int argc, char *argv[])
{
	std::string str;
	std::hash<std::string> h;
	while(std::cin>>str){
		size_t n = h(str);
		std::cout << n << std::endl;
	}
	return 0;
}
