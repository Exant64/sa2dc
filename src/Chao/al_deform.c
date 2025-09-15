#include <task.h>

#include <task.h>
#include <Chao/Chao.h>

#define PART_ATTR_MODEL 0x1
#define PART_ATTR_NODE  0x2

extern Uint16 AL_PartAttr[];
//Uint16 AL_PartAttr[] = { 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 4, 5, 2, 4, 5, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 0, 2, 2, 2, 3, 2, 3 };

extern al_object** CurrObjectList;
extern NJS_CNK_OBJECT** ZeroObjectList;
extern NJS_CNK_OBJECT** ZeroObjectListH;
extern NJS_CNK_OBJECT** ZeroObjectListD;
extern NJS_CNK_OBJECT** NormalObjectList;
extern NJS_CNK_OBJECT** NormalObjectListH;
extern NJS_CNK_OBJECT** NormalObjectListD;
extern NJS_CNK_OBJECT** HorizonObjectList;
extern NJS_CNK_OBJECT** HorizonObjectListH;
extern NJS_CNK_OBJECT** HorizonObjectListD;
extern NJS_CNK_OBJECT** VerticalObjectList;
extern NJS_CNK_OBJECT** VerticalObjectListH;
extern NJS_CNK_OBJECT** VerticalObjectListD;

extern Float ratio_h;
extern Float ratio_v;
extern Float ratio_g;
extern Float inv_ratio_h;
extern Float inv_ratio_v;
extern Float inv_ratio_g;
extern Float div_ratio_h;
extern Float div_ratio_v;
extern Float col_ratio_h;
extern Float col_ratio_v;
extern Float col_ratio_g;
extern Float col_inv_ratio_h;
extern Float col_inv_ratio_v;
extern Float col_inv_ratio_g;
extern Float col_div_ratio_h;
extern Float col_div_ratio_v;

#define lerp(a,b, t) (a * inv_##t + b * t) //((Float)a + ((Float)b - (Float)a) * t)
//#define lerp(a,b, t) (inv_##t * (Float)a + b * t) //((Float)a + ((Float)b - (Float)a) * t)

