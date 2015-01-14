
#include "client.h"


void usage(char** argv)
{
    printf("Usage: %s [-c 50000] [-h]\n", argv[0]);
    printf("\t-h Show this message\n");
    printf("\t-c Loop count, how many messages will be sent to server\n");
    printf("\t-t Thread count\n");
    printf("\t-i server ip\n");
    printf("\t-p server port\n");
    printf("\t-m The message size, default 512\n");
    printf("\t-s Session count, how many connections, default 1\n");
    printf("\t-k send message interval for every session connection, in second, default 1.0\n");
}

    

int main(int argc, char* argv[])
{
    int loopCount = 1;
    int threadCount = 1;
    const char* ip = "127.0.0.1";
    uint16_t port = 80;
    int messageSize = 512;
    int sessionCount = 1;
    int timeout = 1;
    double sendMsgInterval = 1.0;

    int c = 0;
    while (-1 != (c = getopt(argc, argv, "c:t:i:p:m:s:k:h")))
    {
        switch (c)
        {
            case 'c':
                loopCount = atoi(optarg);
                break;
            case 't':
                threadCount = atoi(optarg);
                break;
            case 'i':
                ip = optarg;
                break;
            case 'p':
                port = static_cast<uint16_t>(atoi(optarg));
                break;
            case 'm':
                messageSize = atoi(optarg);
                break;
            case 's':
                sessionCount = atoi(optarg);
                break;
            case 'k':
                sendMsgInterval = atof(optarg);
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

    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid() << "\n\t"
             << "loopCount=" << loopCount << "\n\t"
             << "threadCount=" << threadCount << "\n\t"
             << "serverIp=" << ip << "\n\t"
             << "serverPort=" << port << "\n\t"
             << "messageSize=" << messageSize << "\n\t"
             << "sessionCount=" << sessionCount << "\n\t"
             << "timeout=" << timeout << "\n\t"
             << "sendMsgInterval=" << sendMsgInterval << "\n\t"
             ;
    //Logger::setLogLevel(Logger::WARN);

    EventLoop loop;
    InetAddress serverAddr(ip, port);

    Client client(&loop, serverAddr, messageSize, sessionCount, timeout, threadCount, loopCount, sendMsgInterval);
    loop.loop();


    return 0;
}

