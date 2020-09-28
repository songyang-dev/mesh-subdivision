#include "argparse.h"

#include <string>
#include <cstring>
#include <iostream>
#include <filesystem>

void onlyOneSchemeWarning();

/**
 * Parses the command line arguments
 * and returns a mesh operation
 */
Operation parse(int argc, char *argv[])
{
    Operation operation;
    operation.iterations = 1;
    operation.output = "output.obj";

    // Argument Help
    if (argc == 1)
    {
        std::cerr << "Usage: ./subdivide.exe file (--loop | --butterfly | --sqrt3) [-n iterations] [-o output]" << std::endl;
        std::cerr << "If no iterations are given, default is 1." << std::endl;
        std::cerr << "Default output is \"output.obj\" the current directory." << std::endl;
        exit(EXIT_FAILURE);
    }

    // subdivision scheme exclusion flag
    // only one scheme can be chosen
    bool hasChosenScheme = false;

    // Loop on the array
    for (int i = 0; i < argc; i++)
    {
        // obj mesh file
        if (i == 1)
        {
            operation.meshName.assign(argv[i]);

            // checks if the file exists
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
                std::cerr << msg << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        // subdivision operations
        if (strcmp(argv[i], "--loop") == 0)
        {
            operation.scheme = Loop;
            if (hasChosenScheme)
                onlyOneSchemeWarning();
            else
                hasChosenScheme = true;
        }
        else if (strcmp(argv[i], "--butterfly") == 0)
        {
            operation.scheme = Butterfly;
            if (hasChosenScheme)
                onlyOneSchemeWarning();
            else
                hasChosenScheme = true;
        }
        else if (strcmp(argv[i], "--sqrt3") == 0)
        {
            operation.scheme = Sqrt3;
            if (hasChosenScheme)
                onlyOneSchemeWarning();
            else
                hasChosenScheme = true;
        }

        // iteration count
        else if (strcmp(argv[i], "-n") == 0)
        {
            if (i + 1 >= argc)
            {
                std::cerr << "A nonzero positive number must follow -n" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::string count;
            int n = 0;
            try
            {
                n = std::stoi(count.assign(argv[i+1]));
            }
            catch(const std::invalid_argument&)
            {
                std::cerr << "Received iteration count is: " << count << std::endl;
            }
            catch(const std::out_of_range&)
            {
                std::cerr << "Received iteration count is overflowing the integer capacity" << std::endl;
            }
            
            if (n <= 0)
            {
                std::cerr << "Iteration count n must be a nonzero positive integer, without overflowing" << std::endl;
                exit(EXIT_FAILURE);
            }
            else
                operation.iterations = (unsigned int)n;
        }
        
        // output file location
        else if (strcmp("-o", argv[i]) == 0)
        {
            if (i + 1 >= argc)
            {
                std::cerr << "No output path specified" << std::endl;
                exit(EXIT_FAILURE);
            }
            operation.output.assign(argv[i+1]);
        }
    }

    // one scheme must be chosen
    if (!hasChosenScheme) {
        std::cerr << "A subdivision scheme must be chosen: (--loop | --butterfly | --sqrt3)" << std::endl;
        exit(EXIT_FAILURE);
    }

    return operation;
}

void onlyOneSchemeWarning()
{
    std::cerr << "Only one subdivision scheme may be chosen" << std::endl;
    exit(EXIT_FAILURE);
}