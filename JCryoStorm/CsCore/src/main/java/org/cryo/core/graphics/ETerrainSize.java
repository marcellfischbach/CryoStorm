package org.cryo.core.graphics;

public enum ETerrainSize {
    TS_33     ( 33),
    TS_65     ( 65),
    TS_129    ( 129),
    TS_257    ( 257),
    TS_513    ( 513),
    TS_1025   ( 1025),
    TS_2049   ( 2049),
    TS_4097   ( 4097);

    ETerrainSize(int size) {
        this.size = size;
    }

    public final int size;

    public static ETerrainSize bySize (int size) {
        for (ETerrainSize value : ETerrainSize.values()) {
            if (value.size == size) {
                return value;
            }
        }
        return null;
    }
}
