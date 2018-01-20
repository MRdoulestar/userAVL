c=g++ -g -std=c++0x
b=g++
main:main.cpp AVL.cpp AVLnode.cpp utils.cpp
	$c -o main dog4u.h main.cpp AVL.cpp AVLnode.cpp Init.cpp utils.cpp 
