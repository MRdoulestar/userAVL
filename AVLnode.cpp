#include"dog4u.h"
using namespace std;

//获取节点id
size_t AVLnode::getKey() {
	return key;
}
//设置节点id
void AVLnode::setKey(size_t key) {
	this->key = key;
}
//获取节点存储的用户名
string AVLnode::getUsername(){
	return username;
}
//设置用户名
void AVLnode::setUsername(string username) {
	this->username = username;
}
//获取节点存储的用户密码
string AVLnode::getPassword(){
	return password;
}
//设置用户密码
void AVLnode::setPassword(string password) {
	this->password = password;
}
