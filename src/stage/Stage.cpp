#include "Stage.hpp"
#include <memory>
#include <utility>
#include <res/Cfg.hpp>
#include <FSM/duck_fold.hpp>

Stage::Stage(StageType l_type)
	: m_type{ l_type }
	, projectiles_{}
	, player_{}
	, tilemap_{}
	, enemies_{}
	, tilemapSolidTiles_{}
	, worldSpace_{}
{
	worldSpace_ = sf::View({ 800.f, 450.f }, { 1600.f,900.f });
	projectiles_.clear();
	tilemapSolidTiles_.clear();
	enemies_.clear();
}

StageType Stage::getType()
{
	return m_type;
}
std::vector<ASprite*> Stage::GetFloorTiles()
{
	std::vector<ASprite*> floorTiles{};
	floorTiles.clear();
	std::vector<ASprite*> noTiles{};
	noTiles.clear();
	std::vector<ASprite*> checkedTmp{};
	checkedTmp.clear();
	checkedTmp.reserve(tilemapSolidTiles_.size());

	for (auto& tile : tilemapSolidTiles_)
	{
		if ((*tile)().getPosition().x + (*tile)().getOrigin().x > player_->getSpr().getPosition().x - player_->getSpr().getOrigin().x - 0.1f
			&& (*tile)().getPosition().x - (*tile)().getOrigin().x < player_->getSpr().getPosition().x + player_->getSpr().getOrigin().x + 0.1f)
		{
			checkedTmp.emplace_back(dynamic_cast<Tile*>(tile.get()));
		}
	}
	checkedTmp.shrink_to_fit();
	bool firstCheck{ true };
	ASprite* closestToPlayer{nullptr};
	floorTiles.reserve(checkedTmp.size());
	if (checkedTmp.empty())
	{
		return std::move(noTiles);
	}
	else
	{
		for (auto& tile : checkedTmp)
		{
			if (tile->getSpr().getPosition().y - tile->getSpr().getOrigin().y > player_->getSpr().getPosition().y + player_->getSpr().getOrigin().y - ((float)(*tile)().getTextureRect().height / 10.f))
			{
				floorTiles.emplace_back(dynamic_cast<Tile*>(tile));
			}
		}
		floorTiles.shrink_to_fit();
		if (floorTiles.empty())
		{
			return std::move(noTiles);
		}
		else
		{
			return std::move(floorTiles);
		}
	}
	return std::move(noTiles);
}

// call this at end of tick, right after moving the player position based on its velocity
void Stage::CollideFloor()
{
	auto floorTiles{ std::move(GetFloorTiles()) };
	if (floorTiles.empty()) return;
	
	sf::FloatRect playerRect{ {player_->lft, player_->tp},{(float)player_->w, (float)player_->h}}; 

	for (auto& tile : floorTiles)
	{
		sf::FloatRect tileRect{ {tile->lft, tile->tp},{(float)tile->w, (float)tile->h} };
		if (playerRect.intersects(tileRect))
		{
			player_->vy = 0.f;
			player_->SetGrounded(true);
			dispatch(dynamic_cast<Player*>(player_.get())->fsm, evt_Landed{});
			player_->vel().y = 0.f;
			player_->spr_.setPosition((float)player_->getSpr().getPosition().x, (float)(player_->getSpr().getPosition().y - (player_->bttm - tile->tp) - 0.1f));
			player_->setBottom(tile->tp - 0.1f);
		}
	}

}

