package com.mapbox.mapboxsdk.annotations;

import android.graphics.PointF;

public class MarkerViewSettings {

    private Marker marker;
    private PointF centerOffset;
    private int infoWindowTopOffset;

    public MarkerViewSettings(Marker marker) {
        this.marker = marker;
    }

    public PointF getCenterOffset() {
        return centerOffset;
    }

    public void setCenterOffset(PointF centerOffset) {
        this.centerOffset = centerOffset;
    }

    public int getInfoWindowTopOffset() {
        return infoWindowTopOffset;
    }

    public void setInfoWindowTopOffset(int infoWindowTopOffset) {
        this.infoWindowTopOffset = infoWindowTopOffset;
    }
}
