#ifndef Noncopyable_hpp
#define Noncopyable_hpp

namespace Pht {
    class Noncopyable {
    public:
        Noncopyable() = default;
        ~Noncopyable() = default;

    private:
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator=(const Noncopyable&) = delete;
    };
}

#endif
