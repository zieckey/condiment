#include <muduo/base/Logging.h>
#include <muduo/net/inspect/Inspector.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

using namespace muduo;
using namespace muduo::net;

string status(HttpRequest::Method method, const Inspector::ArgList& args)
{
    Inspector::ArgList::const_iterator it (args.begin());
    Inspector::ArgList::const_iterator ite(args.end());
    for (int i = 0; it != ite; ++it, ++i) {
        LOG_INFO << "i=" << i << " args=" << *it;
    }
    return "OK";
}

int main()
{
    EventLoop loop;
    EventLoopThread t;
    Inspector ins(t.startLoop(), InetAddress(12345), "test");
    ins.add("testmodule", "status", &status, "/testmodule/status/a=1/b=2/c=3/d=4");
    loop.loop();
}

