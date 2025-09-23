#include <task.h>
#include <Chao/Chao.h>
#include <sg_maloc.h>

enum {
    PVP_NCZ,
    PVP_NCN,
    PVP_HCZ,
    PVP_HCN,
    PVP_HCS,
    PVP_HCF,
    PVP_HCR,
    PVP_HCP,
    PVP_DCZ,
    PVP_HNZ,
    PVP_HNN,
    PVP_HNS,
    PVP_HNF,
    PVP_HNR,
    PVP_HNP,
    PVP_HSZ,
    PVP_HSN,
    PVP_HSS,
    PVP_HSF,
    PVP_HSR,
    PVP_HSP,
    PVP_HFZ,
    PVP_HFN,
    PVP_HFS,
    PVP_HFF,
    PVP_HFR,
    PVP_HFP,
    PVP_HRZ,
    PVP_HRN,
    PVP_HRS,
    PVP_HRF,
    PVP_HRR,
    PVP_HRP,
    PVP_HPZ,
    PVP_HPN,
    PVP_HPS,
    PVP_HPF,
    PVP_HPR,
    PVP_HPP,
};

extern const char* gPaletteFilename[];
extern char gPalette[39][48 * 4];

void lbl_0C522B80(const char* pStr) {
    njPrintColor(0xFFFF0000);

    do {
        njPrintC(0x30003, pStr);
        njWaitVSync();
    } while (1);
}

static Uint32 Decode5555_1(Uint32 a1) {
    return ((a1 & 1) != 0) ? 0 : 255;
}

static Uint32 Decode5555_5(Uint32 a1) {
    return ((a1 & 0x1F) << 3) | ((a1 & 0x1F) >> 2);
}

static Uint32 Decode565_6(Uint32 a1) {
    return (4 * (a1 & 0x3F)) | ((a1 & 0x3F) >> 4);
}

static Uint32 Decode4444(Uint32 a1) {
    return (16 * (a1 & 0xF)) | a1 & 0xF;
}

extern const char lbl_0C566B70[]; // "ILLEGAL COLOR MODE"
void alpalLoadTexpalette() {
    NJS_TEXPALETTE texpalette;
    int i;

    texpalette.palette = (void*)syMalloc(0x2800);

    for (i = 0; i < 39; i++) {
        njLoadPaletteFile(gPaletteFilename[i], &texpalette);

        switch (texpalette.mode) {
            case NJD_TEXFMT_ARGB_1555: {
                int j;
                Uint16* pSrc = texpalette.palette;
                Uint8* pDst = gPalette[i];

                for (j = 47; j >= 0; j--) {
                    Uint32 color = *(pSrc++);

                    *(pDst++) = Decode5555_5((Uint32)color);
                    *(pDst++) = Decode5555_5((Uint32)color >> 5);
                    *(pDst++) = Decode5555_5((Uint32)color >> 10);
                    *(pDst++) = Decode5555_1((Uint32)color >> 15);
                }

                break;
            }

            case NJD_TEXFMT_RGB_565: {
                int j;
                Uint16* pSrc = texpalette.palette;
                Uint8* pDst = gPalette[i];

                for (j = 47; j >= 0; j--) {
                    Uint32 color;
                    color = *(pSrc++);
                    *(pDst++) = Decode5555_5(color);
                    *(pDst++) = Decode565_6(color >> 5);
                    *(pDst++) = Decode5555_5(color >> 11);
                    *(pDst++) = 0xFF;
                }

                break;
            }

            case NJD_TEXFMT_ARGB_4444: {
                int j;
                Uint16* pSrc = texpalette.palette;
                Uint8* pDst = gPalette[i];

                for (j = 47; j >= 0; j--) {
                    Uint32 color;
                    color = *(pSrc++);
                    *(pDst++) = Decode4444((Uint32)color);
                    *(pDst++) = Decode4444((Uint32)color >> 4);
                    *(pDst++) = Decode4444((Uint32)color >> 8);
                    *(pDst++) = Decode4444((Uint32)color >> 12);
                }

                break;
            }

            case NJD_TEXFMT_ARGB_8888: {
                memcpy(gPalette[i], texpalette.palette, 48);
                break;
            }

            default:
                lbl_0C522B80(lbl_0C566B70); // "ILLEGAL COLOR MODE"
        }
    }

    syFree(texpalette.palette);
    njSetPaletteMode(NJD_TEXFMT_ARGB_8888);
}

