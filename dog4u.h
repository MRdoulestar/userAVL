
//@author
//Xingxin YU

#include<iostream>
#include<string.h>
#include<cstring>
#include<vector>
#include<unistd.h>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>

#define USER_FILE "user.txt"



//工具类
class Utils {
	public:
	static std::vector<std::string> split(std::string, char dot);	//字符串分割
	static size_t hash(std::string str);	//字符串哈希成数字
};


//AVL节点类
class AVLnode {

	private:
	size_t key;	//节点id
	std::string username;	//节点中存储的用户名
	std::string password;	//节点中用户对应的密码

	public:
	int bf;			//节点平衡因子
	AVLnode *left;	//左子节点
	AVLnode *right;	//右子节点
	AVLnode *parent;//父节点
	
	AVLnode(size_t key, std::string  username, std::string password, AVLnode *left=NULL, AVLnode *right=NULL, AVLnode *parent=NULL):
		key(key), username(username), password(password), left(left), right(right), parent(parent){}
	AVLnode(){}
	
	size_t getKey();
	void setKey(size_t key);
	std::string getUsername();
	void setUsername(std::string username);
	std::string getPassword();
	void setPassword(std::string password);
};	


//AVL树类,包括对该树的所有操作
class AVL {
private:
	AVLnode *root;	//根节点
	//对AVL树的插入和删除
	void AVLinsert(AVLnode* node);
	void AVLremove(std::string username);
	
	
	//旋转操作
	void leftRotate(AVLnode* NBnode);
	void rightRotate(AVLnode* NBnode);
	void lrRotate(AVLnode* NBnode);
	void rlRotate(AVLnode* NBnode);

public:
	int height;	//树的高度
	unsigned long num;	//节点个数
	std::vector<AVLnode> vec;	//所有用户名数据
	//std::vector<std::string> vec_p;	//所有密码数据
	//AVL树的遍历
	void traversal(AVLnode* T);
	//用户查找
	std::string findUser(size_t key);
	AVLnode* getRoot();

	AVL();	//构造函数
	~AVL();	//析构函数
	
	//对用户的增删改查
	void search(std::string username);
	void insert(std::string username, std::string password);
	void remove(std::string username);
	

};

//程序初始化类
class Init {
private:
	int socket_fd, accept_fd;
	sockaddr_in myserver;
	sockaddr_in remote_addr;
public:
	AVL usertree;
	Init(int port);
	int recv_msg();
	int mysend(int *socket, char* buffer, int len);
	bool login(int socket);
};

