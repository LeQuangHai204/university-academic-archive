#include <iostream>
#include <iterator>
#include <set>
#include <vector>

// Example Container class
class Container
{
    public:
    int value;

    Container(int val) : value(val) { }
};

// Custom comparison functor
struct CustomCompare
{
    bool operator()(const Container *lhs, const Container *rhs) const
    {
        // Sort containers by their values in ascending order
        return lhs->value < rhs->value;
    }
};

// Custom allocator
template<typename T> struct CustomAllocator
{
    using value_type = T;

    CustomAllocator() = default;

    template<typename U> CustomAllocator(const CustomAllocator<U> &) noexcept { }

    T *allocate(std::size_t n)
    {
        std::cout << "CustomAllocator::allocate called for " << n << " elements\n";
        if (n > std::size_t(-1) / sizeof(T)) throw std::bad_alloc();
        if (auto p = static_cast<T *>(std::malloc(n * sizeof(T)))) return p;
        throw std::bad_alloc();
    }

    void deallocate(T *p, std::size_t n) noexcept
    {
        std::cout << "CustomAllocator::deallocate called for " << n << " elements\n";
        std::free(p);
    }
};

int main()
{
    std::set<Container *, std::less<Container *>, CustomAllocator<Container *>> setWithCustomAlloc;
    std::cout << "-----------\n";
    setWithCustomAlloc.insert(new Container(9));
    std::cout << "-----------\n";
    setWithCustomAlloc.insert(new Container(4));
    std::cout << "-----------\n";
    setWithCustomAlloc.insert(new Container(6));
    std::cout << "-----------\n";
    setWithCustomAlloc.insert(new Container(7));
    std::cout << "-----------\n";
    setWithCustomAlloc.insert(new Container(1));
    std::cout << "-----------\n";
    setWithCustomAlloc.insert(new Container(32));
    return 0;

    // Using overridden std::vector with custom allocator
    std::vector<Container *, CustomAllocator<Container *>> vecWithCustomAlloc;
    std::cout << "-----------\n";
    std::vector<Container *, CustomAllocator<Container *>> anotherVec;
    std::cout << "-----------\n";
    std::vector<Container *, CustomAllocator<Container *>> container;
    std::cout << "-----------\n";

    container.push_back(new Container(10));
    std::cout << "\n";
    container.push_back(new Container(5));
    std::cout << "\n";
    container.push_back(new Container(20));
    std::cout << "\n";
    container.push_back(new Container(15));
    std::cout << "\n";
    container.push_back(new Container(10));
    std::cout << "\n";
    container.push_back(new Container(5));
    std::cout << "\n";
    container.push_back(new Container(20));
    std::cout << "\n";
    container.push_back(new Container(15));
    std::cout << "\n";
    container.push_back(new Container(10));
    std::cout << "\n";
    container.push_back(new Container(5));
    std::cout << "\n";
    container.push_back(new Container(20));
    std::cout << "\n";
    container.push_back(new Container(15));
    std::cout << "\n";
    container.push_back(new Container(10));
    std::cout << "\n";
    container.push_back(new Container(5));
    std::cout << "\n";
    container.push_back(new Container(20));
    std::cout << "\n";
    container.push_back(new Container(15));
    std::cout << "-----------\n";

    // Reserve space in vecWithCustomAlloc for 4 elements
    vecWithCustomAlloc.reserve(4);

    // Use std::copy to push_back elements into vecWithCustomAlloc
    std::copy(container.begin(), container.end(), std::back_inserter(vecWithCustomAlloc));

    // Iterate and print the elements
    for (const auto &elem : vecWithCustomAlloc)
    {
        std::cout << elem->value << " ";
    }
    std::cout << std::endl;

    // Don't forget to delete the dynamically allocated objects
    for (auto ptr : vecWithCustomAlloc)
    {
        delete ptr;
    }

    return 0;
}
