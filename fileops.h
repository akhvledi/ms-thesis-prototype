
#include <fstream>
#include <streambuf>
#include <string>
#include <cerrno>



class fileops {



	//const char* filename = "source_files/index.html";

public:

	fileops()
	{

	}


	static std::string get_file_string(const char* filename) {
		std::ifstream inFile(filename);
		if(inFile) {
			std::string out = (std::string((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>()) );
			inFile.close();
			return out;
		}
		else {
			std::cerr<< "Unable to open file: " <<std::endl;
		}
		

	}


};

