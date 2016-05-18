package com.mapbox.mapboxsdk.annotations;

import android.content.Context;
import android.widget.FrameLayout;

/**
 * The MGLAnnotationView class is responsible for representing point-based annotation markers as a view.
 * Annotation views represent an annotation object, which is an object that corresponds to the Annotation protocol.
 * When an annotation’s coordinate point is visible on the map view, the map view adapter is asked
 * to provide a corresponding annotation view. If an annotation view is created with a reuse identifier,
 * the map view may recycle the view when it goes offscreen.
 */
public class MarkerView extends FrameLayout {

    public MarkerView(Context context) {
        super(context);
    }

}
