#if !defined(PARTICLE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "XTexture.h"
#include "XTank.h"

const int TOTAL_PARTICLES = 10;

struct ParticlesTexture{
    XTexture* RedTexture;
    XTexture* BlueTexture;
    XTexture* GreenTexture;
    XTexture* ShimmerTexture;

    ParticlesTexture(){
     RedTexture = nullptr;
     RedTexture = new XTexture();
     BlueTexture = nullptr;
     BlueTexture = new XTexture();
     GreenTexture = nullptr;
     GreenTexture = new XTexture();
     ShimmerTexture = nullptr;        
     ShimmerTexture = new XTexture();
    }
};

struct Particle{
    // Offsets
    uint8_t mPosX;
    uint8_t mPosY;
    uint8_t mFrame = 0;

    XTexture* pTexture;
    
    Particle(){
        mPosX = 0;
        mPosY = 0;
        mFrame = 0;
        pTexture = nullptr;
        pTexture = new XTexture;
    };
};

void InitializeParticle(Particle* pa, TankInfo* userTank, ParticlesTexture* particlestexture);
bool isDead(Particle* pa);

#define PARTICLE_H
#endif
