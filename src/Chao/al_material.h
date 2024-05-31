#ifndef _AL_MATERIAL_H_
#define _AL_MATERIAL_H_

#include <Chao/Chao.h>

void AL_MatChangeObjectTexture(al_object* pObject, Uint16 TexId);
void AL_MatChangeObjectTexture2(al_object* pObject, const Uint16* TexIdList, int nbTex);

#endif