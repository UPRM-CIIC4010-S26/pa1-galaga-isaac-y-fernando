#pragma once
#include "Projectile.hpp"
#include "Math.hpp"
#include "ImageManager.hpp"
#include "Animation.hpp"
#include <iostream>
#include <vector>
#include <utility>

class Enemy {
protected:
    float angle = 90;
    int cooldown = 60;
    int speed = 3;
    bool spawning = false;
    bool frame = false;
    int frameCooldown = 30;

public:
    int health = 1;
    std::pair<float, float> position;
    HitBox hitBox;

    inline static float direction = 0.5f;
    inline static int directionChange = 100;
    inline static std::vector<std::pair<std::pair<float, float>, Enemy*>> enemies;

    Enemy() {}

    Enemy(float x, float y) {
        position.first = x;
        position.second = y;
        hitBox = HitBox(x, y, 30, 30);
    }

    virtual ~Enemy() = default;

    virtual void draw() = 0;
    virtual void update(std::pair<float, float> pos, HitBox target) = 0;
    virtual void attack(HitBox target) = 0;

    void frameChange() {
        frameCooldown--;

        if (frameCooldown <= 0) {
            frame = !frame;
            frameCooldown = 30;
        }
    }

    static void ManageEnemies(HitBox target, int& score) {
    for (auto& enemyPair : Enemy::enemies) {
        enemyPair.first.first += (enemyPair.first.first == 0) ? 0 : direction;

        if (!enemyPair.second) continue;

        enemyPair.second->update(enemyPair.first, target);

        for (Projectile& proj : Projectile::projectiles) {
            if (proj.ID != 1 && HitBox::Collision(enemyPair.second->hitBox, proj.getHitBox())) {
                enemyPair.second->health--;
                proj.del = true;
            }
        }

        if (enemyPair.second->health <= 0) {
            score += 100;

            Animation::animations.push_back(
                Animation(
                    enemyPair.second->position.first,
                    enemyPair.second->position.second,
                    155, 0, 33, 33,
                    30, 30,
                    4,
                    ImageManager::SpriteSheet
                )
            );

            delete enemyPair.second;
            enemyPair.second = nullptr;
        }
    }

    for (int i = 0; i < (int)Enemy::enemies.size();) {
        bool eraseEnemy = false;

        if (Enemy::enemies[i].second &&
            Enemy::enemies[i].second->position.first <= -30) {
            delete Enemy::enemies[i].second;
            Enemy::enemies[i].second = nullptr;
            eraseEnemy = true;
        }
        else if (!Enemy::enemies[i].second &&
                 Enemy::enemies[i].first.first == 0 &&
                 Enemy::enemies[i].first.second == 0) {
            eraseEnemy = true;
        }

        if (eraseEnemy) {
            Enemy::enemies.erase(Enemy::enemies.begin() + i);
        } else {
            i++;
        }
    }

    directionChange++;

    if (directionChange >= 200) {
        directionChange = 0;
        direction *= -1;
        }
    }
};