#pragma once
#include <iostream>

template <typename T>
void _print_iterable(T iterable)
{
    std::cout << "Iterable " << "{\n";
    for (const auto &data : iterable)
    {
        std::cout << "\t" << data << ",\n";
    }
    std::cout << "}" << std::endl;
};
