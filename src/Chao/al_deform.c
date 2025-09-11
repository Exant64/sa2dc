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

//INLINE_ASM(_AL_DeformObject, 0x618, "asm/nonmatching/Chao/al_deform/_AL_DeformObject.src");

MERGE_LIST([['_col_inv_ratio_g', '_lbl_0C5093A0'], ['_CurrObjectList', '_lbl_0C5093A4'], ['_ZeroObjectList', '_lbl_0C5093A8'], ['_NormalObjectList', '_lbl_0C5093AC'], ['_HorizonObjectList', '_lbl_0C5093B0'], ['_VerticalObjectList', '_lbl_0C5093B4']]);
INLINE_ASM(_func_0C509358, 0xeec, "asm/nonmatching/Chao/al_deform/_func_0C509358.src");

// MERGE_LIST([['_lbl_0C660C84', '_lbl_0C50A320'], ['_lbl_0C660C88', '_lbl_0C50A324'], ['_lbl_0C660C8C', '_lbl_0C50A328'], ['_lbl_0C660C90', '_lbl_0C50A32C'], ['_lbl_0C511700', '_lbl_0C50A330'], ['_CurrObjectList', '_lbl_0C50A334'], ['_ZeroObjectList', '_lbl_0C50A338'], ['_NormalObjectList', '_lbl_0C50A33C'], ['_HorizonObjectList', '_lbl_0C50A340'], ['_VerticalObjectList', '_lbl_0C50A344'], ['_lbl_0C660C74', '_lbl_0C50A348'], ['_lbl_0C660C78', '_lbl_0C50A34C'], ['_lbl_0C660C7C', '_lbl_0C50A350'], ['_lbl_0C660C80', '_lbl_0C50A354']]);
INLINE_ASM(_AL_Deform, 0x3a6, "asm/nonmatching/Chao/al_deform/_AL_Deform.src");

// MERGE_LIST([['_lbl_0C50CF60', '_lbl_0C50A680'], ['_lbl_0C509358', '_lbl_0C50A684'], ['_AL_DeformObject', '_lbl_0C50A688'], ['_col_div_ratio_h', '_lbl_0C50A678'], ['_lbl_0C510C3E', '_lbl_0C50A68C'], ["h'0000FFFD", '_lbl_0C50A690'], ['_lbl_0C522DC8', '_lbl_0C50A67C'], ['_col_ratio_v', '_lbl_0C50A660'], ['_col_ratio_g', '_lbl_0C50A664'], ['_col_inv_ratio_h', '_lbl_0C50A668'], ['_col_inv_ratio_v', '_lbl_0C50A66C'], ['_col_inv_ratio_g', '_lbl_0C50A670'], ['_col_div_ratio_v', '_lbl_0C50A674']]);
INLINE_ASM(_func_0C50A5EA, 0xbe, "asm/nonmatching/Chao/al_deform/_func_0C50A5EA.src");

INLINE_ASM(_func_0C50A6A8, 0x9c, "asm/nonmatching/Chao/al_deform/_func_0C50A6A8.src");

// MERGE_LIST([['__modls', '_lbl_0C50A7C0']]);
