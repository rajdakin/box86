#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <dlfcn.h>

#include "wrappedlibs.h"

#include "debug.h"
#include "wrapper.h"
#include "bridge.h"
#include "library_private.h"
#include "x86emu.h"
#include "x86emu_private.h"
#include "box86context.h"
#include "sdl2rwops.h"
#include "callback.h"
#include "khash.h"

KHASH_MAP_INIT_INT(effectcb, x86emu_t*)

typedef void* (*pFpi_t)(void*, int32_t);
typedef void* (*pFpii_t)(void*, int32_t, int32_t);
typedef void  (*vFpp_t)(void*, void*);
typedef void  (*vFp_t)(void*);
typedef int32_t (*iFippp_t)(int32_t, void*, void*, void*);

typedef struct sdl2mixer_my_s {
    pFpii_t     Mix_LoadMUSType_RW;
    pFpi_t      Mix_LoadMUS_RW;
    pFpi_t      Mix_LoadWAV_RW;
    vFpp_t      Mix_SetPostMix;
    iFippp_t    Mix_RegisterEffect;
    vFp_t       Mix_ChannelFinished;
    vFpp_t      Mix_HookMusic;

    x86emu_t* PostCallback;
    x86emu_t* hookMusicCB;
    // timer map
    kh_effectcb_t    *effectcb;
} sdl2mixer_my_t;

static void* getSDL2MixerMy(library_t* lib)
{
    sdl2mixer_my_t* my = (sdl2mixer_my_t*)calloc(1, sizeof(sdl2mixer_my_t));
    #define GO(A, W) my->A = (W)dlsym(lib->priv.w.lib, #A);
    GO(Mix_LoadMUSType_RW,pFpii_t)
    GO(Mix_LoadMUS_RW,pFpi_t)
    GO(Mix_LoadWAV_RW,pFpi_t)
    GO(Mix_SetPostMix,vFpp_t)
    GO(Mix_RegisterEffect, iFippp_t)
    GO(Mix_ChannelFinished,vFp_t)
    GO(Mix_HookMusic, vFpp_t)
    #undef GO
    my->effectcb = kh_init(effectcb);
    return my;
}

static void freeSDL2MixerMy(library_t* lib)
{
    sdl2mixer_my_t *my = lib->priv.w.p2;
    if(my->PostCallback)
        FreeCallback(my->PostCallback);
    if(my->hookMusicCB)
        FreeCallback(my->hookMusicCB);
}

void EffectFuncCallback(int chan, void *stream, int len, void *udata)
{
    x86emu_t *emu = (x86emu_t*) udata;
    SetCallbackArg(emu, 0, (void*)chan);
    SetCallbackArg(emu, 1, stream);
    SetCallbackArg(emu, 2, (void*)len);
    RunCallback(emu);
}
void EffectDoneCallback(int chan, void *udata)
{
    x86emu_t *emu = (x86emu_t*) udata;
    SetCallbackArg(emu, 0, (void*)chan);
    RunCallback(emu);
}

EXPORT void* my2_Mix_LoadMUSType_RW(x86emu_t* emu, void* a, int32_t b, int32_t c)
{
    sdl2mixer_my_t *my = (sdl2mixer_my_t *)emu->context->sdl2mixerlib->priv.w.p2;
    SDL2RWSave_t save;
    RWNativeStart2(emu, (SDL2_RWops_t*)a, &save);
    void* r = my->Mix_LoadMUSType_RW(a, b, c);
    if(c==0)
        RWNativeEnd2(emu, (SDL2_RWops_t*)a, &save);
    return r;
}
EXPORT void* my2_Mix_LoadMUS_RW(x86emu_t* emu, void* a, int32_t f)
{
    sdl2mixer_my_t *my = (sdl2mixer_my_t *)emu->context->sdl2mixerlib->priv.w.p2;
    SDL2RWSave_t save;
    RWNativeStart2(emu, (SDL2_RWops_t*)a, &save);
    void* r = my->Mix_LoadMUS_RW(a, f);
    if(f==0)
        RWNativeEnd2(emu, (SDL2_RWops_t*)a, &save);  // this one never free the RWops
    return r;
}
EXPORT void* my2_Mix_LoadWAV_RW(x86emu_t* emu, void* a, int32_t f)
{
    sdl2mixer_my_t *my = (sdl2mixer_my_t *)emu->context->sdl2mixerlib->priv.w.p2;
    SDL2RWSave_t save;
    RWNativeStart2(emu, (SDL2_RWops_t*)a, &save);
    void* r = my->Mix_LoadWAV_RW(a, f);
    if(f==0)
        RWNativeEnd2(emu, (SDL2_RWops_t*)a, &save);
    return r;
}

static void sdl2mixerPostCallback(void *userdata, uint8_t *stream, int32_t len)
{
    x86emu_t *emu = (x86emu_t*) userdata;
    SetCallbackArg(emu, 1, stream);
    SetCallbackArg(emu, 2, (void*)len);
    RunCallback(emu);
}

