#include <task.h>
#include <motion.h>

#define MTN_FLAG_END BIT_0
#define MTN_FLAG_LINK BIT_1
#define MTN_FLAG_SYNC BIT_2
#define MTN_FLAG_STOP BIT_3
#define MTN_FLAG_LKLP BIT_4

void SetMotionSkip(MOTION_CTRL *pMtnCtrl, int MtnNum);
void lbl_8C0337CC(MOTION_CTRL *pMtnCtrl, int MtnNum);
void SetMotionChange(MOTION_CTRL *pMtnCtrl, int MtnNum);
void lbl_8C03356E(MOTION_CTRL *pMtnCtrl);

void MotionInit(MOTION_CTRL *pMtnCtrl, MOTION_TABLE *pTable) {
    MOTION_INFO* pInfo;
    
    pMtnCtrl->flag = 0;
    pMtnCtrl->posture = 0;
    pMtnCtrl->curr_num = 0;
    pMtnCtrl->next_num = -1;
    pMtnCtrl->multi_spd = 0;
    pMtnCtrl->link_spd = 0;
    pMtnCtrl->ratio = 0;

    pInfo = &pMtnCtrl->minfo[0];
    pInfo->frame = 0;
    pInfo->start = 0;
    pInfo->end = 0;
    pInfo->pMotion = NULL;

    pInfo = &pMtnCtrl->minfo[1];
    pInfo->frame = 0;
    pInfo->start = 0;
    pInfo->end = 0;
    pInfo->pMotion = NULL;

    pMtnCtrl->table = pTable;
    
    SetMotionSkip(pMtnCtrl, 0);    
}

