#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <iostream>

using namespace boost::interprocess;

//类型和allocator的定义，使用共享内存时需要使用boost::interprocess中
//重新实现的容器而不能使用标准容器
typedef managed_shared_memory::segment_manager                       segment_manager_t;
typedef allocator<void, segment_manager_t>                           void_allocator;
typedef allocator<int, segment_manager_t>                            int_allocator;
typedef vector<int, int_allocator>                                   int_vector;
typedef allocator<int_vector, segment_manager_t>                     int_vector_allocator;
typedef vector<int_vector, int_vector_allocator>                     int_vector_vector;
typedef allocator<char, segment_manager_t>                           char_allocator;
typedef basic_string<char, std::char_traits<char>, char_allocator>   char_string;

struct complex_data
{
    int               id_;
    char_string       char_string_;
    int_vector_vector int_vector_vector_;

    public:
    //因为void_allocator能够转换为任何类型的allocator<T>, 所以我们能够简单的在构造函数中
    //使用void_allocator来初始化所有的内部容器
    complex_data(int id, const char *name, const void_allocator &void_alloc)
        : id_(id), char_string_(name, void_alloc), int_vector_vector_(void_alloc)
    {}
    ~complex_data(){}
};

//将map的key定义为一个string，而把map的value定义为一个complex_data对象
typedef std::pair<const char_string, complex_data>                      map_value_type;
typedef allocator<map_value_type, segment_manager_t>                    map_value_type_allocator;
typedef map< char_string, complex_data
, std::less<char_string>, map_value_type_allocator>          complex_map_type;


static const char* my_shared_memory = "zieckey_shared_memory";
static const char* my_map_name = "zieckey_map";

int main (int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage : [%s] [write|read|delete]\n", argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "write") == 0) {

        //在程序开始和结束的时候移除同名的共享内存
        //如果只是读其他程序创建的共享内存块则不该包含remover
        struct shm_remove
        {
            shm_remove() { shared_memory_object::remove(my_shared_memory); }
            //~shm_remove(){ shared_memory_object::remove(my_shared_memory); }
        } remover;

        //创建共享内存，根据测试，在32位的linux上，单块命名的共享内存大小
        //最大为2^31-1，如果是读其他程序创建的共享内存，则此句应写为
        //managed_shared_memory segment(open_only, my_shared_memory);
        managed_shared_memory segment(create_only,my_shared_memory, 65536*128);

        //一个能够转换为任何allocator<T, segment_manager_t>类型的allocator 
        void_allocator alloc_inst (segment.get_segment_manager());

        //在共享内存上创建map
        //如果map已由其他程序创建，或者不确定map是否已创建，应如下：
        //complex_map_type *mymap = segment.find_or_construct<complex_map_type>
        complex_map_type *mymap = segment.construct<complex_map_type>(my_map_name)(std::less<char_string>(), alloc_inst);

        for(int i = 0; i < 100; ++i){
            //key(string) 和value(complex_data) 都需要在他们的构造函数中
            //包含一个allocator
            char tmp[16] = "";
            sprintf(tmp, "key_%d", i);
            char_string  key(tmp, alloc_inst);
            sprintf(tmp, "value_%d", i);
            complex_data mapped_object(i, tmp, alloc_inst);
            map_value_type value(key, mapped_object);
            //向map中插值
            mymap->insert(value);
            std::cout << "insert value: " << "key=[" << key.data() << "]"
                << "\n\tvalue.id=[" << mapped_object.id_ << "]"
                << "\n\tvalue.char_string_=[" << mapped_object.char_string_.data() << "]\n";
        }

    }
    else if (strcmp(argv[1], "read") == 0) {

        managed_shared_memory segment(open_only, my_shared_memory);

        //一个能够转换为任何allocator<T, segment_manager_t>类型的allocator 
        void_allocator alloc_inst (segment.get_segment_manager());

        
        std::pair<complex_map_type*, std::size_t> res = segment.find<complex_map_type>(my_map_name);

        std::cout << "res.second=" << res.second << " res.first=" << res.first << "]\n";

        if (!res.first) {
            std::cerr << "shared map error\n";
            return -1;;
        }

        complex_map_type* mymap = res.first;

        for(int i = 0; i < 100; ++i){
            std::cout << "i=" << i << "\t";
            char tmp[16] = "";
            sprintf(tmp, "key_%d", i);
            char_string  key(tmp, alloc_inst);
            std::cout << "key=[" << key.data() << "]\t";
            complex_map_type::const_iterator it = mymap->find(key);
            if (it != mymap->end()) {
                std::cout << " value.id=[" << it->second.id_ << "]\t";
                std::cout << " value.char_string=[" << it->second.char_string_.data() << "]\t";
                std::cout << std::endl;
            }
        }

    }
    else if (strcmp(argv[1], "delete") == 0) {

        //在程序开始和结束的时候移除同名的共享内存
        //如果只是读其他程序创建的共享内存块则不该包含remover
        struct shm_remove
        {
            shm_remove() { shared_memory_object::remove(my_shared_memory); }
            ~shm_remove(){ shared_memory_object::remove(my_shared_memory); }
        } remover;

        std::cout << "delete shared memory : " << my_shared_memory << "\n";
    }

    return 0;
}

