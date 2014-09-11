
#include <iostream> 
#include <tr1/memory> 
#include <vector> 

/*shared_ptr can work with class hierarchies, so that shared<D> is convertible to shared<B>, where D is a class (or struct) derived 
 * from B. The following class hierarchy is used to demonstrate the concept. 
 */ 
class Item 
{

    std::string title_; 
    public: 
    Item(const std::string& title) : 
        title_(title) 
        { 
        } 
    virtual ~Item() 
    { 
    } 


    virtual std::string Description() const = 0; 
    std::string Title() const 
    { 

        return title_; 

    } 
}; 

class Book: public Item 
{ 

    int pages_; 
    public: 
    Book(const std::string& title, int pages) : 
        Item(title), pages_(pages) 
        { 
        } 


    virtual std::string Description() const 
    { 

        return "Book: " + Title(); 

    } 
    int Pages() const 
    { 
        return pages_; 

    } 
}; 

class DVD: public Item 
{ 

    int tracks_; 
    public: 
    DVD(const std::string& title, int tracks) :

        Item(title), tracks_(tracks) 
        { 
        } 


    virtual std::string Description() const 

    { 

        return "DVD: " + Title(); 

    } 
    int Tracks() const 
    { 


        return tracks_; 

    } 
}; 

/* 
 * output: 

 * Book: Effective STL 
 * DVD: Left of the Middle 
 */ 
int main() 
{

    std::vector<std::tr1::shared_ptr<Item> > items; 
    items.push_back(std::tr1::shared_ptr<Book>(new Book("Effective STL", 400))); 
    items.push_back(std::tr1::shared_ptr<DVD>(new DVD("Left of the Middle", 14))); 

    for (std::vector<std::tr1::shared_ptr<Item> >::const_iterator it = 
                items.begin(); it != items.end(); ++it) 
      std::cout << (*it)->Description() << std::endl; 


    return 0; 

} 

