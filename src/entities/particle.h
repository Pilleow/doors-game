//
// Created by igor on 2024.08.09..
//

#ifndef DOORS_GAME_PARTICLE_H
#define DOORS_GAME_PARTICLE_H

#include <raylib.h>

struct Particle {
    bool enabled;
    Rectangle rect;
    int rotation;
    int rotationSpeed;
    Vector2 velocity;
    unsigned int speed;
    float decayTimeLeft;
    Color color;
};

void InitParticle(struct Particle *pa, Vector2 position, int size, int speed, Vector2 velocity, float decayTime, Color color, int rotSpeed);

void UpdateParticle(struct Particle *pa);

void DrawParticle(struct Particle *pa);

#endif //DOORS_GAME_PARTICLE_H
