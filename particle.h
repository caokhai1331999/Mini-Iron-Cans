#if !defined(PARTICLE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "LTexture.h"

struct Particle{
    uin8_t x;
    uin8_t y;

    uint8_t mFrame;
    

    
    Particle{
        x = 0;
        y = 0;
        mFrame = 20;
     RedTexture = nullptr;
     GreenTexture = nullptr;
     BlueTexture = nullptr;
     ShimmerTexture = nullptr;        
    };
};

void RunParticleEngine(Particle* pa);
void SetParticlePos(TankInfo* userTank);
void RenderParticle(Particle* pa);

#define PARTICLE_H
#endif
