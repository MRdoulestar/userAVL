#include"dog4u.h"
#include<fstream>
using namespace std;

//拿到根节点地址
AVLnode* AVL::getRoot() {
	return root;
}

//AVL树遍历
void AVL::traversal(AVLnode* T) {
	if(T) {
		std::cout<<T->getKey()<<"\n";
		std::cout<<T->getUsername()<<"\n";
		std::cout<<T->getPassword()<<"\n";
		traversal(T->left);
		traversal(T->right);
	}
}

//返回用户Key的密码
string AVL:: findUser(size_t key) {

	return "password";
}

//AVL树左旋
void AVL::leftRotate(AVLnode* NBnode) {
	NBnode->bf = 0;
	NBnode->right->bf = 0;
	AVLnode* subleft = NBnode->right->left;
	//如果是根节点
	if(NBnode->parent == NULL){	
		root = NBnode->right;			//右子节点成为根节点
		NBnode->right->parent = NULL;
		NBnode->right->left = NBnode;	//成为右子节点的左节点
		NBnode->right = subleft;		//旧的右子节点的左节点成为节点的右子节点
	}else {
		if(NBnode->parent->right != NULL && NBnode->parent->right->getKey() == NBnode->getKey()) {	//判断是父节点的左节点还是右节点
			NBnode->parent->right = NBnode->right;
		}else {
			NBnode->parent->left = NBnode->right;
		}
		NBnode->right->parent = NBnode->parent;	//右子节点为父节点
		NBnode->right->left = NBnode;	//成为右子节点的左节点
		NBnode->right = subleft;		//旧的右子节点的左节点成为节点的右子节点
	}

}

//AVL树右旋
void AVL::rightRotate(AVLnode* NBnode) {
	NBnode->bf = 0;
	NBnode->left->bf = 0;
	AVLnode* subright = NBnode->left->right;
	//如果是根节点
	if(NBnode->parent == NULL){	
		root = NBnode->left;			//左子节点成为根节点
		NBnode->left->parent = NULL;
		NBnode->left->right = NBnode;	//成为左子节点的右节点
		NBnode->left = subright;		//旧的左子节点的右节点成为节点的左子节点
	}else {
		if(NBnode->parent->right != NULL && NBnode->parent->right->getKey() == NBnode->getKey()) {	//判断是父节点的左节点还是右节点
			NBnode->parent->right = NBnode->left;
		}else {
			NBnode->parent->left = NBnode->left;
		}
		//NBnode->parent->left = NBnode->left;
		NBnode->left->parent = NBnode->parent;	//左子节点为父节点
		NBnode->left->right = NBnode;	//成为右子节点的左节点
		NBnode->left = subright;		//旧的左子节点的右节点成为节点的左子节点
	}
	
}


//AVL树左右双旋
void AVL::lrRotate(AVLnode* NBnode) {
	leftRotate(NBnode->left);
	rightRotate(NBnode);
}


//AVL树右左双旋
void AVL::rlRotate(AVLnode* NBnode) {
	rightRotate(NBnode->right);
	leftRotate(NBnode);
	
}


//AVL树删除节点
void AVL::AVLremove(string username) {
	
}

//AVL树插入节点
void AVL::AVLinsert(AVLnode *node) {
	AVLnode* NBnode;
	//设置平衡因子
	node->bf = 0;
	int flag = 1;
	//由x,y的组合决定左右旋
	int x=-1;
	int y=-1;
	//初始状态
	if(root == NULL){
		root = node;
		flag = 0;
	}
	//根节点
	AVLnode* temp = root;
	//开始节点插入
	while(flag){
		if(node->getKey() > temp->getKey()){
			temp->bf--;	//平衡因子-1
			//右失衡节点
			if(temp->bf == -2){
				x=1;
				NBnode = temp;
			}
			if(temp->right == NULL) {
				y=1;
				//连接节点
				temp->right = node;
				node->parent=temp;
				break;
			}else{
				temp = temp->right;
			}
		}else if(node->getKey() < temp->getKey()){
			temp->bf++;	//平衡因子+1
			//左失衡节点
			if(temp->bf == 2){
				x=0;
				NBnode = temp;
			}
			if(temp->left == NULL) {
				y=0;
				//连接节点
				temp->left = node;
				node->parent=temp;
				break;
			}else{
				temp = temp->left;
			}
		}
	}
	//进行平衡操作
	//右旋
	if(x==0 && y==0) {
		rightRotate(NBnode);

	//左旋
	}else if(x==1 && y==1) {
		leftRotate(NBnode);

	//左右双旋
	}else if(x==0 && y==1) {
		lrRotate(NBnode);

	//右左双旋
	}else if(x==1 && y==0) {
		rlRotate(NBnode);
	}

	
	
}

//用户搜索
void AVL::search(string username){
	size_t key = Utils::hash(username);
	string pw = findUser(key);
}

//用户删除
void AVL::remove(string username){
}

//用户添加
void AVL::insert(string username, string password){
	AVLnode *newuser = new AVLnode();
	newuser->setKey(Utils::hash(username));	//将用户名hash后作为key
	newuser->setUsername(username);	//设置用户名
	newuser->setPassword(password);	//设置密码
	AVLinsert(newuser);	//插入到AVL树
	//插入到本文文件
}

//构造函数，根据data.txt构建AVL树
AVL::AVL() {
	//初始化根为空
	root = NULL;
	//记录节点数
	int index=0;
	//读取文件内容缓冲区
	char buff[1024]={0};
	string str;
	vector<string> split_str;
	//文件流
	ifstream f;
	f.open(USER_FILE);
	//读取用户数据并且生成所有节点
	while(!f.eof()){
		f.getline(buff,1024);
		str = buff;
		//字符分割
		split_str = Utils::split(str, ' ');
		//再次判断是否读取文件末尾
		if(0 == split_str.size()){
			break;
		}
		//生成节点并且存储,其中，key为用户名的哈希值
		AVLnode node(Utils::hash(split_str[0]), split_str[0], split_str[1], (AVLnode*)NULL, (AVLnode*)NULL);
		vec.push_back(node);
		
		index++;
	}
	f.close();
	//设置总节点数
	num = index;
	

	//将节点初始化为AVL树
	for(unsigned long  i=0;i<num;i++){
		//在AVL树中插入节点
		AVLinsert(&vec[i]);
	}

	//测试输出所有节点数据
	cout<<num<<endl;
	for(unsigned long i=0;i<num;i++){
		cout<<"用户名：" << vec[i].getUsername()<<endl;
		cout<<"密码：" << vec[i].getPassword()<<endl;
	}
	traversal(root);
	//cout<<root->getKey()<<" "<<root->getUsername()<<endl;	
	//cout<<root->left->getKey()<<" "<<root->left->getUsername()<<endl;	
	//cout<<root->left->left->getKey()<<" "<<root->left->left->getUsername()<<endl;	
	//cout<<root->left->right->getKey()<<" "<<root->left->right->getUsername()<<endl;	
}

//析构函数
AVL::~AVL() {}
