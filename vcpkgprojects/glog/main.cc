#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>

struct OnApp {
    ~OnApp() {
#ifdef _WIN32
        system("pause");
#endif
    }
} __s_onexit_pause;

int main() {
    LOG(INFO) << "INFO";
    LOG(WARNING) << "WARN";
    LOG(ERROR) << "ERROR";
    return 0;
}