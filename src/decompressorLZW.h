#include <string>
#include <iostream>
#ifndef DECOMP_LZW
#define DECOMP_LZW

#define LOG(x) std::cout << x << " "
#define LOGNAME(x) std::cout << #x " = " << x << " "
#define ENDL << '\n'

#define MAX_SIZE_DIC 65535
#define BUFFSIZE 102400

class d_Node{
public:
	int ID;
	uint8_t value;
	bool unknown;
	d_Node* parent;
	d_Node(uint8_t _value, uint _ID, d_Node* _parent = nullptr, bool _unknown = false);
};

int decompressLZW(std::string file);

#endif //DECOMP_LZW