void Stage::CollideXBefore(float l_dt)
{
	// after x velocity is updated, but before position is updated, check tiles in x vel direction pre moving,
	// like on the Y, but switch a flag to not move if tile edge closest to the player is within the swept area
	// and instead of just stopping, at movement time, use the flag to see that there is a tile to hug, hug the tile instead of moving,
	// and set x velocity to 0.  no tunnelling should happen on the x, then position is updated and THEN the floor tile collision check occurs
	// pushing back up when neccessary
	player_->blockX_ = false;
	player_->blockPosYBefore_ = player_->spr_.getPosition().y;
	if ((!player_->movingLeft_ && !player_->movingRight_) || player_->vel_.x == 0.f) return;

	std::vector<ASprite*> sideTiles{};
	sideTiles.clear();
	std::vector<ASprite*> noTiles{};
	noTiles.clear();
	std::vector<ASprite*> checkedTmp{};
	checkedTmp.clear();
	checkedTmp.reserve(tilemapSolidTiles_.size());

	for (auto& tile : tilemapSolidTiles_)
	{
		if ((*tile)().getPosition().y + (*tile)().getOrigin().y > player_->getSpr().getPosition().y - player_->getSpr().getOrigin().y - 0.1f
			&& (*tile)().getPosition().y - (*tile)().getOrigin().y < player_->getSpr().getPosition().y + player_->getSpr().getOrigin().y + 0.1f)
		{
			checkedTmp.push_back(tile.get());
		}
	}
	checkedTmp.shrink_to_fit();
	bool firstCheck{ true };
	ASprite* closestToPlayer{ nullptr };
	sideTiles.reserve(checkedTmp.size());
	if (checkedTmp.empty())
	{
		goto NO_COLLIDE;
	}
	else
	{

		if (player_->movingRight_ && player_->vel_.x > 0.f)
		{
			for (auto& tile : checkedTmp)
			{
				
					if (tile->getSpr().getPosition().x - tile->getSpr().getOrigin().x > player_->getSpr().getPosition().x + player_->getSpr().getOrigin().x - ((float)(*tile)().getTextureRect().width / 10.f))
					{
						sideTiles.push_back(tile);
					}

			}
		}
		else if (player_->movingLeft_ && player_->vel_.x < 0.f)
		{
			for (auto& tile : checkedTmp)
			{

				if (tile->getSpr().getPosition().x + tile->getSpr().getOrigin().x < player_->getSpr().getPosition().x - player_->getSpr().getOrigin().x + ((float)(*tile)().getTextureRect().width / 10.f))
				{
					sideTiles.push_back(tile);
				}

			}
		}
		else
		{
			goto NO_COLLIDE;
		}
		sideTiles.shrink_to_fit();
		if (sideTiles.empty())
		{
			goto NO_COLLIDE;
		}
		else
		{

			player_->blockXTiles.clear();
			player_->blockXTiles.shrink_to_fit();
			player_->blockXTiles.reserve(sideTiles.size());
			sf::FloatRect playerRect{};
			if (player_->movingRight_)
			  playerRect = { {player_->spr_.getPosition().x - player_->spr_.getOrigin().x,  player_->spr_.getPosition().y - player_->spr_.getOrigin().y },{(float)player_->spr_.getTextureRect().width + player_->vel_.x * l_dt, (float)player_->spr_.getTextureRect().height} };
			else
			  playerRect = { {player_->spr_.getPosition().x - player_->spr_.getOrigin().x + player_->vel_.x * l_dt ,  player_->spr_.getPosition().y - player_->spr_.getOrigin().y },{(float)player_->spr_.getTextureRect().width + player_->vel_.x * l_dt, (float)player_->spr_.getTextureRect().height} };

				
			ASprite* closestTile{ nullptr };
			bool first{ true };
			for (auto& tile : sideTiles)
			{
				sf::FloatRect tileRect{ {tile->spr_.getPosition().x - tile->spr_.getOrigin().x,  tile->spr_.getPosition().y - tile->spr_.getOrigin().y },{(float)tile->spr_.getTextureRect().width, (float)tile->spr_.getTextureRect().height} };

				if (playerRect.intersects(tileRect))
				{
					player_->blockX_ = true;
					if (first)
					{
						first = false;
						closestTile = tile;
					}
					else
					{
						if (player_->movingRight_)
						{
							if (tile->spr_.getPosition().x < closestTile->spr_.getPosition().x)
							{
								closestTile = tile;
							}
						}
						else
						{
							if (tile->spr_.getPosition().x > closestTile->spr_.getPosition().x)
							{
								closestTile = tile;
							}
						}
					}
					
					player_->blockXTiles.push_back(tile);
				}
			}
			player_->blockXTiles.shrink_to_fit();
			if (!closestTile)
			{
				player_->closestTile_ = nullptr;
			}
			else
			{
				player_->closestTile_ = closestTile;
			}

		}
	}
NO_COLLIDE:

player_->blockVelX_ = player_->vel_.x;
player_->vel_.x = 0.f;
}
void Stage::FinishCollidePlayer(const sf::Time& l_dt)
{
	player_->vel_.x = player_->blockVelX_;
	if (player_->blockX_)
	{
		// dont move on x, sweep

		// y has been set, reset the xVelocity if blockX is true and resolve x checks
		if (player_->blockPosYAfter_ - player_->blockPosYBefore_ == 0.f)
		{
			//player did not move on the y, sweep
			sf::FloatRect playerRect{};
			if (player_->movingRight_)
				playerRect = { {player_->spr_.getPosition().x - player_->spr_.getOrigin().x,  player_->spr_.getPosition().y - player_->spr_.getOrigin().y },{(float)player_->spr_.getTextureRect().width + player_->vel_.x * l_dt.asSeconds(), (float)player_->spr_.getTextureRect().height} };
			else
				playerRect = { {player_->spr_.getPosition().x - player_->spr_.getOrigin().x + player_->vel_.x * l_dt.asSeconds() ,  player_->spr_.getPosition().y - player_->spr_.getOrigin().y},{(float)player_->spr_.getTextureRect().width + player_->vel_.x * l_dt.asSeconds(), (float)player_->spr_.getTextureRect().height} };

			if (player_->closestTile_)
			{
				player_->vx = 0.f;
				player_->vel().x = 0.f;
				if (player_->movingRight_)
				{
					player_->spr_.setPosition((float)player_->getSpr().getPosition().x + (player_->closestTile_->spr_.getPosition().x - player_->getSpr().getPosition().x) - player_->spr_.getOrigin().x - player_->closestTile_->spr_.getOrigin().x - 0.1f, player_->getSpr().getPosition().y);
					player_->setRight(player_->closestTile_->spr_.getPosition().x - player_->closestTile_->spr_.getOrigin().x - 0.1f);
				}
				else
				{
					player_->spr_.setPosition((float)player_->getSpr().getPosition().x - (player_->getSpr().getPosition().x - player_->closestTile_->spr_.getPosition().x) + player_->spr_.getOrigin().x + player_->closestTile_->spr_.getOrigin().x + 0.1f, player_->getSpr().getPosition().y);
					player_->setLeft(player_->closestTile_->spr_.getPosition().x + player_->closestTile_->spr_.getOrigin().x + 0.1f);

				}
			}
		}
		else
		{
			if (player_->blockPosYAfter_ - player_->blockPosYBefore_ > 0.f)
			{
				//player moving down
				//player did not move on the y, sweep
				sf::FloatRect playerRect{};
				if (player_->movingRight_)
					playerRect = { {player_->spr_.getPosition().x - player_->spr_.getOrigin().x,  player_->spr_.getPosition().y - player_->spr_.getOrigin().y },{(float)player_->spr_.getTextureRect().width + player_->vel_.x * l_dt.asSeconds(), (float)player_->spr_.getTextureRect().height} };
				else
					playerRect = { {player_->spr_.getPosition().x - player_->spr_.getOrigin().x + player_->vel_.x * l_dt.asSeconds() ,  player_->spr_.getPosition().y - player_->spr_.getOrigin().y},{(float)player_->spr_.getTextureRect().width + player_->vel_.x * l_dt.asSeconds(), (float)player_->spr_.getTextureRect().height} };

				if (player_->closestTile_)
				{
					player_->vx = 0.f;
					player_->vel().x = 0.f;
					if (player_->movingRight_)
					{
						player_->spr_.setPosition((float)player_->getSpr().getPosition().x + (player_->closestTile_->spr_.getPosition().x - player_->getSpr().getPosition().x) - player_->spr_.getOrigin().x - player_->closestTile_->spr_.getOrigin().x - 0.1f, player_->getSpr().getPosition().y);
						player_->setRight(player_->closestTile_->spr_.getPosition().x - player_->closestTile_->spr_.getOrigin().x - 0.1f);
					}
					else
					{
						player_->spr_.setPosition((float)player_->getSpr().getPosition().x - (player_->getSpr().getPosition().x - player_->closestTile_->spr_.getPosition().x) + player_->spr_.getOrigin().x + player_->closestTile_->spr_.getOrigin().x + 0.1f, player_->getSpr().getPosition().y);
						player_->setLeft(player_->closestTile_->spr_.getPosition().x + player_->closestTile_->spr_.getOrigin().x + 0.1f);

					}
				}
			}
			else
			{
				if (player_->blockPosYAfter_ - player_->blockPosYBefore_ > 0.f)
				{
					// player moving up
					//player did not move on the y, sweep
					sf::FloatRect playerRect{};
					if (player_->movingRight_)
						playerRect = { {player_->spr_.getPosition().x - player_->spr_.getOrigin().x,  player_->spr_.getPosition().y - player_->spr_.getOrigin().y },{(float)player_->spr_.getTextureRect().width + player_->vel_.x * l_dt.asSeconds(), (float)player_->spr_.getTextureRect().height} };
					else
						playerRect = { {player_->spr_.getPosition().x - player_->spr_.getOrigin().x + player_->vel_.x * l_dt.asSeconds() ,  player_->spr_.getPosition().y - player_->spr_.getOrigin().y},{(float)player_->spr_.getTextureRect().width + player_->vel_.x * l_dt.asSeconds(), (float)player_->spr_.getTextureRect().height} };

					if (player_->closestTile_)
					{
						player_->vx = 0.f;
						player_->vel().x = 0.f;
						if (player_->movingRight_)
						{
							player_->spr_.setPosition((float)player_->getSpr().getPosition().x + (player_->closestTile_->spr_.getPosition().x - player_->getSpr().getPosition().x) - player_->spr_.getOrigin().x - player_->closestTile_->spr_.getOrigin().x - 0.1f, player_->getSpr().getPosition().y);
							player_->setRight(player_->closestTile_->spr_.getPosition().x - player_->closestTile_->spr_.getOrigin().x - 0.1f);
						}
						else
						{
							player_->spr_.setPosition((float)player_->getSpr().getPosition().x - (player_->getSpr().getPosition().x - player_->closestTile_->spr_.getPosition().x) + player_->spr_.getOrigin().x + player_->closestTile_->spr_.getOrigin().x + 0.1f, player_->getSpr().getPosition().y);
							player_->setLeft(player_->closestTile_->spr_.getPosition().x + player_->closestTile_->spr_.getOrigin().x + 0.1f);

						}
					}
				}
			}
		}
	}
	else
	{
		// do regular movement on x
		float tmp = player_->vel_.y;
		player_->vel_.y = 0;
		player_->tickPos(l_dt.asSeconds());
		player_->vel_.y = tmp;
	}
	// moving done
	player_->blockPosYAfter_ = 0.f;
	player_->blockPosYBefore_ = 0.f;
	player_->closestTile_ = nullptr;
	player_->blockX_ = false;
	player_->blockXTiles.clear();
	player_->blockXTiles.shrink_to_fit();
	player_->blockVelX_ = 0.f;
	//reset params
}

