#include <iostream>
#include <vector>
#include <string>

#ifndef __COMP_LZW__
#define __COMP_LZW__

#define LOG(x) std::cout << x << " "
#define LOGNAME(x) std::cout << #x " = " << x << " "

#define MAX_SIZE_DIC 65535
#define BUFFSIZE 102400


class Node{
public:
	std::vector<Node*> m_children;
	int ID;
	uint8_t value;
	Node(uint8_t _value, uint _ID);
	bool isChild(uint8_t v);
	Node* getChild(uint8_t v);
	void addChild(uint8_t v, uint id);
};

void toBinStr(int value, int bitsCount, std::string& output);

int compressLZW(std::string file);
#endif //__COMP_LZW__