void AL_DeformObject(chaowk* pChao) {
    AL_SHAPE* pShape = &pChao->Shape;
    Int i;
    
	for (i = 0; i < 40; i++) {
		int attr = AL_PartAttr[i];
		al_model* pModel = (*CurrObjectList)->pModel;

		if (pModel && (attr & PART_ATTR_MODEL)) {
            int j;
            Uint16 nbVertex = pModel->nbVertex;
			NJS_POINT3* pOrgVertexPos = pModel->pOrgVertexPos;
			CNK_VN_VERTEX* pVertex = pModel->pVertex;
			CNK_VN_VERTEX* pVertexZ = (CNK_VN_VERTEX*)((*ZeroObjectList)->model->vlist + 2);
			CNK_VN_VERTEX* pVertexN = (CNK_VN_VERTEX*)((*NormalObjectList)->model->vlist + 2);
			CNK_VN_VERTEX* pVertexH = (CNK_VN_VERTEX*)((*HorizonObjectList)->model->vlist + 2);
			CNK_VN_VERTEX* pVertexV = (CNK_VN_VERTEX*)((*VerticalObjectList)->model->vlist + 2);
			
			for (j = 0; j < nbVertex; j++)
			{
                NJS_POINT3* pPosN = &pVertexN->Pos;
                NJS_VECTOR* pNormalN = &pVertexN->Normal;
                float zhvX;
                float zhvY;
                float zhvZ;
                float zhvNX;
                float zhvNY;
                float zhvNZ;

                zhvX= div_ratio_h * lerp(pPosN->x, pVertexH->Pos.x, ratio_h)
                            + div_ratio_v * lerp(pPosN->x, pVertexV->Pos.x, ratio_v);
                zhvY = div_ratio_h * lerp(pPosN->y, pVertexH->Pos.y, ratio_h)
                            + div_ratio_v * lerp(pPosN->y, pVertexV->Pos.y, ratio_v);
                zhvZ = div_ratio_h * lerp(pPosN->z, pVertexH->Pos.z, ratio_h)
                            + div_ratio_v * lerp(pPosN->z, pVertexV->Pos.z, ratio_v);
                
                pOrgVertexPos->x = lerp(pVertexZ->Pos.x, zhvX, ratio_g);
                pOrgVertexPos->y = lerp(pVertexZ->Pos.y, zhvY, ratio_g);
                pOrgVertexPos->z = lerp(pVertexZ->Pos.z, zhvZ, ratio_g);

                zhvNX = div_ratio_h * lerp(pNormalN->x, pVertexH->Normal.x, ratio_h)
                            + div_ratio_v * lerp(pNormalN->x, pVertexV->Normal.x, ratio_v);
                zhvNY = div_ratio_h * lerp(pNormalN->y, pVertexH->Normal.y, ratio_h)
                            + div_ratio_v * lerp(pNormalN->y, pVertexV->Normal.y, ratio_v);
                zhvNZ = div_ratio_h * lerp(pNormalN->z, pVertexH->Normal.z, ratio_h)
                            + div_ratio_v * lerp(pNormalN->z, pVertexV->Normal.z, ratio_v);
                
                pVertex->Normal.x = lerp(pVertexZ->Normal.x, zhvNX, ratio_g);
                pVertex->Normal.y = lerp(pVertexZ->Normal.y, zhvNY, ratio_g);
                pVertex->Normal.z = lerp(pVertexZ->Normal.z, zhvNZ, ratio_g);

                pOrgVertexPos++;
                pVertex++;
                pVertexZ++;
                pVertexN++;
                pVertexH++;
                pVertexV++;    
			}

            {
                int colorIndex;
    			Uint8* pColor = (Uint8*)((*CurrObjectList)->pModel->PList);
    			Uint8* pColorZ = (Uint8*)((*ZeroObjectList)->model->plist);
    			Uint8* pColorN = (Uint8*)((*NormalObjectList)->model->plist);
    			Uint8* pColorH = (Uint8*)((*HorizonObjectList)->model->plist);
    			Uint8* pColorV = (Uint8*)((*VerticalObjectList)->model->plist);
    
                pColor += 4;
                pColorZ += 4;
                pColorN += 4;
                pColorH += 4;
                pColorV += 4;

                for(colorIndex = 0; colorIndex < 4; colorIndex++) {
                    //float colorN = *pColorN;
                    
                    *pColor = (*pColorN * col_inv_ratio_h + *pColorH * col_ratio_h) * col_div_ratio_h
    				+ (*pColorN * col_inv_ratio_v + *pColorV * col_ratio_v) * col_div_ratio_v;
    			    *pColor = (*pColorZ * col_inv_ratio_g + *pColor * col_ratio_g);

                    pColor ++;
                    pColorZ ++;
                    pColorN ++;
                    pColorH ++;
                    pColorV ++;
                }
            }
		}

		if (attr & PART_ATTR_NODE)
		{
            NJS_POINT3* pPos = (NJS_POINT3*)((*CurrObjectList)->Pos);
            NJS_POINT3* pPosZ = (NJS_POINT3*)((*ZeroObjectList)->pos);
            NJS_POINT3* pPosN = (NJS_POINT3*)((*NormalObjectList)->pos);
            NJS_POINT3* pPosH = (NJS_POINT3*)((*HorizonObjectList)->pos);
            NJS_POINT3* pPosV = (NJS_POINT3*)((*VerticalObjectList)->pos);

            float zhvPosX = div_ratio_h * lerp(pPosN->x, pPosH->x, ratio_h)
                            + div_ratio_v * lerp(pPosN->x, pPosV->x, ratio_v);
            float zhvPosY = div_ratio_h * lerp(pPosN->y, pPosH->y, ratio_h)
                            + div_ratio_v * lerp(pPosN->y, pPosV->y, ratio_v);
            float zhvPosZ = div_ratio_h * lerp(pPosN->z, pPosH->z, ratio_h)
                            + div_ratio_v * lerp(pPosN->z, pPosV->z, ratio_v);

            pPos->x = lerp(pPosZ->x, zhvPosX, ratio_g);
            pPos->y = lerp(pPosZ->y, zhvPosY, ratio_g);
            pPos->z = lerp(pPosZ->z, zhvPosZ, ratio_g);

            {
                Angle3* pAng = (Angle3*)((*CurrObjectList)->Ang);
                Angle3* pAngZ = (Angle3*)((*ZeroObjectList)->ang);
                Angle3* pAngN = (Angle3*)((*NormalObjectList)->ang);
                Angle3* pAngH = (Angle3*)((*HorizonObjectList)->ang);
                Angle3* pAngV = (Angle3*)((*VerticalObjectList)->ang);
                
                float zhvAngX = div_ratio_h * lerp(pAngN->x, pAngH->x, ratio_h)
                            + div_ratio_v * lerp(pAngN->x, pAngV->x, ratio_v);
                float zhvAngY = div_ratio_h * lerp(pAngN->y, pAngH->y, ratio_h)
                                + div_ratio_v * lerp(pAngN->y, pAngV->y, ratio_v);
                float zhvAngZ = div_ratio_h * lerp(pAngN->z, pAngH->z, ratio_h)
                                + div_ratio_v * lerp(pAngN->z, pAngV->z, ratio_v);

                pAng->x = lerp(pAngZ->x, zhvAngX, ratio_g);
                pAng->y = lerp(pAngZ->y, zhvAngY, ratio_g);
                pAng->z = lerp(pAngZ->z, zhvAngZ, ratio_g);
            }
		}

        CurrObjectList++;
        ZeroObjectList++;
        NormalObjectList++;
        HorizonObjectList++;
        VerticalObjectList++;        
	}
}
MERGE_LIST([['_col_inv_ratio_g', '_lbl_0C5093A0'], ['_CurrObjectList', '_lbl_0C5093A4'], ['_ZeroObjectList', '_lbl_0C5093A8'], ['_NormalObjectList', '_lbl_0C5093AC'], ['_HorizonObjectList', '_lbl_0C5093B0'], ['_VerticalObjectList', '_lbl_0C5093B4']]);