void Stage::SetupPlayerMovement(const sf::Time& l_dt)
{
	// apply gravity
	if (player_->affectedByGravity_)
	{
		// update the player after handling the collision checking
		if (!player_->GetGrounded())
		{
			player_->vel().y += Cfg::Gravity * l_dt.asSeconds();
			dispatch(dynamic_cast<Player*>(player_.get())->fsm, evt_Fell {});

		}
	}
	// apply input force, if any
	if (!player_->movingLeft_ && !player_->movingRight_)
	{
		player_->vel_.x = 0.f;

		dynamic_cast<Player*>(player_.get())->SetMoving(false, player_->facingRight_);
		dispatch(dynamic_cast<Player*>(player_.get())->fsm, evt_StoppedMoving {});
	}
	player_->animMgr.SetFacingRight(player_->facingRight_);
	if (dynamic_cast<Player*>(player_.get())->IsMoving())
	{
		if (dynamic_cast<Player*>(player_.get())->facingRight_)
			player_->vel().x = Player::MoveSpeed;
		else
			player_->vel().x = -Player::MoveSpeed;

		if (player_->vel().x == 0.f)
		{
			dynamic_cast<Player*>(player_.get())->SetMoving(false, dynamic_cast<Player*>(player_.get())->facingRight_);
			dispatch(dynamic_cast<Player*>(player_.get())->fsm, evt_StoppedMoving {});
		}
		if (player_->vel().x > 0.f)
		{
			// check tiles to the right and see if any are in the area from where the player is, to where it will be,
			// those will be deep checked
		}
		else if (player_->vel().x < 0.f)
		{
			// check tiles to the right and see if any are in the area from where the player is, to where it will be,
			// those will be deep checked
		}
	}
}


