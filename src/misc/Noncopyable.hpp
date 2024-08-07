#ifndef NONCOPYABLE_HPP__
#define NONCOPYABLE_HPP__

class Noncopyable {
public:
	Noncopyable() = default;
	~Noncopyable() = default;

private:
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator=(const Noncopyable&) = delete;
};

#endif