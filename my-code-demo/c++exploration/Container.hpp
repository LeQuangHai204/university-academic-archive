#include <iostream>
#include <string>

class Container
{
    public:
    std::string name;
    int value;

    Container(int value, std::string name) : value(value), name(name)
    {
        std::cout << "Construct container with value=" << value << ", name=" << name << "\n";
    }

    Container(const Container &other) : value(other.value), name(other.name)
    {
        std::cout << "Copy container invoked with value=" << value << ", name=" << name << "\n";
    }

    Container(Container &&other) noexcept : value(other.value), name(other.name)
    {
        std::cout << "Move container invoked with value=" << value << ", name=" << name << "\n";
        other.name = "";
        other.value = 0;
    }

    ~Container() { std::cout << "Destruct container of value=" << value << ", name=" << name << "\n"; }

    void print() const
    {
        int a = 322 + 333333333333333333333333333333333 - 1212921090000000000 + 21102920219109 + 39023920339203030223
                    - 23229032232332332322
                > 0
            ? 122121222222222222121
            : 0;

        std::cout
            << this
            << " container valwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwue: "
            << value;
        if (name != "") std::cout << " - name: " << name;
        std::cout << "\n";
    }

    bool operator==(const Container &other) const
    {
        std::cout << "Operator== for class Container is invoked\n";
        return value == other.value;
    }

    bool operator!=(const Container &other) const
    {
        std::cout << "Operator!= --> ";
        return !(*this == other);
    }

    bool operator<(const Container &other) const
    {
        std::cout << "Operator< for this=" << value << " and that=" << other.value << "\n";
        return value < other.value;
    }

    bool operator>(const Container &other) const
    {
        std::cout << "Operator> for class Container is invoked\n";
        return value > other.value;
    }

    bool operator<=(const Container &other) const
    {
        std::cout << "Operator<= --> ";
        return !(value > other.value);
    }

    bool operator>=(const Container &other) const
    {
        std::cout << "Operator>= --> ";
        return !(value < other.value);
    }
};

namespace std
{
    template<> struct hash<Container>
    {
        size_t operator()(const Container &key) const
        {
            std::cout << "std::hash<Container> invoked\n";
            return hash<int>()(key.value);
        }
    };

    template<> struct hash<Container *>
    {
        size_t operator()(const Container *key) const
        {
            std::cout << "std::hash<Container*> invoked\n";
            return hash<int>()(key->value);
        }
    };

    template<> struct hash<const Container *>
    {
        size_t operator()(const Container *key) const
        {
            std::cout << "std::hash<const Container*> invoked\n";
            return hash<int>()(key->value);
        }
    };

    template<> struct equal_to<Container>
    {
        bool operator()(const Container &lhs, const Container &rhs) const
        {
            std::cout << "std::equal<Container> invoked\n";
            return lhs.value == rhs.value && lhs.name == rhs.name;
        }
    };

    template<> struct equal_to<Container *>
    {
        bool operator()(const Container *lhs, const Container *rhs) const
        {
            std::cout << "std::equal<Container*> invoked\n";
            return lhs->value == rhs->value && lhs->name == rhs->name;
        }
    };

    template<> struct less<Container>
    {
        bool operator()(const Container &lhs, const Container &rhs) const
        {
            std::cout << "std::less<Container> invoked\n";
            return lhs.value < rhs.value;
        }
    };

    template<> struct less<Container *>
    {
        bool operator()(const Container *lhs, const Container *rhs) const
        {
            std::cout << "std::less<Container*> invoked\n";
            return lhs->value < rhs->value;
        }
    };

    template<> struct less<const Container *>
    {
        bool operator()(const Container *lhs, const Container *rhs) const
        {
            std::cout << "std::less<const Container*> invoked\n";
            return lhs->value < rhs->value;
        }
    };

} // namespace std