void MotionControl(MOTION_CTRL *pMtnCtrl) {
    MOTION_INFO* pInfo0 = &pMtnCtrl->minfo[0];
    MOTION_INFO* pInfo1 = &pMtnCtrl->minfo[1];
    Bool ended = FALSE;
    
    pMtnCtrl->flag &= ~(MTN_FLAG_END | MTN_FLAG_STOP);

    if(pMtnCtrl->flag & MTN_FLAG_LINK) {
        if(pMtnCtrl->link_spd < 0) pMtnCtrl->link_spd *= -1;

        pMtnCtrl->ratio += pMtnCtrl->link_spd * pMtnCtrl->multi_spd;

        if(pMtnCtrl->flag & MTN_FLAG_SYNC) {
            Bool ended1;
            
            pInfo0->frame += pInfo0->spd * pMtnCtrl->multi_spd;

            if(pInfo0->spd >= 0) {
                if(pInfo0->frame >= pInfo0->end) {
                    ended = TRUE;
                }
            }
            else {
                if(pInfo0->frame <= pInfo0->end) {
                    ended = TRUE;
                }
            }

            if(ended) {
                pInfo0->frame -= pInfo0->end - pInfo0->start;
            }

            ended1 = FALSE;
            pInfo1->frame += pInfo1->spd * pMtnCtrl->multi_spd;
            if(pInfo1->spd >= 0) {
                if(pInfo1->frame >= pInfo1->end) {
                    ended1 = TRUE;
                }
            }
            else {
                if(pInfo1->frame <= pInfo1->end) {
                    ended1 = TRUE;
                }
            }

            if(ended1) {
                pInfo1->frame -= pInfo1->end - pInfo1->start;
            }

            if(pMtnCtrl->ratio >= 1.f) {
                pMtnCtrl->ratio = 1.f;
                lbl_8C0337CC(pMtnCtrl, pMtnCtrl->next_num);
            }
        }
        else {
            if(pMtnCtrl->ratio >= 1.f) {
                if(pMtnCtrl->flag & MTN_FLAG_LKLP) {
                    if(pInfo0->spd > 0) {
                        pInfo0->frame = pInfo0->start + (pMtnCtrl->ratio - 1);
                    }
                    else {
                        pInfo0->frame = pInfo0->start - (pMtnCtrl->ratio - 1);
                    }

                    pMtnCtrl->ratio = 1;
                    pMtnCtrl->flag &= ~(MTN_FLAG_LKLP | MTN_FLAG_LINK);
                }
                else {
                    pMtnCtrl->ratio = 1;
                    SetMotionSkip(pMtnCtrl, pMtnCtrl->next_num);
                }
            }
        }
    }
    else {
        pInfo0->frame += pInfo0->spd * pMtnCtrl->multi_spd;

        if(pInfo0->spd >= 0.f) {
            if(pInfo0->mode != MD_MTN_ADLP) {
                if(pInfo0->frame >= pInfo0->end ) {
                    ended = TRUE;
                }
            }
            else {
                if(pInfo0->frame >= pInfo0->end + 1) {
                    ended = TRUE;
                }
            }
        }
        else {
            if(pInfo0->mode != MD_MTN_ADLP) {
                if(pInfo0->frame <= pInfo0->end ) {
                    ended = TRUE;
                }
            }
            else {
                if(pInfo0->frame <= pInfo0->end - 1) {
                    ended = TRUE;
                }
            }
        }

        if(ended) {
            switch(pInfo0->mode){
                case MD_MTN_LOOP:
                    pInfo0->frame -= pInfo0->end - pInfo0->start;
                    
                    pMtnCtrl->flag |= MTN_FLAG_END;
                    break;
                case MD_MTN_ADLP:
                    if(pInfo0->spd >= 0) {
                        pInfo0->frame -= (pInfo0->end + 1) - pInfo0->start;
                    }
                    else {
                        pInfo0->frame -= (pInfo0->end - 1) - pInfo0->start;
                    }
                    
                    pMtnCtrl->flag |= MTN_FLAG_END;
                    break;
                case MD_MTN_LKLP: {
                    MOTION_INFO* pInfo1 = &pMtnCtrl->minfo[1];
                    if(pInfo0->spd > 0) {
                        pMtnCtrl->link_spd = pInfo0->spd;
                        pMtnCtrl->ratio = pInfo0->frame - pInfo0->end;
                    }
                    else {
                        pMtnCtrl->link_spd = -pInfo0->spd;
                        pMtnCtrl->ratio = -(pInfo0->frame - pInfo0->end);
                    }

                    if(pMtnCtrl->ratio > 1);

                    pInfo1->mode = pInfo0->mode;
                    pInfo1->frame = pInfo0->start;
                    pInfo1->start = pInfo0->start;
                    pInfo1->end = pInfo0->end;
                    pInfo1->spd = pInfo0->spd;
                    pInfo1->pMotion = pInfo0->pMotion;
                    
                    pMtnCtrl->flag = 0;
                    pMtnCtrl->flag |= MTN_FLAG_END | MTN_FLAG_LINK | MTN_FLAG_LKLP;
                    break;
                }
                case MD_MTN_STOP:
                    pInfo0->frame = pInfo0->end;

                    pMtnCtrl->flag |= MTN_FLAG_END;
                    pMtnCtrl->flag |= MTN_FLAG_STOP;
                    break;
                case MD_MTN_LINK:
                    pInfo0->frame = pInfo0->end;

                    if(pMtnCtrl->next_num != -1) {
                        lbl_8C03356E(pMtnCtrl);
                    }
                    
                    pMtnCtrl->flag |= MTN_FLAG_END;
                    break;
                case MD_MTN_SKIP:
                    if(pMtnCtrl->next_num != -1) {
                        SetMotionSkip(pMtnCtrl, pMtnCtrl->next_num);
                    }
                    
                    pMtnCtrl->flag |= MTN_FLAG_END;
                    break;
                case MD_MTN_CHNG:
                    if(pMtnCtrl->next_num != -1) {
                        SetMotionChange(pMtnCtrl, pMtnCtrl->next_num);
                    }
                    
                    pMtnCtrl->flag |= MTN_FLAG_END;
                    break;
                case MD_MTN_BACK: {
                    float bckp;
                    
                    pInfo0->frame = pInfo0->end;
                    bckp = pInfo0->start;
                    pInfo0->start = pInfo0->end;
                    pInfo0->end = bckp;
                    pInfo0->spd *= -1;

                    pMtnCtrl->flag |= MTN_FLAG_END;
                    break;
                }
            }
        }
    }

    pMtnCtrl->multi_spd = 1.f;
}

