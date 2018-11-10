#include "compressorLZW.h"
#include "decompressorLZW.h"

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)> FILE\n"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-d,--descompress \tSpecify to decompress .lzw file\n";
}

int main(int argc, const char *argv[])
{
	if (argc < 2 || argc > 3) {
        show_usage(argv[0]);
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        } else if ((arg == "-d") || (arg == "--descompress")) {
			if(i+1 == argc) decompressLZW(std::string(argv[1]));
			else 			decompressLZW(std::string(argv[2]));
			return 0;
		}
    }
	compressLZW(std::string(argv[1]));
	return 0;
}