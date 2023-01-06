#ifndef LOG_SYSTEM
#define LOG_SYSTEM
#include "AccSystem.h"

struct opt_ {
    double inc, outc;
    opt_();
    opt_(double p, double q);
};

class FinLog {
    private:
    int n = -1;
    std::fstream _file;
    const std::string _filename = "finlog";

    public:
    FinLog();
    ~FinLog();
    void Show();
    void Show(int num);
    void Add(double in, double out);
};

#endif // LOG_SYSTEM
