#include <stdio.h>
#include <string.h>


// g++ compile_time_calculation.cc -S -std=c++0x -O3

  // compile time calculation of basename of source file
    const char * base_name(const char *arr)
    {
      const char* slash = strrchr(arr, '/'); // builtin function
      if (slash)
      {
        return slash + 1;
      }
      return NULL;
    }

  // compile time calculation of basename of source file
  class SourceFile
  {
   public:
    template<int N>
    inline SourceFile(const char (&arr)[N])
      : data_(arr),
        size_(N-1)
    {
      const char* slash = strrchr(data_, '/'); // builtin function
      if (slash)
      {
        data_ = slash + 1;
        size_ -= static_cast<int>(data_ - arr);
      }
    }

    explicit SourceFile(const char* filename)
      : data_(filename)
    {
      const char* slash = strrchr(filename, '/');
      if (slash)
      {
        data_ = slash + 1;
      }
      size_ = static_cast<int>(strlen(data_));
    }

    const char* data_;
    int size_;
  };


int main()
{
    const char* f = base_name(__FILE__);
    printf("%s:xxxxxxxxxxxxxxxxxxxxxx\n", f);
    SourceFile g(__FILE__);
    printf("%s:%lu:YYYYYYYYYYYYYYYYYYYYYYYY\n", g.data_, g.size_);
    return 0;
}
