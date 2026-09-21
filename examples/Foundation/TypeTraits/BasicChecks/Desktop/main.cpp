#include <iostream>

#include "Shared.h"

int main() {
    const FoundationExamples::TypeTraits::BasicChecks::Result result =
        FoundationExamples::TypeTraits::BasicChecks::Run();

    std::cout << "Foundation TypeTraits BasicChecks example\n";
    std::cout << "Buffer[0]: " << result.BufferFirst << '\n';
    std::cout << "Buffer size: " << result.BufferSize << '\n';
    std::cout << "Sample channel: " << result.Sample.Channel << '\n';
    std::cout << "Sample value: " << result.Sample.Value << '\n';

    return result.Score == 16 ? 0 : 1;
}