void Stage::HandlePlayerMovement(const sf::Time& l_dt)
{
	SetupPlayerMovement(l_dt);
	
	//setup X
	CollideXBefore(l_dt.asSeconds());

	// update y pos
	player_->updateBase(l_dt);

	// collide y only
	CollideYAfter();

	// restore movement on x, tick and collide to the right position
	FinishCollidePlayer(l_dt);

}
void Stage::CollideYAfter()
{


	std::vector<ASprite*> floorTiles{};
	floorTiles.clear();
	std::vector<ASprite*> noTiles{};
	noTiles.clear();
	std::vector<ASprite*> checkedTmp{};
	checkedTmp.clear();
	checkedTmp.reserve(tilemapSolidTiles_.size());

	for (auto& tile : tilemapSolidTiles_)
	{
		if ((*tile)().getPosition().x + (*tile)().getOrigin().x > player_->getSpr().getPosition().x - player_->getSpr().getOrigin().x - 0.1f
			&& (*tile)().getPosition().x - (*tile)().getOrigin().x < player_->getSpr().getPosition().x + player_->getSpr().getOrigin().x + 0.1f)
		{
			checkedTmp.push_back(tile.get());
		}
	}
	checkedTmp.shrink_to_fit();
	bool firstCheck{ true };
	ASprite* closestToPlayer{ nullptr };
	floorTiles.reserve(checkedTmp.size());
	if (checkedTmp.empty())
	{
		goto NO_COLLIDE;
	}
	else
	{
		for (auto& tile : checkedTmp)
		{
			if (tile->getSpr().getPosition().y - tile->getSpr().getOrigin().y > player_->getSpr().getPosition().y + player_->getSpr().getOrigin().y - ((float)(*tile)().getTextureRect().height / 10.f))
			{
				floorTiles.push_back(tile);
			}
		}
		floorTiles.shrink_to_fit();
		if (floorTiles.empty())
		{
			goto NO_COLLIDE;
		}
		else
		{
			sf::FloatRect playerRect{ {player_->spr_.getPosition().x - player_->spr_.getOrigin().x,  player_->spr_.getPosition().y - player_->spr_.getOrigin().y },{(float)player_->spr_.getTextureRect().width, (float)player_->spr_.getTextureRect().height} };

			for (auto& tile : floorTiles)
			{
				sf::FloatRect tileRect{ {tile->spr_.getPosition().x - tile->spr_.getOrigin().x,  tile->spr_.getPosition().y - tile->spr_.getOrigin().y },{(float)tile->spr_.getTextureRect().width, (float)tile->spr_.getTextureRect().height} };

				//if (playerRect.intersects(tileRect))
				if (player_->spr_.getPosition().y + player_->spr_.getOrigin().y >= tile->spr_.getPosition().y - tile->spr_.getOrigin().y)
				{
					player_->vy = 0.f;
					player_->SetGrounded(true);
					dispatch(dynamic_cast<Player*>(player_.get())->fsm, evt_Landed {});
					player_->vel().y = 0.f;
					player_->spr_.setPosition((float)player_->getSpr().getPosition().x, (float)(player_->getSpr().getPosition().y - ((player_->spr_.getPosition().y + player_->spr_.getOrigin().y) - (tile->spr_.getPosition().y - tile->spr_.getOrigin().y)) - 0.1f));
					player_->setBottom(tile->tp - 0.1f);
				}
			}

		}
	}
NO_COLLIDE:

	if (!player_->GetGrounded())
	{
		dispatch(dynamic_cast<Player*>(player_.get())->fsm, evt_Fell {});
	}

	player_->blockPosYAfter_ = player_->spr_.getPosition().y;
}


	

void Stage::CreateFriendlyBullet(ASprite& l_spr, SpriteName l_name)
{
	sf::Vector2f pos = { (!l_spr.IsFacingRight()) ? l_spr().getPosition().x - l_spr().getOrigin().x + 10.f : l_spr().getPosition().x + l_spr().getOrigin().x - 10.f, l_spr().getPosition().y - l_spr().getOrigin().y + ((float)l_spr().getTextureRect().height / 3.f) + 18.f };

	switch (l_name)
	{
	case SpriteName::BusterBullet:
	{
		projectiles_.reserve(projectiles_.size() + 1);
		projectiles_.emplace_back(std::make_shared<Bullet>(Cfg::textures.get((int)Cfg::Textures::Bullet1), player_->facingRight_));
		(*projectiles_.back())().setTextureRect({ { 0, 0 }, { 24 , 18 } });
		(*projectiles_.back())().setOrigin({ 12.f, 9.f });
		(*projectiles_.back()).vel() = { (l_spr.IsFacingRight()) ? 600.f : -600.f, 0.f };
		(*projectiles_.back())().setPosition(pos);
	}
		break;
	default:
		break;
	}
}
