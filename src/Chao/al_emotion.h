#ifndef _AL_EMOTION_H_
#define _AL_EMOTION_H_

#include <task.h>

int AL_EmotionGetValue(task* tp, Uint32 EmotionNum);
void AL_EmotionSetValue(task* tp, Uint32 EmotionNum, int value);
void AL_EmotionAdd(task* tp, Uint32 EmotionNum, int add);

#endif