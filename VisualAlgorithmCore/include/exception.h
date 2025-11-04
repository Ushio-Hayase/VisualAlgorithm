//
// Created by UshioHayase on 2025-10-22.
//

#ifndef VISUALALGORITHMCORE_EXCEPTION_H
#define VISUALALGORITHMCORE_EXCEPTION_H
#include <stdexcept>

class FileError final : public std::runtime_error
{
   public:
    explicit FileError(const std::string& msg) : std::runtime_error(msg)
    {
    }
};

class NullPointerError final : public std::runtime_error
{
   public:
    explicit NullPointerError()
        : std::runtime_error("NullPointerReferenceError")
    {
    }
};

#endif  // VISUALALGORITHMCORE_EXCEPTION_H
