#include "dog4u.h"
#define MAXSIZE 1024

//初始化socket监听
Init::Init(int port) {
	if(( socket_fd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 ){  
	   throw "socket() failed";  
    }  
	  
    memset(&myserver,0,sizeof(myserver));  
    myserver.sin_family = AF_INET;  
    myserver.sin_addr.s_addr = htonl(INADDR_ANY);  
    myserver.sin_port = htons(port);

    if( bind(socket_fd,(sockaddr*) &myserver,sizeof(myserver)) < 0 ) {  
	   throw "bind() failed";  
    }  
			  
    if( listen(socket_fd,10) < 0 ) {  
       throw "listen() failed";  
    }  

}

//用户登录验证
bool Init::login(int socket) {
	std::string username;
	std::string password;
	char buffer[MAXSIZE];
	char send_first[] = "请输入用户名:\n";
	char send_second[] = "请输入密码:\n";

	send(socket, send_first, strlen(send_first), 0); //发送提示消息用户名
	memset(buffer,0,MAXSIZE);	//清空缓冲区
	if( read(socket,buffer,MAXSIZE) < 0){
		throw("Read() error");
	} else {
		username = buffer;	//char[] 转 string
		if(username.size()==0) {	//如果用户强制退出，进程关闭
			std::cout<<"User quit success"<<"\n";
			exit(0);
		}
	}
	send(socket, send_second, strlen(send_second), 0); //发送提示消息密码
	memset(buffer,0,MAXSIZE);	//清空缓冲区
	if( read(socket,buffer,MAXSIZE) < 0){
		throw("Read() error");
	} else {
		password = buffer;	//char[] 转 string
		if(password.size()==0) {	//如果用户强制退出，进程关闭
			std::cout<<"User quit success"<<"\n";
			exit(0);
		}
	}
	//判断用户名和密码
	username = username.substr(0,username.size()-1);
	password = password.substr(0,password.size()-1);
	if(username.compare("admin") == 0 && password.compare("admin") == 0) {
		return true;
	}else {
		return false;
	}
}

//自定义命令执行和回复
int Init::mysend(int* socket, char* buffer, int len) {
	char l_buffer[MAXSIZE];
	char* cp;
	std::string res;
	std::string cmd = buffer;
	std::string username;
	std::string password;
	cmd = cmd.substr(0,len);
	//命令帮助
	if(cmd.compare("help") == 0 || cmd.compare("h") == 0) {
		res = "chpassword:更换用户密码\nadduser:添加用户\ndeluser:删除用户\n";
	}else if(cmd.compare("users") == 0) {
		usertree.traversal(usertree.getRoot());	//服务端查看所有用户
		res = "ok\n";
	}else if(cmd.compare("adduser") == 0) {	//添加用户
		char p1[] = "请输入要添加的用户名:\n";
		char p2[] = "请输入用户密码:\n";
		send(*socket, p1, strlen(p1), 0);
		memset(l_buffer,0,MAXSIZE);	//清空缓冲区
		if( read(*socket,l_buffer,MAXSIZE) < 0){
			throw("Read() error");
		} else {
			username = l_buffer;	//char[] 转 string
			if(username.size()==0) {	//如果用户强制退出，进程关闭
				std::cout<<"User quit success"<<"\n";
				exit(0);
			}
		}

		send(*socket, p2, strlen(p2), 0);
		memset(l_buffer,0,MAXSIZE);	//清空缓冲区
		if( read(*socket,l_buffer,MAXSIZE) < 0){
			throw("Read() error");
		} else {
			password = l_buffer;	//char[] 转 string
			if(password.size()==0) {	//如果用户强制退出，进程关闭
				std::cout<<"User quit success"<<"\n";
				exit(0);
			}
		}
		usertree.insert(username, password);
		res="添加成功!\n";
	}
	cp = new char[res.length()+1];
	res.copy(cp, res.length(), 0);
	send(*socket, cp, strlen(cp), 0);
	memset(cp,0,res.length()+1);	//清空缓冲区
	delete cp;
	return 0;
}

//接收客户端命令
int Init::recv_msg() {
	while( 1 ) {
		socklen_t sin_size = sizeof(struct sockaddr_in);
		if(( accept_fd = accept(socket_fd,(struct sockaddr*) &remote_addr,&sin_size)) == -1 ) {
			throw "Accept error";
			continue;
		}
		std::cout<<"有客户端连接："<<(char*) inet_ntoa(remote_addr.sin_addr)<<"\n";
		
		if(!fork()) {
			char buffer[MAXSIZE];
			std::string str;	//接收消息的字符串变量
			char prefix[] = ">>>";
			while(!login(accept_fd)){}	//首先进行用户登录
			while( 1 ) {	//循环接收消息
				send(accept_fd, prefix, strlen(prefix), 0);
				memset(buffer,0,MAXSIZE);	//清空缓冲区
				if( read(accept_fd,buffer,MAXSIZE) < 0){
					throw("Read() error");
				} else {
					str = buffer;	//char[] 转 string
					if(str.size()==0) {	//如果用户强制退出，进程关闭
						std::cout<<"User quit success"<<"\n";
						exit(0);
					}
					std::cout<<"收到消息："<<str<<"\n";
					std::cout<<"长度："<<str.size()<<"\n";
					mysend(&accept_fd, buffer, str.size()-1);	//自定义回复函数
					//break;
				}
				if(str.substr(0,str.size()-1).compare("quit") == 0 || str.substr(0,str.size()-1).compare("exit") == 0 ) {
					std::cout<<"User quit success"<<"\n";
					exit(0);
					//break;
				}
			}
			//exit(0);
		}

	}
	close(accept_fd);
	return 0;
}
