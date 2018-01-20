#include"dog4u.h"
using namespace std;

//字符串分割
vector<string> Utils::split(string str, char dot) {
	vector<string> resVec;
	if ("" == str) {
		return resVec;
	}
	vector<string> str_vec;
    //方便截取最后一段数据
    std::string strs = str + dot;
	size_t pos = strs.find(dot);
	size_t size = strs.size();

	while (pos != std::string::npos){
		std::string x = strs.substr(0,pos);
		resVec.push_back(x);
		strs = strs.substr(pos+1,size);
		pos = strs.find(dot);
	}
	
	return resVec;
}

//字符串哈希
size_t Utils::hash(string str = "?") {
	std::hash<string> h;
	return h(str);
}
