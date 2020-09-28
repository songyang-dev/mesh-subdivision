#if !defined(__argparse_h__)
#define __argparse_h__

#include <string>

enum Scheme { Loop, Butterfly, Sqrt3 };

struct Operation {
    Scheme scheme;
    unsigned int iterations;
    std::string meshName;
};

Operation parse(int argc, char *argv[]);

#endif // MACRO