EXPORT void my2_Mix_SetPostMix(x86emu_t* emu, void* a, void* b)
{
    sdl2mixer_my_t *my = (sdl2mixer_my_t *)emu->context->sdl2mixerlib->priv.w.p2;
    // create a callback
    if(a) {
        x86emu_t *cbemu = AddCallback(emu, (uintptr_t)a, 3, b, NULL, NULL, NULL);
        my->Mix_SetPostMix(sdl2mixerPostCallback, cbemu);
        if(my->PostCallback)
            FreeCallback(my->PostCallback);
        my->PostCallback = cbemu;
    } else {
        my->Mix_SetPostMix(NULL, b);
        FreeCallback(my->PostCallback);
        my->PostCallback = NULL;
    }
}

EXPORT int32_t my2_Mix_RegisterEffect(x86emu_t*emu, int32_t channel, void* cb_effect, void* cb_done, void* arg)
{
    sdl2mixer_my_t *my = (sdl2mixer_my_t *)emu->context->sdl2mixerlib->priv.w.p2;
    x86emu_t *effect = NULL;
    khint_t k;
    void* cb1 = NULL;
    void* cb2 = NULL;
printf("Warning: Mix_RegisterEffect(%d, %p, %p, %p) Ignored\n", channel, cb_effect, cb_done, arg);
    #if 0
    // how to register 2 functions with the same pointer
    if(cb_effect) {
        k = kh_get(effectcb, my->effectcb, (uintptr_t)effect);
        if(k==kh_end(my->effectcb)) {
            effect = AddCallback(emu, (uintptr_t)cb_effect, 4, NULL, NULL, NULL, arg);
            int ret;
            k = kh_put(effectcb, my->effectcb, (uintptr_t)cb_effect, &ret);
            kh_value(my->effectcb, k) = effect;
        } else {
            effect = kh_value(my->effectcb, k);
        }
        cb1 = EffectFuncCallback;
    }
    if(cb_done) {
        k = kh_get(effectcb, my->effectcb, (uintptr_t)effect);
        if(k==kh_end(my->effectcb)) {
            effect = AddCallback(emu, (uintptr_t)cb_done, 2, NULL, arg, NULL, NULL);
            int ret;
            k = kh_put(effectcb, my->effectcb, (uintptr_t)cb_done, &ret);
            kh_value(my->effectcb, k) = effect;
        } else {
            effect = kh_value(my->effectcb, k);
        }
        cb2 = EffectDoneCallback;
    }
    return my->Mix_RegisterEffect(channel, cb1, cb2, )
    #else
    return 0;
    #endif
}

x86emu_t* sdl2channelfinished_emu = NULL;
static void sdl2ChannelFinishedCallback(int channel)
{
    if(!sdl2channelfinished_emu)
        return;
    SetCallbackArg(sdl2channelfinished_emu, 0, (void*)channel);
    RunCallback(sdl2channelfinished_emu);
}
void EXPORT my2_Mix_ChannelFinished(x86emu_t* emu, void* cb)
{
    sdl2mixer_my_t *my = (sdl2mixer_my_t *)emu->context->sdl2mixerlib->priv.w.p2;
    if(sdl2channelfinished_emu) {
        FreeCallback(sdl2channelfinished_emu);
        sdl2channelfinished_emu = NULL;
    }
    if(cb) {
        sdl2channelfinished_emu = AddCallback(emu, (uintptr_t)cb, 1, NULL, NULL, NULL, NULL);
        my->Mix_ChannelFinished(sdl2ChannelFinishedCallback);
    } else
        my->Mix_ChannelFinished(NULL);
}
static void sdl2mixer_hookMusicCallback(void* udata, uint8_t* stream, int32_t len)
{
    x86emu_t *emu = (x86emu_t*)udata;
    SetCallbackArg(emu, 1, (void*)stream);
    SetCallbackArg(emu, 2, (void*)len);
    RunCallback(emu);
}

EXPORT void my2_Mix_HookMusic(x86emu_t* emu, void* f, void* arg)
{
    sdl2mixer_my_t *my = (sdl2mixer_my_t *)emu->context->sdl2mixerlib->priv.w.p2;
    if(my->hookMusicCB) {
        my->Mix_HookMusic(NULL, NULL);
        FreeCallback(my->hookMusicCB);
        my->hookMusicCB = NULL;
    }
    if(!f)
        return;
    x86emu_t *cb = NULL;
    cb =  AddCallback(emu, (uintptr_t)f, 3, arg, NULL, NULL, NULL);
    my->hookMusicCB = cb;
    my->Mix_HookMusic(sdl2mixer_hookMusicCallback, cb);
}


const char* sdl2mixerName = "libSDL2_mixer-2.0.so.0";
#define LIBNAME sdl2mixer

#define CUSTOM_INIT \
    box86->sdl2mixerlib = lib; \
    lib->priv.w.p2 = getSDL2MixerMy(lib); \
    lib->altmy = strdup("my2_");

#define CUSTOM_FINI \
    freeSDL2MixerMy(lib); \
    free(lib->priv.w.p2); \
    ((box86context_t*)(lib->context))->sdl2mixerlib = NULL;

#include "wrappedlib_init.h"

