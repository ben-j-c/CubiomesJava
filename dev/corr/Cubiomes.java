package dev.corr;

import java.lang.RuntimeException;

public class Cubiomes implements AutoCloseable {
	static {
		System.loadLibrary("Cubiomes");
		System.out.println("Cubiomes shared library loaded.");
		initLibrary();
	}
	private long cubiomes_lp = 0;

	private static native void initLibrary();
	private native void initGenerator(long seed, int version, int options, int dimension);
	private native void uninitGenerator();
	public native byte[] makeImage(int x, int sx, int z, int sz);
	public native String getLastError();

	public Cubiomes(long seed, int version, int options, int dimension) {
		if (version < 0 || version > 18)
			throw new RuntimeException("Out of bounds version information");
		if (dimension != -1 && dimension != 0 && dimension != 1)
			throw new RuntimeException("Incorrect dimension given");
		initGenerator(seed, version, options, dimension);
	}

	public void close() {
		uninitGenerator();
	}

	public static final int OPT_LARGE_BIOMES = 0x1;
	public static final int OPT_FORCE_OCEAN_VARIANTS = 0x4;

	public static final int VER_MC_1_0 = 0;
	public static final int VER_MC_1_1 = 1;
	public static final int VER_MC_1_2 = 2;
	public static final int VER_MC_1_3 = 3;
	public static final int VER_MC_1_4 = 4;
	public static final int VER_MC_1_5 = 5;
	public static final int VER_MC_1_6 = 6;
	public static final int VER_MC_1_7 = 7;
	public static final int VER_MC_1_8 = 8;
	public static final int VER_MC_1_9 = 9;
	public static final int VER_MC_1_10 = 10;
	public static final int VER_MC_1_11 = 11;
	public static final int VER_MC_1_12 = 12;
	public static final int VER_MC_1_13 = 13;
	public static final int VER_MC_1_14 = 14;
	public static final int VER_MC_1_15 = 15;
	public static final int VER_MC_1_16 = 16;
	public static final int VER_MC_1_17 = 17;
	public static final int VER_MC_1_18 = 18;

	public static final int DIM_NETHER = -1;
	public static final int DIM_OVERWORLD = 0;
	public static final int DIM_END = 1;
}