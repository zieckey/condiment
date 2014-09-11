#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <boost/thread.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>

class Message {
public:
    Message(int id) {
        id_ = id;
        has_called_ = false;
        count_ = 0;
    }

    void Check() {
        boost::mutex::scoped_lock lock(mutex_);
        if (has_called_) {
            return;
        }

        has_called_ = true;

        __sync_fetch_and_add(&count_, 1); 

        //occupy some cpu time
        using namespace boost::archive::iterators;
        typedef base64_from_binary< transform_width< const char *, 6, 8 > > base64_text;
        const char* address = "zlkxcnvzkx nsdfhlaweir q[230q'as dpjf;ae[0q9u[2309h21    ;2lkjasd;faj;sd fo[p398q123u09rj;fasdifh afj";
        size_t      count   = strlen(address);
        for (int i = 0; i < 10; ++i)
        {
            std::stringstream ss;
            std::copy( base64_text(address),
                        base64_text(address + count),
                        std::ostream_iterator<char>(ss) );
            std::string b64 = ss.str();
            std::string eb64 = "emxreGNudnpreCBuc2RmaGxhd2VpciBxWzIzMHEnYXMgZHBqZjthZVswcTl1WzIzMDloMjEgICAgOzJsa2phc2Q7ZmFqO3NkIGZvW3AzOThxMTIzdTA5cmo7ZmFzZGlmaCBhZmo";
            if (b64 != eb64) {
                fprintf(stderr, "boost base64 encode error, [%s]\n", b64.c_str());
            }
        }
    }

    int count() const { return count_; }
    int id()    const { return id_;    }

private:
    int          count_;          
    bool         has_called_;
    boost::mutex mutex_;
    int          id_;
};

#endif

