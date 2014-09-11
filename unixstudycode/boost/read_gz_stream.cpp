#include <iostream>
#include <fstream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

int main(int argc, char* argv[])
{
    const char* path = NULL;
    if (argc != 2)
    {
        fprintf(stderr, "Usage : %s gz_file_path\n", argv[0]);
        return -1;
    }
    path = argv[1];
    
    std::ifstream file(path, std::ios_base::in | std::ios_base::binary);
    if (!file)
    {
        std::cerr << "open file failed:" << path << '\n';
    }

    try {
        boost::iostreams::filtering_istream in;
        in.push(boost::iostreams::gzip_decompressor());
        in.push(file);
        for(std::string str; std::getline(in, str); )
        {
            std::cout << "Processed line:\"" << str << "\"\n";
        }
    }
    catch(const boost::iostreams::gzip_error& e) {
         std::cout << e.what() << '\n';
    }

    return 0;
}

