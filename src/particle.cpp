/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "particle.h"

void InitializeParticle(Particle* pa, TankInfo* userTank, ParticlesTexture* particletextures){

    // NOTE: %Xh stand for hexadecimal number

    // put srand() inside this will cause overlapped position of different particles
    printf("current texture address is: %Xh\n", pa);
    printf("current texture position X address is: %Xh\n", &pa->mPosX);
    printf("current texture position Y address is: %Xh\n", &pa->mPosY);
    printf("Particle pos:%d %d \n", pa->mPosX, pa->mPosY);    
    
    pa->mPosX = userTank->mBox.x - 5 +(rand()%65);
    pa->mPosY = userTank->mBox.y - 5 +(rand()%40);

    // Initialize animation
    pa->mFrame = rand()%5;//WHY??

    switch(rand()%3){
            case 0 :pa->pTexture = particletextures->RedTexture;break;
            case 1 :pa->pTexture = particletextures->BlueTexture;break;
            case 2 :pa->pTexture = particletextures->GreenTexture;break;
        // default : ;break;
        }

};

bool isDead(Particle* pa){
    return pa->mFrame >= 20;
}
