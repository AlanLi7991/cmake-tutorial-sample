#!/bin/sh

set -o errexit

# Locate directory
ELECTRON_DIR=$(pwd)
WASM_DIR="$ELECTRON_DIR/../../wrapper/WebAssembly"

# Lcoate SDK
SDK_JS="$WASM_DIR/wasm/AlgorithmSDK.js"
SDK_WASM="$WASM_DIR/wasm/AlgorithmSDK.wasm"
SDK_JS_SYNC="$WASM_DIR/wasm/AlgorithmSDK_Sync.js"
SDK_WASM_SYNC="$WASM_DIR/wasm/AlgorithmSDK_Sync.wasm"

# Build SDK
if [ ! -e $SDK_WASM -o ! -e $SDK_JS -o ! -e $SDK_JS_SYNC -o ! -e $SDK_WASM_SYNC ] 
then
    cmake --build "$WASM_DIR"
fi

# Copy SDK
cp $WASM_DIR/wasm/AlgorithmSDK*.* "$ELECTRON_DIR/"


# Set up Electron
npm install && npm start