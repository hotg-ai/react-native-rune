// RunevmModule.java

package com.reactlibrary;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.ReadableArray;
public class RunevmModule extends ReactContextBaseJavaModule {

    private final ReactApplicationContext reactContext;
    boolean isLibraryLoaded = false;

    public RunevmModule(ReactApplicationContext reactContext) {
        super(reactContext);
        this.reactContext = reactContext;
        if(!isLibraryLoaded) {
            System.loadLibrary("rune_vm_loader");
            isLibraryLoaded = true;
        }
    }

      //declare SDK functions
      public native String getLogs();
      public native String getManifest(byte[] wasm);
      public native String runRune(byte[] input,int[] lengths[]);

    @Override
    public String getName() {
        return "Runevm";
    }

    @ReactMethod
    public void loadWasm(String base64,Callback callback) {
        // TODO: Implement some actually useful functionality
        callback.invoke("Received wasm " + base64.length());
    }

    @ReactMethod
    public void runRune(String base64, ReadableArray lengths, Callback callback) {
        // TODO: Implement some actually useful functionality
        callback.invoke("Received wasm " + base64.length());
    }
}
