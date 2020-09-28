#include "argparse.h"

#include <string>
#include <cstring>
#include <iostream>
#include <filesystem>

/**
 * Parses the command line arguments
 * and returns a mesh operation
 */
Operation parse(int argc, char *argv[])
{
    Operation operation;

    // Argument Help
    if (argc == 0)
    {
        std::cout << "Usage: ./subdivide.exe file [--loop | --butterfly | --sqrt3] -n iterations" << std::endl;
        std::cout << "If no iterations are given, default is 1" << std::endl;
        exit(EXIT_SUCCESS);
    }

    // Loop on the array
    for (int i = 0; i < argc; i++)
    {
        // obj mesh file
        if (i == 1)
        {
            operation.meshName.assign(argv[i]);
            if (std::filesystem::exists(argv[i]))
            {
                // std::string msg;
                // msg.append("Found ").append(argv[i]);
                // std::cout << msg << std::endl;
                continue;
            }
            else
            {
                std::string msg;
                msg.append("Given file does not exist: ").append(argv[i]);
                std::cout << msg << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        // subdivision operations
        if (strcmp(argv[i], "--loop") == 0)
            operation.scheme = Loop;
        else if (strcmp(argv[i], "--butterfly") == 0)
            operation.scheme = Butterfly;
        else if (strcmp(argv[i], "--sqrt3") == 0)
            operation.scheme = Sqrt3;

        // iteration count
        if (strcmp(argv[i], "-n") == 0)
        {
            if (i + 1 >= argc)
            {
                std::cout << "A nonzero positive number must follow -n" << std::endl;
                exit(EXIT_FAILURE);
            }
            int n = atoi(argv[i + 1]);
            if (n <= 0)
            {
                std::cout << "Iteration count n cannot be negative or zero" << std::endl;
                exit(EXIT_FAILURE);
            }
            else
                operation.iterations = (unsigned int)n;
        }
    }

    return operation;
}