#if 0
enum {
    ATTR_HERO = 0x0,
    ATTR_DARK = 0x1,
};

extern int HDattr;

void AL_InterpolateAngle(Angle3* pAng1, Angle3* pAng2, float ratio, Angle3* pAnswer);
void AL_DeformObjectChild(chaowk* pChao) {
    AL_SHAPE* pShape = &pChao->Shape;
    Int i;
    CHAO_PARAM* pParam;
    float ratio_a;
    float inv_ratio_a;

    pParam = &pChao->param;
    ratio_a = pParam->body.APos;
    
    if(ratio_a >= 0) {
        HDattr = ATTR_HERO;
    }
    else {
        ratio_a *= -1;
        HDattr = ATTR_DARK;
    }
    
	for (i = 0; i < 40; i++) {
		int attr = AL_PartAttr[i];
		al_model* pModel = (*CurrObjectList)->pModel;

		if (pModel && (attr & PART_ATTR_MODEL)) {
            int j;
            Uint16 nbVertex = pModel->nbVertex;
			NJS_POINT3* pOrgVertexPos = pModel->pOrgVertexPos;
			NJS_POINT3* pOrgVertexNormal = pModel->pOrgVertexNormal;
			CNK_VN_VERTEX* pVertex = pModel->pVertex;
			CNK_VN_VERTEX* pVertexZ = (CNK_VN_VERTEX*)((*ZeroObjectList)->model->vlist + 2);
			CNK_VN_VERTEX* pVertexN = (CNK_VN_VERTEX*)((*NormalObjectList)->model->vlist + 2);
			CNK_VN_VERTEX* pVertexH = (CNK_VN_VERTEX*)((*HorizonObjectList)->model->vlist + 2);
			CNK_VN_VERTEX* pVertexV = (CNK_VN_VERTEX*)((*VerticalObjectList)->model->vlist + 2);
            CNK_VN_VERTEX* pVertexAZ;
            CNK_VN_VERTEX* pVertexAN;
            CNK_VN_VERTEX* pVertexAH;
            CNK_VN_VERTEX* pVertexAV;
            
            if(HDattr == ATTR_HERO) {
                pVertexAZ = (CNK_VN_VERTEX*)((*ZeroObjectListH)->model->vlist + 2);
			    pVertexAN = (CNK_VN_VERTEX*)((*NormalObjectListH)->model->vlist + 2);
			    pVertexAH = (CNK_VN_VERTEX*)((*HorizonObjectListH)->model->vlist + 2);
			    pVertexAV = (CNK_VN_VERTEX*)((*VerticalObjectListH)->model->vlist + 2);
            }
            else {
                pVertexAZ = (CNK_VN_VERTEX*)((*ZeroObjectListD)->model->vlist + 2);
			    pVertexAN = (CNK_VN_VERTEX*)((*NormalObjectListD)->model->vlist + 2);
			    pVertexAH = (CNK_VN_VERTEX*)((*HorizonObjectListD)->model->vlist + 2);
			    pVertexAV = (CNK_VN_VERTEX*)((*VerticalObjectListD)->model->vlist + 2);
            }

            if(ratio_a == 0.f) ratio_a = 0.0000001f;
            inv_ratio_a = 1 - ratio_a;
            
			for (j = 0; j < nbVertex; j++) {
                float zhvX;
                float zhvY;
                float zhvZ;
                float zhvaX;
                float zhvaY;
                float zhvaZ;
                float zhvNX;
                float zhvNY;
                float zhvNZ;
                float zhvaNX;
                float zhvaNY;
                float zhvaNZ;
                
                {
                NJS_POINT3* pPosN = &pVertexN->Pos;
                zhvX= div_ratio_h * lerp(pPosN->x, pVertexH->Pos.x, ratio_h)
                            + div_ratio_v * lerp(pPosN->x, pVertexV->Pos.x, ratio_v);
                zhvY = div_ratio_h * lerp(pPosN->y, pVertexH->Pos.y, ratio_h)
                            + div_ratio_v * lerp(pPosN->y, pVertexV->Pos.y, ratio_v);
                zhvZ = div_ratio_h * lerp(pPosN->z, pVertexH->Pos.z, ratio_h)
                            + div_ratio_v * lerp(pPosN->z, pVertexV->Pos.z, ratio_v);
                
                zhvX = lerp(pVertexZ->Pos.x, zhvX, ratio_g);
                zhvY = lerp(pVertexZ->Pos.y, zhvY, ratio_g);
                zhvZ = lerp(pVertexZ->Pos.z, zhvZ, ratio_g);

                pPosN = &pVertexAN->Pos;
                zhvaX = div_ratio_h * lerp(pPosN->x, pVertexAH->Pos.x, ratio_h)
                            + div_ratio_v * lerp(pPosN->x, pVertexAV->Pos.x, ratio_v);
                zhvaY = div_ratio_h * lerp(pPosN->y, pVertexAH->Pos.y, ratio_h)
                            + div_ratio_v * lerp(pPosN->y, pVertexAV->Pos.y, ratio_v);
                zhvaZ = div_ratio_h * lerp(pPosN->z, pVertexAH->Pos.z, ratio_h)
                            + div_ratio_v * lerp(pPosN->z, pVertexAV->Pos.z, ratio_v);
                
                zhvaX = lerp(pVertexAZ->Pos.x, zhvaX, ratio_g);
                zhvaY = lerp(pVertexAZ->Pos.y, zhvaY, ratio_g);
                zhvaZ = lerp(pVertexAZ->Pos.z, zhvaZ, ratio_g);

                pOrgVertexPos->x = lerp(zhvX, zhvaX, ratio_a);
                pOrgVertexPos->y = lerp(zhvY, zhvaY, ratio_a);
                pOrgVertexPos->z = lerp(zhvZ, zhvaZ, ratio_a);
                }
                {
                NJS_VECTOR* pNormalN = &pVertexN->Normal;
                zhvNX = div_ratio_h * lerp(pNormalN->x, pVertexH->Normal.x, ratio_h)
                            + div_ratio_v * lerp(pNormalN->x, pVertexV->Normal.x, ratio_v);
                zhvNY = div_ratio_h * lerp(pNormalN->y, pVertexH->Normal.y, ratio_h)
                            + div_ratio_v * lerp(pNormalN->y, pVertexV->Normal.y, ratio_v);
                zhvNZ = div_ratio_h * lerp(pNormalN->z, pVertexH->Normal.z, ratio_h)
                            + div_ratio_v * lerp(pNormalN->z, pVertexV->Normal.z, ratio_v);
                
                zhvNX = lerp(pVertexZ->Normal.x, zhvNX, ratio_g);
                zhvNY = lerp(pVertexZ->Normal.y, zhvNY, ratio_g);
                zhvNZ = lerp(pVertexZ->Normal.z, zhvNZ, ratio_g);

                pNormalN = &pVertexAN->Normal;
                zhvaNX = div_ratio_h * lerp(pNormalN->x, pVertexAH->Normal.x, ratio_h)
                            + div_ratio_v * lerp(pNormalN->x, pVertexAV->Normal.x, ratio_v);
                zhvaNY = div_ratio_h * lerp(pNormalN->y, pVertexAH->Normal.y, ratio_h)
                            + div_ratio_v * lerp(pNormalN->y, pVertexAV->Normal.y, ratio_v);
                zhvaNZ = div_ratio_h * lerp(pNormalN->z, pVertexAH->Normal.z, ratio_h)
                            + div_ratio_v * lerp(pNormalN->z, pVertexAV->Normal.z, ratio_v);
                
                zhvaNX = lerp(pVertexAZ->Normal.x, zhvaNX, ratio_g);
                zhvaNY = lerp(pVertexAZ->Normal.y, zhvaNY, ratio_g);
                zhvaNZ = lerp(pVertexAZ->Normal.z, zhvaNZ, ratio_g);
                
                pOrgVertexNormal->x = lerp(zhvNX, zhvaNX, ratio_a);
                pOrgVertexNormal->y = lerp(zhvNY, zhvaNY, ratio_a);
                pOrgVertexNormal->z = lerp(zhvNZ, zhvaNZ, ratio_a);
                }
                pOrgVertexPos++;
                pOrgVertexNormal++;
                pVertex++;
                pVertexZ++;
                pVertexN++;
                pVertexH++;
                pVertexV++;  
                pVertexAZ++;
                pVertexAN++;
                pVertexAH++;
                pVertexAV++;  
			}

            {
                int colorIndex;
    			Uint8* pColor = (Uint8*)((*CurrObjectList)->pModel->PList + 2);
    			Uint8* pColorZ = (Uint8*)((*ZeroObjectList)->model->plist + 2);
    			Uint8* pColorN = (Uint8*)((*NormalObjectList)->model->plist + 2);
    			Uint8* pColorH = (Uint8*)((*HorizonObjectList)->model->plist + 2);
    			Uint8* pColorV = (Uint8*)((*VerticalObjectList)->model->plist + 2);
                Uint8* pColorAZ;
                Uint8* pColorAN;
                Uint8* pColorAH;
                Uint8* pColorAV;
                
                if(HDattr == ATTR_HERO) {
    			    pColorAZ = (Uint8*)((*ZeroObjectListH)->model->plist + 2);
    			    pColorAN = (Uint8*)((*NormalObjectListH)->model->plist + 2);
    			    pColorAH = (Uint8*)((*HorizonObjectListH)->model->plist + 2);
    			    pColorAV = (Uint8*)((*VerticalObjectListH)->model->plist + 2);
                }
                else {
                    pColorAZ = (Uint8*)((*ZeroObjectListD)->model->plist + 2);
    			    pColorAN = (Uint8*)((*NormalObjectListD)->model->plist + 2) ;
    			    pColorAH = (Uint8*)((*HorizonObjectListD)->model->plist + 2);
    			    pColorAV = (Uint8*)((*VerticalObjectListD)->model->plist + 2);
                }
                
                for(colorIndex = 0; colorIndex < 4; colorIndex++) {
                    //float colorN = *pColorN;
                    float zhv;
                    float zhvA;
                    
                    zhv = col_div_ratio_h * (*pColorN * col_inv_ratio_h + *pColorH * col_ratio_h)
    				+ col_div_ratio_v * (*pColorN * col_inv_ratio_v + *pColorV * col_ratio_v);
                    zhv = (*pColorZ * col_inv_ratio_g + zhv * col_ratio_g);
                    
                    zhvA = col_div_ratio_h * (*pColorAN * col_inv_ratio_h + *pColorAH * col_ratio_h)
    				+ col_div_ratio_v * (*pColorAN * col_inv_ratio_v + *pColorAV * col_ratio_v);
                    zhvA = (*pColorAZ * col_inv_ratio_g + zhvA * col_ratio_g);
                    
    			    *pColor = lerp(zhv, zhvA, ratio_a);

                    pColor ++;
                    pColorZ ++;
                    pColorN ++;
                    pColorH ++;
                    pColorV ++;
                    pColorAZ ++;
                    pColorAN ++;
                    pColorAH ++;
                    pColorAV ++;
                }
            }
		}
        
		// if (attr & PART_ATTR_NODE)
		{
            NJS_POINT3* pPos = (NJS_POINT3*)((*CurrObjectList)->Pos);
            NJS_POINT3* pPosZ = (NJS_POINT3*)((*ZeroObjectList)->pos);
            NJS_POINT3* pPosN = (NJS_POINT3*)((*NormalObjectList)->pos);
            NJS_POINT3* pPosH = (NJS_POINT3*)((*HorizonObjectList)->pos);
            NJS_POINT3* pPosV = (NJS_POINT3*)((*VerticalObjectList)->pos);
            NJS_POINT3* pPosAZ;
            NJS_POINT3* pPosAN;
            NJS_POINT3* pPosAH;
            NJS_POINT3* pPosAV;
            float zhvPosX;
            float zhvPosY;
            float zhvPosZ;
            float zhvaPosX;
            float zhvaPosY;
            float zhvaPosZ;
            
            if(HDattr == ATTR_HERO) {
                pPosAZ = (NJS_POINT3*)((*ZeroObjectListH)->pos);
                pPosAN = (NJS_POINT3*)((*NormalObjectListH)->pos);
                pPosAH = (NJS_POINT3*)((*HorizonObjectListH)->pos);
                pPosAV = (NJS_POINT3*)((*VerticalObjectListH)->pos);
            }
            else {
                pPosAZ = (NJS_POINT3*)((*ZeroObjectListD)->pos);
                pPosAN = (NJS_POINT3*)((*NormalObjectListD)->pos);
                pPosAH = (NJS_POINT3*)((*HorizonObjectListD)->pos);
                pPosAV = (NJS_POINT3*)((*VerticalObjectListD)->pos);
            }
            #define posLerp(a,b,t) (a * inv_##t + b * t)

            zhvPosX = div_ratio_h * posLerp(pPosN->x, pPosH->x, ratio_h)
                            + div_ratio_v * posLerp(pPosN->x, pPosV->x, ratio_v);
            zhvPosY = div_ratio_h * posLerp(pPosN->y, pPosH->y, ratio_h)
                            + div_ratio_v * posLerp(pPosN->y, pPosV->y, ratio_v);
            zhvPosZ = div_ratio_h * posLerp(pPosN->z, pPosH->z, ratio_h)
                            + div_ratio_v * posLerp(pPosN->z, pPosV->z, ratio_v);

            zhvPosX = lerp(pPosZ->x, zhvPosX, ratio_g);
            zhvPosY = lerp(pPosZ->y, zhvPosY, ratio_g);
            zhvPosZ = lerp(pPosZ->z, zhvPosZ, ratio_g);

            zhvaPosX = div_ratio_h * posLerp(pPosAN->x, pPosAH->x, ratio_h)
                            + div_ratio_v * posLerp(pPosAN->x, pPosAV->x, ratio_v);
            zhvaPosY = div_ratio_h * posLerp(pPosAN->y, pPosAH->y, ratio_h)
                            + div_ratio_v * posLerp(pPosAN->y, pPosAV->y, ratio_v);
            zhvaPosZ = div_ratio_h * posLerp(pPosAN->z, pPosAH->z, ratio_h)
                            + div_ratio_v * posLerp(pPosAN->z, pPosAV->z, ratio_v);

            zhvaPosX = lerp(pPosAZ->x, zhvaPosX, ratio_g);
            zhvaPosY = lerp(pPosAZ->y, zhvaPosY, ratio_g);
            zhvaPosZ = lerp(pPosAZ->z, zhvaPosZ, ratio_g);

            pPos->x = lerp(zhvPosX, zhvaPosX, ratio_a);
            pPos->y = lerp(zhvPosY, zhvaPosY, ratio_a);
            pPos->z = lerp(zhvPosZ, zhvaPosZ, ratio_a);            
		}
        
        {
            Angle3* pAng = (Angle3*)((*CurrObjectList)->Ang);
            Angle3* pAngZ = (Angle3*)((*ZeroObjectList)->ang);
            Angle3* pAngN = (Angle3*)((*NormalObjectList)->ang);
            Angle3* pAngH = (Angle3*)((*HorizonObjectList)->ang);
            Angle3* pAngV = (Angle3*)((*VerticalObjectList)->ang);
            Angle3* pAngAZ;
            Angle3* pAngAN;
            Angle3* pAngAH;
            Angle3* pAngAV;

            // names from sa2 debug build stack checks
            Angle3 Answer0;
            Angle3 Answer1;
            Angle3 IntAng;
            Angle3 IntAngA;

            if(HDattr == ATTR_HERO) {
                pAngAZ = (Angle3*)((*ZeroObjectListH)->ang);
                pAngAN = (Angle3*)((*NormalObjectListH)->ang);
                pAngAH = (Angle3*)((*HorizonObjectListH)->ang);
                pAngAV = (Angle3*)((*VerticalObjectListH)->ang);
            }
            else {
                pAngAZ = (Angle3*)((*ZeroObjectListD)->ang);
                pAngAN = (Angle3*)((*NormalObjectListD)->ang);
                pAngAH = (Angle3*)((*HorizonObjectListD)->ang);
                pAngAV = (Angle3*)((*VerticalObjectListD)->ang);
            }
            
            AL_InterpolateAngle(pAngN, pAngH, ratio_h, &Answer0);
            AL_InterpolateAngle(pAngN, pAngV, ratio_v, &Answer1);

            IntAng.x = div_ratio_h * Answer0.x + div_ratio_v * Answer1.x;
            IntAng.y = div_ratio_h * Answer0.y + div_ratio_v * Answer1.y;
            IntAng.z = div_ratio_h * Answer0.z + div_ratio_v * Answer1.z;

            AL_InterpolateAngle(pAngZ, &IntAng, ratio_g, &IntAng);
            
            AL_InterpolateAngle(pAngAN, pAngAH, ratio_h, &Answer0);
            AL_InterpolateAngle(pAngAN, pAngAV, ratio_v, &Answer1);

            IntAngA.x = div_ratio_h * Answer0.x + div_ratio_v * Answer1.x;
            IntAngA.y = div_ratio_h * Answer0.y + div_ratio_v * Answer1.y;
            IntAngA.z = div_ratio_h * Answer0.z + div_ratio_v * Answer1.z;

            AL_InterpolateAngle(pAngAZ, &IntAngA, ratio_g, &IntAngA);

            AL_InterpolateAngle(&IntAng, &IntAngA, ratio_a, pAng);
        }

        CurrObjectList++;
        
        ZeroObjectList++;
        NormalObjectList++;
        HorizonObjectList++;
        VerticalObjectList++;        
        
        ZeroObjectListH++;
        NormalObjectListH++;
        HorizonObjectListH++;
        VerticalObjectListH++;     
        
        ZeroObjectListD++;
        NormalObjectListD++;
        HorizonObjectListD++;
        VerticalObjectListD++;       
	}
}
#else
//MERGE_LIST([['_ZeroObjectListD', '_lbl_0C50A320'], ['_NormalObjectListD', '_lbl_0C50A324'], ['_HorizonObjectListD', '_lbl_0C50A328'], ['_VerticalObjectListD', '_lbl_0C50A32C'], ['_AL_InterpolateAngle', '_lbl_0C50A330'], ['_CurrObjectList', '_lbl_0C50A334'], ['_ZeroObjectList', '_lbl_0C50A338'], ['_NormalObjectList', '_lbl_0C50A33C'], ['_HorizonObjectList', '_lbl_0C50A340'], ['_VerticalObjectList', '_lbl_0C50A344'], ['_ZeroObjectListH', '_lbl_0C50A348'], ['_NormalObjectListH', '_lbl_0C50A34C'], ['_HorizonObjectListH', '_lbl_0C50A350'], ['_VerticalObjectListH', '_lbl_0C50A354']]);
INLINE_ASM(_AL_DeformObjectChild, 0xeec, "asm/nonmatching/Chao/al_deform/_AL_DeformObjectChild.src");
#endif
INLINE_ASM(_AL_Deform, 0x3a6, "asm/nonmatching/Chao/al_deform/_AL_Deform.src");

// MERGE_LIST([['_lbl_0C50CF60', '_lbl_0C50A680'], ['_AL_DeformObjectChild', '_lbl_0C50A684'], ['_AL_DeformObject', '_lbl_0C50A688'], ['_col_div_ratio_h', '_lbl_0C50A678'], ['_lbl_0C510C3E', '_lbl_0C50A68C'], ["h'0000FFFD", '_lbl_0C50A690'], ['_lbl_0C522DC8', '_lbl_0C50A67C'], ['_col_ratio_v', '_lbl_0C50A660'], ['_col_ratio_g', '_lbl_0C50A664'], ['_col_inv_ratio_h', '_lbl_0C50A668'], ['_col_inv_ratio_v', '_lbl_0C50A66C'], ['_col_inv_ratio_g', '_lbl_0C50A670'], ['_col_div_ratio_v', '_lbl_0C50A674']]);
INLINE_ASM(_func_0C50A5EA, 0xbe, "asm/nonmatching/Chao/al_deform/_func_0C50A5EA.src");

INLINE_ASM(_func_0C50A6A8, 0x9c, "asm/nonmatching/Chao/al_deform/_func_0C50A6A8.src");

// MERGE_LIST([['__modls', '_lbl_0C50A7C0']]);
