#include <boost/foreach.hpp> 
#include <boost/program_options.hpp> 

//using namespace boost::program_options; 
//using namespace std; 

int print_vm(const boost::program_options::options_description &opts, const boost::program_options::variables_map &vm) 
{ 
    if (vm.size() == 0) { 
        std::cout << opts << std::endl; 
        return 0; 
    } 

    // 解析完成，实现选项处理逻辑 
    if (vm.count("help")) { 
        std::cout << opts << std::endl; //输出帮助信息 
        return 0; 
    } 

    //输出查找文件名，因为有缺省值，故总存在 
    std::cout << "find opt: " << vm["filename"].as<std::string>() << std::endl; 

    if (vm.count("dir")) { 
        std::cout << "dir opt:"; 
        BOOST_FOREACH(std::string str, vm["dir"].as<std::vector<std::string> >()) { 
            std::cout << str << ","; 
        } 
        std::cout << std::endl; 
    } 

    if (vm.count("depth")) { 
        std::cout << "depth opt: " << vm["depth"].as<int>() << std::endl; 
    } 
} 

int main(int argc, char* argv[]) // 需要命令行参数 
{ 
    std::string filename; 
    std::string port; 
    boost::program_options::options_description mysql_opts("Mysql performance options"); //增加两个程序选项 
    mysql_opts.add_options() 
        ("help,h", "help message") 
        ("filename,f", boost::program_options::value<std::string>(&filename)->default_value("text"), "to find a file") 
        ("dir,D", boost::program_options::value<std::vector<std::string> >()->multitoken(), "search dir") 
        ("depth,d", boost::program_options::value<int>()->implicit_value(5), "search depth"); 

    boost::program_options::options_description http_opts("http options"); //增加两个程序选项 
    http_opts.add_options() 
        ("port,p", boost::program_options::value<int>()->implicit_value(5), "http port"); 


    boost::program_options::options_description cmdline_options;
    cmdline_options.add(mysql_opts);
    cmdline_options.add(http_opts);
    boost::program_options::variables_map vm; // 选项存储map容器 
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, cmdline_options), vm); //解析参数并存储到vm中 
    boost::program_options::notify(vm); 
    print_vm(cmdline_options, vm); 
    return 0;
}
