  #include <iostream>
  #include <boost/progress.hpp>

  int main()
  {
      long int i(1100000000),j(1100000000),k(1);
      boost::progress_display pd(i);
      boost::progress_timer t;
      while(i>0)
      {
          while(j>0)
          {
              j--;
          }
          i--;
          ++pd;   //更新进度显示
      }
  }
