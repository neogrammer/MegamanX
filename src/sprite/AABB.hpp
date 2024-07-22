#ifndef AABB_HPP__
#define AABB_HPP__
#include <cstddef>
#include <concepts>
#include <functional>
template <typename T>
concept ConvertibleToFloat = std::convertible_to<T, float>;
template <typename T>
concept ConvertibleToUInt32 = std::convertible_to<T, uint32_t>;
struct AABB
{
	AABB() = default;
	AABB(ConvertibleToFloat auto l_, ConvertibleToFloat auto t_
		,ConvertibleToUInt32 auto w_, ConvertibleToUInt32 auto h_);
	void setTop(ConvertibleToFloat auto t_);  
	void setLeft(ConvertibleToFloat auto l_); 
	void setRight(ConvertibleToFloat auto r_);
	void setBottom(ConvertibleToFloat auto b_);
public:
	float tp{0.f}, oTp{0.f};
	float lft{0.f}, oLft{0.f};
	float rt{0.f}, oRt{0.f};
	float bttm{0.f}, oBttm{0.f};
	uint32_t w{0Ui32}, h{0Ui32};
	float vx{0.f};
	float vy{0.f};
};
#include "tpl/AABB.tpl"
#endif
