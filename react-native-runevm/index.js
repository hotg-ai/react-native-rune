// main index.js

import { NativeModules } from 'react-native';

const { Runevm } = NativeModules;

const TensorType = {
    U8: 0,
    I8: 1,
    U16: 2,
    I16: 3,
    U32: 4,
    I32: 5,
    F32: 6,
    U64: 7,
    I64: 8,
    F64: 9,
};
export { TensorType };
export default Runevm;
