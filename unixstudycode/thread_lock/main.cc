#include "message.h"

#include <boost/threadpool.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>

typedef std::vector< boost::shared_ptr<Message> > MessageVector;

void usage(char** argv)
{
    printf("Usage: %s [-m message_count] [-c loop_count] [-t thread_count] [-h]\n", argv[0]);
    printf("\t-t thread count\n");
    printf("\t-m message count\n");
    printf("\t-c loop count\n");
    printf("\t-h Show this message\n");
}

void run(Message* message) {
    message->Check();
}

int main(int argc, char* argv[]) {
    int thread_count    = 4;
    int message_count   = 100;
    int loop_count      = 100;
    int c = 0;
    while (-1 != (c = getopt(argc, argv, "t:c:m:h")))
    {
        switch (c)
        {
            case 't':
                thread_count = atoi(optarg);
                break;
            case 'm':
                message_count = atoi(optarg);
                break;
            case 'c':
                loop_count = atoi(optarg);
                break;
            case 'h':
                usage(argv);
                return -1;
            default:
                fprintf(stderr, "Illegal argument \"%c\"\n", c);
                usage(argv);
                return 1;
        }
    }

    printf("\t thread count=%d\n", thread_count);
    printf("\tmessage count=%d\n", message_count);
    printf("\t   loop count=%d\n", loop_count);

    MessageVector messages;
    for (int i = 0; i < message_count; ++i) {
        messages.push_back(boost::shared_ptr<Message>(new Message(i)));
    }

    boost::shared_ptr<boost::threadpool::pool> pool = boost::shared_ptr<boost::threadpool::pool>(new boost::threadpool::pool(thread_count));

    printf("\n");
    printf("test begin, please wait ...\n");

    MessageVector::iterator it (messages.begin());
    MessageVector::iterator ite(messages.end());
    for (; it != ite; ++it) {
        for (int i = 0; i < loop_count; i++) {
            pool->schedule(boost::bind(&run, it->get()));
        }
    }

    pool->wait();

    bool verify_ok = true;
    it = messages.begin();
    for (; it != ite; ++it) {
        if ((*it)->count() != 1) {
            verify_ok = false;
            fprintf(stderr, "id=%d thread-lock error, loop_count=%d message->count=%d\n", (*it)->id(), loop_count, (*it)->count());
        }
    }

    if (verify_ok) {
        printf("verify pass!\n");
    } else {
        printf("verify falied!\n");
    }


    printf("\n");

    return 0;
}

