// #include <examples/include/Metal.hpp>
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Metal.hpp>
#include <iostream>

int main()
{
    MTL::Device *pDevice = MTL::CreateSystemDefaultDevice();

    std::cout << "Metal include works: " << pDevice->name() << std::endl;

    pDevice->release();
    return 0;
}
