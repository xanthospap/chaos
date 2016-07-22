#include <iostream>

char line[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void
set_ptr_to_n(char** ptr=nullptr, std::size_t n=0) noexcept
{
    if (*ptr) { *ptr = line + n; }
    return;
}

int main()
{
    char *ptr = line;
    std::cout<<"ptr = " << ptr << "\n";
    set_ptr_to_n(&ptr, 5);
    std::cout<<"ptr = " << ptr << "\n";
    ptr = nullptr;
    set_ptr_to_n(&ptr, 5);
    std::cout<<"ptr = " << ptr << "\n";

    return 0;
}
