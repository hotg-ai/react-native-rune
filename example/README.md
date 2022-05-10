# react-native-runevm

Run Rune container in React Native (iOS & Android)

## Getting started

`$ npm install react-native-runevm --save`

### Mostly automatic installation

`$ react-native link react-native-runevm`

## Usage

Since RN does not support transferring TypedArrays to native, data is transferred in base64 over the RN-bridge.
We recommend using 'react-native-base64' package for this:

`$ npm install react-native-base64 --save`

```javascript
import Runevm, { TensorType } from 'react-native-runevm';
import base64 from 'react-native-base64';

//init the Rune 

const b64encoded = base64.encodeFromByteArray(runeBytes); //bytes is a Uint8Array containing the rune
let message = await Runevm.loadWasm(b64encoded, (message) => {
  //json formatted manifest file
  console.log(message);
});

//add input before running inference (eg image data)

const b64encoded = base64.encodeFromByteArray(inputBytes);
await Runevm.addInput(1, b64encoded, [width, width, 3], TensorType.U8, (response) => {
  console.log(response);
});

//Run model

await Runevm.runRune((message) => {
  this.setState(() => {
    return { message: message };
  });
});

//Retrieve logs of inference

await Runevm.getRuneLogs((logs) => {
  console.log(logs);
});
      
```

A working example can be found here:
[example/App.js](https://github.com/hotg-ai/react-native-rune/blob/main/example/App.js)
