// RunevmModule.java

package com.reactlibrary;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.ReadableArray;
import java.util.Base64;

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
      public native String runRune(byte[] input,int[] lengths);

    @Override
    public String getName() {
        return "Runevm";
    }

    @ReactMethod
    public void loadWasm(String base64,Callback callback) {
        byte[] bytes = Base64.getDecoder().decode(base64);
        System.out.println("Going for manifest");
        String manifest = getManifest(bytes);
        callback.invoke(manifest);
    }

    @ReactMethod
    public void runRune(String base64, ReadableArray lengths, Callback callback) {
        byte[] bytes = Base64.getDecoder().decode(base64);
        int[] lengthArray = new int[lengths.size()];
        for(int i=0;i<lengths.size();i++) {
            lengthArray[i] = lengths.getInt(i);
        }
        System.out.println("Going for output");
        String out = runRune(bytes,lengthArray);
        System.out.println(">>>>"+out);
        callback.invoke(out);
    }
}