void SetMotionLink(MOTION_CTRL *pMtnCtrl, int MtnNum) {
    MOTION_TABLE* pTable = &pMtnCtrl->table[MtnNum];
    MOTION_INFO* pInfo1 = &pMtnCtrl->minfo[1];

    pMtnCtrl->next_num = MtnNum;

    if(pTable->link_step < 0) {
        pMtnCtrl->link_spd = 1.f / 15.f;
    }
    else {
        pMtnCtrl->link_spd = 1.f / pTable->link_step;
    }
    
    pMtnCtrl->ratio = 0.f;
    
    pInfo1->mode = pTable->mode;
    pInfo1->frame = pTable->start;
    pInfo1->start = pTable->start;
    pInfo1->end = pTable->end;
    pInfo1->spd = pTable->spd;
    pInfo1->pMotion = pTable->pMotion;

    pMtnCtrl->flag = 0;
    pMtnCtrl->flag |= MTN_FLAG_LINK;
}

void lbl_8C03356E(MOTION_CTRL *pMtnCtrl) {
    MOTION_INFO* pInfo1 = &pMtnCtrl->minfo[1];
    int curr_num = pMtnCtrl->curr_num;
    int next_num = pMtnCtrl->next_num;
    MOTION_TABLE* pCurrTable = &pMtnCtrl->table[curr_num];
    MOTION_TABLE* pNextTable = &pMtnCtrl->table[next_num];

    if(pCurrTable->link_step < 0) {
        pMtnCtrl->link_spd = 1.f / (-pCurrTable->link_step);
    }
    else {
        pMtnCtrl->link_spd = 1.f / pNextTable->link_step;
    }
    
    pMtnCtrl->ratio = 0.f;
    
    pInfo1->mode = pNextTable->mode;
    pInfo1->frame = pNextTable->start;
    pInfo1->start = pNextTable->start;
    pInfo1->end = pNextTable->end;
    pInfo1->spd = pNextTable->spd;
    pInfo1->pMotion = pNextTable->pMotion;

    pMtnCtrl->flag = 0;
    pMtnCtrl->flag |= MTN_FLAG_LINK;
}

void SetMotionLinkStep(MOTION_CTRL *pMtnCtrl, int MtnNum, Uint16 step) {
    MOTION_TABLE* pTable = &pMtnCtrl->table[MtnNum];
    MOTION_INFO* pInfo1 = &pMtnCtrl->minfo[1];
    
    pMtnCtrl->next_num = MtnNum;
    
    pMtnCtrl->link_spd = 1.f / step;
    pMtnCtrl->ratio = 0.f;
    
    pInfo1->mode = pTable->mode;
    pInfo1->frame = pTable->start;
    pInfo1->start = pTable->start;
    pInfo1->end = pTable->end;
    pInfo1->spd = pTable->spd;
    pInfo1->pMotion = pTable->pMotion;

    pMtnCtrl->flag = 0;
    pMtnCtrl->flag |= MTN_FLAG_LINK;
}

void SetMotionLinkSync(MOTION_CTRL *pMtnCtrl, int MtnNum) {
    MOTION_TABLE* pTable = &pMtnCtrl->table[MtnNum];
    MOTION_INFO* pInfo0 = &pMtnCtrl->minfo[0];
    MOTION_INFO* pInfo1 = &pMtnCtrl->minfo[1];

    pMtnCtrl->next_num = MtnNum;

    if(pTable->link_step < 0) {
        pMtnCtrl->link_spd = 1.f / 15.f;
    }
    else {
        pMtnCtrl->link_spd = 1.f / pTable->link_step;
    }
    
    pMtnCtrl->ratio = 0.f;

    {
        float start = pInfo0->start;
        float end = pInfo0->end;
        float ratio = (pInfo0->frame - start) / (end - start);

        pInfo1->mode = pTable->mode;
        pInfo1->frame = pTable->start + (pTable->end - pTable->start) * ratio;
        pInfo1->start = pTable->start;
        pInfo1->end = pTable->end;
        pInfo1->spd = pTable->spd;
        pInfo1->pMotion = pTable->pMotion;
    }
    
    pMtnCtrl->flag = 0;
    pMtnCtrl->flag |= MTN_FLAG_LINK;

    if(pInfo0->mode == MD_MTN_LOOP && pInfo1->mode == MD_MTN_LOOP) {
        pMtnCtrl->flag |= MTN_FLAG_SYNC;
    }
}

