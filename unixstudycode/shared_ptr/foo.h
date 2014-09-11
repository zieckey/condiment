class Foo 
{ 

    public: 
        ~Foo()
        {
            std::cout  << "pointer:" << this << " " << __PRETTY_FUNCTION__ << " called\n";
        }

        void print() 
        {

            std::cout << "pointer:" << this << " foo::print" << std::endl; 

        } 
}; 


class Bar
{
    public:
        Bar(int _d) : d_(_d) {}

        ~Bar() 
        {
            std::cout <<  "pointer:" << this << " " << __PRETTY_FUNCTION__ << " called, this->d_=" << d_ << std::endl;
        }

        int d() const 
        {
            return d_;
        }
    private:
        int d_;
};
