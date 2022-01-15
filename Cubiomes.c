#include "Cubiomes.h"

#include <assert.h>
#include <stdio.h>

#include "cubiomes/generator.h"
#include "cubiomes/util.h"
#include "dev_corr_Cubiomes.h"

typedef struct cubiomes_context
{
	uint64_t seed;
	jint version;
	jint flags;
	Generator g;
	char errstr[256];
} cctx;

static struct
{
	jfieldID cubiomes_lp;
} ids = {};

unsigned char biomeColors[256][3] = {0};

void Java_dev_corr_Cubiomes_initLibrary(JNIEnv *env, jclass class)
{
	ids.cubiomes_lp = (*env)->GetFieldID(env, class, "cubiomes_lp", "J");
	initBiomeColors(biomeColors);
}

void Java_dev_corr_Cubiomes_initGenerator(JNIEnv *env,
                                          jobject this,
                                          jlong seed,
                                          jint version,
                                          jint flags,
                                          int dimension)
{
	cctx *ctx    = calloc(1, sizeof(*ctx));
	ctx->seed    = seed;
	ctx->version = version;
	ctx->flags   = flags;

	setupGenerator(&ctx->g, version, flags);
	applySeed(&ctx->g, 0, seed);
	(*env)->SetLongField(env, this, ids.cubiomes_lp, (jlong) ctx);
}

void Java_dev_corr_Cubiomes_uninitGenerator(JNIEnv *env, jobject this)
{
	cctx *ctx = (cctx *) (*env)->GetLongField(env, this, ids.cubiomes_lp);
	if (ctx) {
		free(ctx);
	}
}

jbyteArray Java_dev_corr_Cubiomes_makeImage(JNIEnv *env,
                                            jobject this,
                                            jint x,
                                            jint sx,
                                            jint z,
                                            jint sz)
{
	Range r = {
	        .scale = 1,
	        .x     = x,
	        .sx    = sx,
	        .z     = z,
	        .sz    = sz,
	        .y     = 64,
	        .sy    = 1,
	};
	cctx *ctx         = NULL;
	int *biomeIds     = NULL;
	jbyteArray retval = NULL;
	uint8_t rgb[3 * r.sx * r.sz];

	CB_ASRT(ctx = (cctx *) (*env)->GetLongField(env, this, ids.cubiomes_lp), "NULL context");
	CB_ASRT(biomeIds = allocCache(&ctx->g, r), "Failed to allocate cache");

	genBiomes(&ctx->g, biomeIds, r);
	biomesToImage(rgb, biomeColors, biomeIds, r.sx, r.sz, 1, 2);
	free(biomeIds);

	retval = (*env)->NewByteArray(env, 3 * r.sx * r.sz);
	(*env)->SetByteArrayRegion(env, retval, 0, 3 * r.sx * r.sz, rgb);
	return retval;
}

JNIEXPORT jstring JNICALL Java_dev_corr_Cubiomes_getLastError(JNIEnv *env, jobject this)
{
	return (*env)->NewStringUTF(env, "SOMETHING");
	/*
	 cctx *ctx = NULL;
	 ctx       = (cctx *) (*env)->GetLongField(env, this, ids.cubiomes_lp);
	if (!ctx) {
	    return NULL;
	}
	return (*env)->NewStringUTF(env, ctx->errstr);
	*/
}