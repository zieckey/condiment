#include <memory.h>

#include <iostream>

#ifndef DISALLOW_COPY_AND_ASSIGN
    //! Micro of disable the copy and assignment constructor.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);               \
    TypeName& operator=(const TypeName&)
#endif


class CExam
{
public:
    CExam() : 
        max_(0), 
        n_(0), 
        elements_(NULL)
    {
    }

    CExam(int nmax, double* pItem) :
        max_(nmax), 
        n_(0),
        elements_(pItem)
    {
    }

    ~CExam();

    double GetAverage();
    bool   Init(const size_t count, double init_value = 0.0); 
    bool   AddElement(double value);

private:
    int     max_;
    int     n_;
    double* elements_; 

private:
    DISALLOW_COPY_AND_ASSIGN(CExam);
};

CExam::~CExam()
{
    n_ = 0;
    max_ = 0;
    if(elements_)
    {
        delete [] elements_;
        elements_ = NULL;
    }
}

bool CExam::Init(const size_t count, double init_value /*= 0.0*/)
{
    double* temp_elements = new double[count];
    if (NULL == temp_elements)
    {
        return false;
    }

    for (size_t i = 0; i < count; ++i)
    {
        temp_elements[i] = init_value;
    }

    if(elements_)
    {
        delete[] elements_;
        elements_ = NULL;
    }

    elements_ = temp_elements;
    n_        = 0;
    max_      = static_cast<int>(count);
    return true;
}

bool CExam::AddElement(double value)
{
    if(n_ < max_)
    {
        elements_[n_++] = value;
        return true;
    }

    return false;
}

double CExam::GetAverage()
{
    if(n_ <= 0)
    {
        return 0.0;
    }

    long double total = 0.0;
    for(int i=0; i<n_; i++)
    {
        total += static_cast<long double>(elements_[i]);
    }

    return total / n_;
}

int main()
{
    CExam exam;
    const int total = 10;
    exam.Init(total);
    exam.AddElement(1.1);
    exam.AddElement(3.1);
    std::cout << "average : " << exam.GetAverage() << std::endl;
    return 0;
}

