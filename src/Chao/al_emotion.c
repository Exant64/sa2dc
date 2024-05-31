#include "Chao/Chao.h"

void AL_EmotionAdd(task* tp, Uint32 EmotionNum, int add) {
    int value;
    AL_EMOTION* emotion;

    // this is odd, but i can't match it any other way
    if (emotion = &GET_CHAOPARAM(tp)->Emotion, EmotionNum <= 7) {
        value = add + (Uint8)emotion->Mood[EmotionNum];

        if (value > 200)
            value = 200;
        if (value < 0)
            value = 0;
        emotion->Mood[EmotionNum] = value;
    } else if (EmotionNum >= 8 && EmotionNum <= 18) {
        value = add + emotion->State[EmotionNum - 8];

        if (value > 10000)
            value = 10000;
        if (value < 0)
            value = 0;
        emotion->State[EmotionNum - 8] = value;
    } else if (EmotionNum >= 19 && EmotionNum <= 31) {
        value = add + emotion->Personality[EmotionNum - 19];

        if (value > 100)
            value = 100;
        if (value < -100)
            value = -100;
        emotion->Personality[EmotionNum - 19] = value;
    }
}

void AL_EmotionSetValue(task* tp, Uint32 EmotionNum, int value) {
    AL_EMOTION* emotion;
    emotion = &GET_CHAOWK(tp)->param.Emotion;
    if (EmotionNum <= 7) {
        if (value > 200)
            value = 200;
        if (value < 0)
            value = 0;
        emotion->Mood[EmotionNum] = value;
    } else if (EmotionNum >= 8 && EmotionNum <= 18) {
        if (value > 10000)
            value = 10000;
        if (value < 0)
            value = 0;
        emotion->State[EmotionNum - 8] = value;
    } else if (EmotionNum >= 19 && EmotionNum <= 31) {
        if (value > 100)
            value = 100;
        if (value < -100)
            value = -100;
        emotion->Personality[EmotionNum - 19] = value;
    }
}

int AL_EmotionGetValue(task* tp, Uint32 EmotionNum) {
    AL_EMOTION* emotion;
    emotion = &GET_CHAOWK(tp)->param.Emotion;
    if (EmotionNum <= 7) {
        return (Uint8)emotion->Mood[EmotionNum];
    } else if (EmotionNum >= 8 && EmotionNum <= 18) {
        return emotion->State[EmotionNum - 8];
    } else if (EmotionNum >= 19 && EmotionNum <= 31) {
        return emotion->Personality[EmotionNum - 19];
    }
}