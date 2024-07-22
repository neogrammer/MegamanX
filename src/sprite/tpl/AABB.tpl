AABB::AABB(ConvertibleToFloat auto l_, ConvertibleToFloat auto t_
		 , ConvertibleToUInt32 auto w_, ConvertibleToUInt32 auto h_)
	: lft{ static_cast<float>(l_) } 
	, tp{ static_cast<float>(t_) } 
	, w{ static_cast<std::uint32_t>(w_) }
	, h{ static_cast<std::uint32_t>(h_) }
{
	this->oLft = static_cast<float>(static_cast<std::float_t>(l_));
	this->oTp = static_cast<float>(t_);
	this->oRt = this->rt = static_cast<float>(l_) + static_cast<float>(static_cast<std::uint32_t>(w_));
	this->oBttm = this->bttm = static_cast<float>(t_) + static_cast<float>(static_cast<std::uint32_t>(h_));
	this->vx = this->vy = 0.f;
}
void AABB::setTop(ConvertibleToFloat auto t_) { this->tp = t_; this->bttm = t_ + this->h; }
void AABB::setLeft(ConvertibleToFloat auto l_) { this->lft = l_; this->rt = l_ + this->w; }
void AABB::setRight(ConvertibleToFloat auto r_) { this->rt = r_; this->lft = r_ - this->w; }
void AABB::setBottom(ConvertibleToFloat auto b_) { this->bttm = b_; this->tp = b_ - this->h; };