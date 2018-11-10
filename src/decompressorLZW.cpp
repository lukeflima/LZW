#include "decompressorLZW.h"
#include <bitset>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>

d_Node::d_Node(uint8_t _value, uint _ID, d_Node* _parent, bool _unknown):
	value(_value), ID(_ID), parent(_parent), unknown(_unknown)
	{}

int decompressLZW(std::string file){
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

	std::vector<bool> bits;
	for(int i = 0; i < bytes.size(); i++){
		std::bitset<8> bs(bytes[i]);
		auto s = bs.to_string();
		for(int j = 0; j < 8; j++)
			bits.push_back(s[j] == '1');
	}

	uint numBits = 8;
	std::vector<d_Node*> Dic;
	for(int i = 0; i < 256; i++)
		Dic.push_back(new d_Node (i,i));

	int decId;
	std::vector<char> decoded;
	int increment = numBits;
	d_Node *prev = nullptr;
	bool end = false;
	int sizebits = bits.size();
	for(int i = 0; (i + numBits) - 1 < sizebits; i += increment){
		std::cout << (int) i*100/sizebits << " %\r";
		increment = numBits;
		std::vector<bool> b(bits.begin() + i, bits.begin() + i + numBits);
		decId = 0;
		for(auto a: b)
			decId = (decId << 1) + a;
		auto current = Dic[decId];
		auto code = current;
		std::vector<char> temp;
		while(code){
			if(!code->unknown)
				temp.push_back((uint8_t)code->value);
			code = code->parent;
		}
		std::reverse(temp.begin(),temp.end());
		if(end){
			if(prev->unknown){
				prev->value = temp[0];
				prev->unknown = false;
			}
			end = false;
		}
		if(current->unknown){
			temp.push_back(temp[0]);
			current->value = temp[0];
			current->unknown = false;
		}
		for(auto t: temp){
			decoded.push_back(t);
		}

		if(Dic.size() < MAX_SIZE_DIC){
			if(prev){
				if(prev->unknown){
					prev->value = temp[0];
					prev->unknown = false;
				}
			}
			prev = new d_Node(0,Dic.size(), current, true);
			Dic.push_back(prev);
			if(Dic.size() > std::pow(2,numBits)) numBits++;
			if(!(Dic.size() < MAX_SIZE_DIC))
				end = true;
		}

	}
	auto fileOut = file;
	fileOut.erase(file.size() - 4,file.size() - 1);
	std::ofstream f(fileOut, std::ios::binary);
	for(int i = 0; i < decoded.size(); i++){
		//std::cout << decoded[i];
		f << decoded[i];
	}
	//for(auto d: decoded) std::cout << (uint8_t)d << " ";
	//std::cout ENDL;

	LOG(file + " was decompressed as " + fileOut) ENDL;
	return 0;
}