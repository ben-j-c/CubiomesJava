#include <ctype.h>
#include <stdio.h>

#include "cubiomes/generator.h"
#include "cubiomes/util.h"

typedef struct cubiomes_context
{
	uint64_t seed;
	int version;
	int flags;
	Generator g;
} cctx;

unsigned char biomeColors[256][3] = {0};

int main(int argc, char **argv)
{
	int version = 17, flags = 0, seed = 0;
	int x = 0, z = 0, sx = 1024, sz = 1024;

	cctx *ctx = calloc(1, sizeof(*ctx));
	if (!ctx)
		return -1;

	setupGenerator(&ctx->g, version, flags);
	applySeed(&ctx->g, 0, seed);
	Range r = {
	        .scale = 1,
	        .x     = x,
	        .sx    = sx,
	        .z     = z,
	        .sz    = sz,
	        .y     = 64,
	        .sy    = 1,
	};
	int *biomeIds = allocCache(&ctx->g, r);
	if (!biomeIds)
		return -1;
	genBiomes(&ctx->g, biomeIds, r);

	uint8_t rgb[3 * r.sx * r.sz];
	biomesToImage(rgb, biomeColors, biomeIds, r.sx, r.sz, 1, 2);
	free(biomeIds);
	free(ctx);
	return 0;
}