void SetMotionSkip(MOTION_CTRL *pMtnCtrl, int MtnNum) {
    MOTION_TABLE* pTable = &pMtnCtrl->table[MtnNum];
    MOTION_INFO* pInfo0 = &pMtnCtrl->minfo[0];

    pMtnCtrl->curr_num = MtnNum;
    
    pMtnCtrl->next_num = pTable->next;
    pMtnCtrl->posture = pTable->posture;
    
    pMtnCtrl->link_spd = 0.f;
    pMtnCtrl->ratio = 0.f;
    
    pInfo0->mode = pTable->mode;
    pInfo0->frame = pTable->start;
    pInfo0->start = pTable->start;
    pInfo0->end = pTable->end;
    pInfo0->spd = pTable->spd;
    pInfo0->pMotion = pTable->pMotion;

    pMtnCtrl->flag = 0;
}

void SetMotionChange(MOTION_CTRL *pMtnCtrl, int MtnNum) {
    MOTION_TABLE* pTable = &pMtnCtrl->table[MtnNum];
    MOTION_INFO* pInfo0 = &pMtnCtrl->minfo[0];
    
    pMtnCtrl->curr_num = MtnNum;
    
    pMtnCtrl->next_num = pTable->next;
    pMtnCtrl->posture = pTable->posture;
    
    pMtnCtrl->link_spd = 0.f;
    pMtnCtrl->ratio = 0.f;
    
    pInfo0->mode = pTable->mode;
    pInfo0->frame = (pTable->start + pInfo0->frame) - pInfo0->end;
    pInfo0->start = pTable->start;
    pInfo0->end = pTable->end;
    pInfo0->spd = pTable->spd;
    pInfo0->pMotion = pTable->pMotion;

    pMtnCtrl->flag = 0;
}

void lbl_8C0337CC(MOTION_CTRL *pMtnCtrl, int MtnNum)  {
    MOTION_TABLE* pTable = &pMtnCtrl->table[MtnNum];
    MOTION_INFO* pInfo0 = &pMtnCtrl->minfo[0];
    MOTION_INFO* pInfo1 = &pMtnCtrl->minfo[1];
    
    pMtnCtrl->curr_num = MtnNum;
    
    pMtnCtrl->next_num = pTable->next;
    pMtnCtrl->posture = pTable->posture;
    
    pMtnCtrl->link_spd = 0.f;
    pMtnCtrl->ratio = 0.f;
    
    pInfo0->mode = pInfo1->mode;
    pInfo0->frame = pInfo1->frame;
    pInfo0->start = pInfo1->start;
    pInfo0->end = pInfo1->end;
    pInfo0->spd = pInfo1->spd;
    pInfo0->pMotion = pInfo1->pMotion;

    pMtnCtrl->flag = 0;
}

void SetMotionSpd(MOTION_CTRL *pMtnCtrl, float spd) {
    pMtnCtrl->multi_spd = spd;
}

Bool IsMotionEnd(MOTION_CTRL *pMtnCtrl) {
    if(pMtnCtrl->flag & MTN_FLAG_END) {
        return TRUE;
    }

    return FALSE;
}

Bool IsMotionStop(MOTION_CTRL *pMtnCtrl) {
    if(pMtnCtrl->flag & MTN_FLAG_STOP) {
        return TRUE;
    }

    return FALSE;
}

void haCnkEasyDrawMotion(NJS_CNK_OBJECT *pObject, MOTION_CTRL *pMtnCtrl) {
    MOTION_INFO* pInfo0 = &pMtnCtrl->minfo[0];
    MOTION_INFO* pInfo1 = &pMtnCtrl->minfo[1];
    
    NJS_MOTION_LINK motionlink;

    if(pMtnCtrl->flag & MTN_FLAG_LINK) {
        motionlink.motion[0] = pInfo0->pMotion;
        motionlink.motion[1] = pInfo1->pMotion;
        motionlink.frame[0] = pInfo0->frame;
        motionlink.frame[1] = pInfo1->frame;

        njCnkEasyDrawMotionLink(pObject, &motionlink, pMtnCtrl->ratio);
    }
    else {
        njCnkEasyDrawMotion(pObject, pInfo0->pMotion, pInfo0->frame);
    }
}

