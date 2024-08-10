//
// Created by igor on 2024.08.09..
//

#include <stdlib.h>
#include "particle.h"

void
InitParticle(struct Particle *pa, Vector2 position, int size, int speed, Vector2 velocity, float decayTime, Color color,
             int rotSpeed) {
    pa->enabled = true;
    pa->rotation = rand() % 360;
    pa->rect = (Rectangle) {
            position.x, position.y, size, size
    };
    pa->speed = speed;
    pa->velocity = velocity;
    pa->decayTimeLeft = decayTime;
    pa->color = color;
    pa->rotationSpeed = rotSpeed;
}

void UpdateParticle(struct Particle *pa) {
    pa->speed *= 0.9;
    pa->decayTimeLeft -= GetFrameTime();
    if (pa->decayTimeLeft <= 0) {
        pa->enabled = false;
    } else if (pa->decayTimeLeft < 0.3) {
        pa->rect.width *= 0.9;
        pa->rect.height *= 0.9;
    }
    pa->rotation += pa->rotationSpeed;
    pa->rect.x += pa->velocity.x * pa->speed * GetFrameTime();
    pa->rect.y += pa->velocity.y * pa->speed * GetFrameTime();
}

void DrawParticle(struct Particle *pa) {
    if (!pa->enabled) return;
    DrawRectanglePro(
            pa->rect,
            (Vector2) {0, 0},
            pa->rotation,
            pa->color
    );
}
