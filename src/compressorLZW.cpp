#include <fstream>
#include <cmath>
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include "compressorLZW.h"


Node::Node(uint8_t _value, uint _ID): m_children(0){
	ID = _ID;
	value = _value;
}
bool Node::isChild(uint8_t v){
	for (const auto child: m_children){
		if(child->value == v) return true;
	}
	return false;
}
Node* Node::getChild(uint8_t v){
	for (Node* child: m_children){
		if(child->value == v) return child;
	}
}
void Node::addChild(uint8_t v, uint id){
	m_children.push_back(new Node(v,id));
}


void toBinStr(int value, int bitsCount, std::string& output)
{
	std::string a = "";
	for (int i = bitsCount - 1; i >= 0; --i, value >>= 1)
	{
		a += (value & 1) + '0';
	}
	for(int i = a.size() - 1; i >= 0; i-- ){
		output += a[i];
	}
}

int compressLZW(std::string file){
	//opening file
	std::ifstream ifs(file, std::ios::binary|std::ios::ate);
	if(!ifs.is_open()){
		std::cout << "File not found\n";
		return -1;
	}
	auto pos = ifs.tellg();

	std::vector<char> bytes(pos);

	ifs.seekg(0, ifs.beg);
	ifs.read(&bytes[0], pos);
	ifs.close();

	//Creating initial dictionary
	size_t numBits = 8;
	size_t sizeDic = 0;
	std::vector<Node*> Dic;
	for(int i = 0; i < 256; i++)
		Dic.push_back(new Node (i,sizeDic++));

	//Running algorithm
	std::ofstream out(file + ".lzw", std::ios::binary);
	std::string binBuffer;
	int bs = bytes.size();
	for(int i = 0; i < bs; i++){
		std::cout << (int) i*100/bs << " %\r";
		auto root = Dic[(uint8_t)bytes[i]]; //getting context
		while((root->isChild(bytes[i+1])) && (i < (bs - 1)) ) {
			root = root->getChild(bytes[++i]);
		}
		toBinStr(root->ID, numBits, binBuffer);
		//dumping buffer
		if(binBuffer.size() >= BUFFSIZE){
			for(int i = 0; i < BUFFSIZE; i+=8){
				std::bitset<8> bits(binBuffer.substr(i,8));
				char binary_value = (char) bits.to_ulong();
				out.put(binary_value);
			}

			binBuffer.erase(0,BUFFSIZE);
		}
		//checking dictionary capacity
		if(sizeDic < MAX_SIZE_DIC){
			root->addChild(bytes[i+1], sizeDic++);
			if(sizeDic > std::pow(2,numBits)) numBits++;
		}
	}
	//dumping rest of buffer
	std::string zero("00000000");
	for(int i = 0; i < binBuffer.size(); i+=8){
		if((( (int)binBuffer.size()/8)*8) == i){
			std::bitset<8> bits(binBuffer.substr(i, binBuffer.size()-i) + zero.substr(0, 8 - (binBuffer.size()-i) ));
			char binary_value = (char) bits.to_ulong();
			out.put(binary_value);
			break;
		}
		std::bitset<8> bits(binBuffer.substr(i,8));
		char binary_value = (char) bits.to_ulong();
		out.put(binary_value);
	}
	std::cout << file << " compressed.\n";
	return 0;
}


