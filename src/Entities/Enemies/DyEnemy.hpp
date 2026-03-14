#pragma once
#include "Enemy.hpp"

class DyEnemy : public Enemy {
    private: 
        float angle = 135;
        float aimAngle = 225;
        bool loop = false;

        Rectangle textureRect;

    public:
        DyEnemy(float x, float y) : Enemy(x, y) { 
            this->cooldown = GetRandomValue(90, 300);
            this->health = 1; 

            if(GetRandomValue(0,1) == 0){
                textureRect = Rectangle{0,147,13,13};
            }
            else{
                textureRect = Rectangle{2,147,13,13};
            }
        }

        void draw() override;
        void update(std::pair<float, float> pos, HitBox target) override;
        void attack(HitBox target) override;
};