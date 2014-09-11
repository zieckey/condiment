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
    
    std::ofstream file(path, std::ios_base::out | std::ios_base::binary);
    if (!file)
    {
        std::cerr << "open file failed:" << path << '\n';
    }

    try {
        boost::iostreams::filtering_ostream out;
        out.set_auto_close(true);
        out.push(boost::iostreams::gzip_compressor());
        out.push(file);
        for(int i = 0; i < 10; ++i )
        {
            out << "aaaaaaaaaaaaaaaaaaaaaaa\n";
            out << "bbbbbbbbbbbbbbbbbbbbbbb\n";
        }
    }
    catch(const boost::iostreams::gzip_error& e) {
         std::cout << e.what() << '\n';
    }

    file.flush();
    file.close();

    return 0;
}