void haCnkSimpleDrawMotion(NJS_CNK_OBJECT *pObject, MOTION_CTRL *pMtnCtrl) {
    MOTION_INFO* pInfo0 = &pMtnCtrl->minfo[0];
    MOTION_INFO* pInfo1 = &pMtnCtrl->minfo[1];
    
    NJS_MOTION_LINK motionlink;

    if(pMtnCtrl->flag & MTN_FLAG_LINK) {
        motionlink.motion[0] = pInfo0->pMotion;
        motionlink.motion[1] = pInfo1->pMotion;
        motionlink.frame[0] = pInfo0->frame;
        motionlink.frame[1] = pInfo1->frame;

        njCnkSimpleDrawMotionLink(pObject, &motionlink, pMtnCtrl->ratio);
    }
    else {
        njCnkSimpleDrawMotion(pObject, pInfo0->pMotion, pInfo0->frame);
    }
}

void SetMotionTable(MOTION_CTRL* pMtnCtrl, MOTION_TABLE* pTable) {
    pMtnCtrl->table = pTable;
}

void SetMotionFrame(MOTION_CTRL *pMtnCtrl, float frame) {
    MOTION_INFO* pInfo = &pMtnCtrl->minfo[0];

    if(pInfo->spd >= 0) {
        if(frame < pInfo->start) {
            frame = pInfo->start;
        }
        else if (frame > pInfo->end) {
            frame = pInfo->end;
        }
    }
    else {
        if(frame < pInfo->end) {
            frame = pInfo->end;
        }
        else if(frame > pInfo->start) {
            frame = pInfo->start;
        }
    }

    pInfo->frame = frame;
}

void sub_8C033960(MOTION_CTRL *pMtnCtrl, float ratio) {
    MOTION_INFO* pInfo = &pMtnCtrl->minfo[0];
    float start = pInfo->start;
    float end = pInfo->end;

    if(ratio < 0) ratio = 0;
    else if(ratio > 1) ratio = 1;

    pInfo->frame = start + (end-start) * ratio;
}

float GetMotionFrame(MOTION_CTRL* pMtnCtrl) {
    return pMtnCtrl->minfo[0].frame;
}

float sub_8C033990(MOTION_CTRL* pMtnCtrl) {
    MOTION_INFO* pInfo = &pMtnCtrl->minfo[0];
    float start = pInfo->start;
    float end = pInfo->end;
    float frame = pMtnCtrl->minfo[0].frame;

    return (frame - start) / (end - start);
}

int GetMotionNum(MOTION_CTRL* pMtnCtrl) {
    return pMtnCtrl->curr_num;
}

int GetMotionPosture(MOTION_CTRL* pMtnCtrl) {
    return pMtnCtrl->posture;
}

void lbl_8C0339B4(MOTION_CTRL* pMtnCtrl, int x, int y) {
    njPrintC(NJM_LOCATION(x, y), "[MOTION INFO]");
    y++;
    
    njPrintC(NJM_LOCATION(x, y), "CURR:");
    njPrintD(NJM_LOCATION(x + 6, y), pMtnCtrl->curr_num, 2);
    y++;
    
    njPrintC(NJM_LOCATION(x, y), "NEXT:");
    njPrintD(NJM_LOCATION(x + 6, y), pMtnCtrl->next_num, 2);
    y++;
    
    njPrintC(NJM_LOCATION(x, y), "FRM :");
    njPrintD(NJM_LOCATION(x + 6, y), pMtnCtrl->minfo[0].frame, 2);
    y++;
    
    njPrintC(NJM_LOCATION(x, y), "LNK :");
    njPrintD(NJM_LOCATION(x + 6, y), (Uint32)(pMtnCtrl->ratio * 100), 2);
    y++;
    
    njPrintC(NJM_LOCATION(x, y), "POSE:");
    njPrintD(NJM_LOCATION(x + 6, y), pMtnCtrl->posture, 2);
}