#define lerp(a, b, t) (a * inv_##t + b * t)

void AL_PaletteSetColorRatio(CHAO_PARAM* pParam, Sint32 cno) {
    Sint32 no;

    switch (pParam->type) {
        case TYPE_CHILD:
            no = PVP_NCZ;
            break;
        case TYPE_H_NORMAL:
            no = PVP_HNZ;
            break;
        case TYPE_H_SWIM:
            no = PVP_HSZ;
            break;
        case TYPE_H_FLY:
            no = PVP_HFZ;
            break;
        case TYPE_H_RUN:
            no = PVP_HRZ;
            break;
        case TYPE_H_POWER:
            no = PVP_HPZ;
            break;
        default:
            return;
    }

    if (pParam->type == TYPE_CHILD) {
        void* p = syMalloc(48 * 4);

        Uint8* pPaletteNC0 = (Uint8*)gPalette[PVP_NCZ];
        Uint8* pPaletteNC1 = (Uint8*)gPalette[PVP_NCN];
        Uint8* pPaletteHCZ = (Uint8*)gPalette[PVP_HCZ];
        Uint8* pPaletteHCN = (Uint8*)gPalette[PVP_HCN];
        Uint8* pPaletteH = (Uint8*)gPalette[PVP_HCP];
        Uint8* pPaletteV = (Uint8*)gPalette[PVP_HCF];
        Uint8* pPaletteDC = (Uint8*)gPalette[PVP_DCZ];
        Uint8* pColor = p;

        Float ratio_h = pParam->body.HPos;
        float inv_ratio_h;
        float div_ratio_h;
        Float ratio_v = pParam->body.VPos;
        float inv_ratio_v;
        float div_ratio_v;
        Float ratio_g = pParam->body.growth;
        float inv_ratio_g;
        Float ratio_a = pParam->body.APos;
        float inv_ratio_a;

        if (ratio_g > 1.0f)
            ratio_g = 1.0f;
        if (ratio_h == 0.0f)
            ratio_h = 0.000001f;
        if (ratio_v == 0.0f)
            ratio_v = 0.000001f;

        if (ratio_h > 1.0f) {
            ratio_h = 1.0f;
        } else if (ratio_h < -1.0f) {
            ratio_h = -1.0f;
        }

        if (ratio_h < 0.0f) {
            pPaletteH = (Uint8*)gPalette[PVP_HCR];
            ratio_h = -ratio_h;
        }

        if (ratio_v > 1.0f) {
            ratio_v = 1.0f;
        } else if (ratio_v < -1.0f) {
            ratio_v = -1.0f;
        }

        if (ratio_v < 0.0f) {
            pPaletteV = (Uint8*)gPalette[PVP_HCS];
            ratio_v = -ratio_v;
        }

        inv_ratio_h = 1.0f - ratio_h;
        inv_ratio_v = 1.0f - ratio_v;
        inv_ratio_g = 1.0f - ratio_g;

        div_ratio_h = div_ratio_v = 1.0f / (ratio_v + ratio_h);
        div_ratio_h *= ratio_h;
        div_ratio_v *= ratio_v;

        if (ratio_a >= 0.0f) {
            int i;

            if (ratio_a > 1.0f)
                ratio_a = 1.0f;

            inv_ratio_a = 1 - ratio_a;

            for (i = 192 - 1; i >= 0; i--) {
                float zhv;
                float zhvA;

                zhvA = (*pPaletteHCN * inv_ratio_h + *pPaletteH * ratio_h) * div_ratio_h +
                       (*pPaletteHCN * inv_ratio_v + *pPaletteV * ratio_v) * div_ratio_v;
                zhvA = (*pPaletteHCZ * inv_ratio_g + zhvA * ratio_g);

                zhv = (*pPaletteNC0 * inv_ratio_g + *pPaletteNC1 * ratio_g);

                *pColor = lerp(zhv, zhvA, ratio_a);

                pColor++;
                ++pPaletteHCZ;
                ++pPaletteHCN;
                ++pPaletteH;
                ++pPaletteV;
                ++pPaletteNC0;
                ++pPaletteNC1;
                ++pPaletteDC;
            }
        } else {
            int i;
            if (ratio_a < -1.0f)
                ratio_a = -1.0f;

            ratio_a *= -1.f;

            inv_ratio_a = 1 - ratio_a;

            for (i = 192 - 1; i >= 0; i--) {
                float zhv;
                float zhvA = *pPaletteDC;

                zhv = (*pPaletteNC0 * inv_ratio_g + *pPaletteNC1 * ratio_g);
                *pColor = lerp(zhv, zhvA, ratio_a);

                ++pColor;
                ++pPaletteNC0;
                ++pPaletteNC1;
                ++pPaletteDC;
            }
        }

        njSetPaletteData(48 * cno, 48, p);
        syFree(p);
    } else {
        void* p = syMalloc(4 * 48);

        Uint8* pPaletteZ = (Uint8*)gPalette[no + 0];
        Uint8* pPaletteN = (Uint8*)gPalette[no + 1];
        Uint8* pPaletteH = (Uint8*)gPalette[no + 5]; // HPos
        Uint8* pPaletteV = (Uint8*)gPalette[no + 3]; // VPos
        Uint8* pColor = p;

        Float ratio_h = pParam->body.HPos;
        float inv_ratio_h;
        float div_ratio_h;
        Float ratio_v = pParam->body.VPos;
        float inv_ratio_v;
        float div_ratio_v;
        Float ratio_g = pParam->body.growth;
        float inv_ratio_g;

        int i;

        if (ratio_g > 1.0f)
            ratio_g = 1.0f;
        if (ratio_h == 0.0f)
            ratio_h = 0.000001f;
        if (ratio_v == 0.0f)
            ratio_v = 0.000001f;

        if (ratio_h > 1.0f) {
            ratio_h = 1.0f;
        } else if (ratio_h < -1.0f) {
            ratio_h = -1.0f;
        }

        if (ratio_h < 0.0f) {
            pPaletteH = (Uint8*)gPalette[no + 4];
            ratio_h = -ratio_h;
        }

        if (ratio_v > 1.0f) {
            ratio_v = 1.0f;
        } else if (ratio_v < -1.0f) {
            ratio_v = -1.0f;
        }

        if (ratio_v < 0.0f) {
            pPaletteV = (Uint8*)gPalette[no + 2];
            ratio_v = -ratio_v;
        }

        inv_ratio_h = 1.0f - ratio_h;
        inv_ratio_v = 1.0f - ratio_v;
        inv_ratio_g = 1.0f - ratio_g;

        div_ratio_h = div_ratio_v = 1.0f / (ratio_v + ratio_h);
        div_ratio_h *= ratio_h;
        div_ratio_v *= ratio_v;

        for (i = 192 - 1; i >= 0; i--) {
            float zhv;

            zhv = (*pPaletteN * inv_ratio_h + *pPaletteH * ratio_h) * div_ratio_h +
                  (*pPaletteN * inv_ratio_v + *pPaletteV * ratio_v) * div_ratio_v;

            *pColor = lerp(*pPaletteZ, zhv, ratio_g);

            ++pColor;
            ++pPaletteZ;
            ++pPaletteN;
            ++pPaletteH;
            ++pPaletteV;
        }

        njSetPaletteData(48 * cno, 48, p);
        syFree(p);
    }
}

INLINE_ASM(_func_0C523400, 0xb8, "asm/nonmatching/Chao/al_palette/_func_0C523400.src");

// MERGE_LIST([['_syMalloc', '_lbl_0C5235E0'], ['_syFree', '_lbl_0C5235EC'], ['_gPalette', '_lbl_0C5235E4'],
// ["h'_njSetPaletteData", '_lbl_0C5235E8']]);
INLINE_ASM(_func_0C5234B8, 0x148, "asm/nonmatching/Chao/al_palette/_func_0C5234B8.src");

INLINE_ASM(_alpalSetBank, 0xc0, "asm/nonmatching/Chao/al_palette/_alpalSetBank.src");

INLINE_ASM(_func_0C5236C0, 0x120, "asm/nonmatching/Chao/al_palette/_func_0C5236C0.src");
