#!/bin/sh

set -o errexit

# Locate directory
WEB_DIR=$(pwd)
WASM_DIR="$WEB_DIR/../../wrapper/WebAssembly"

# Lcoate SDK
SDK_JS="$WASM_DIR/wasm/AlgorithmSDK.js"
SDK_WASM="$WASM_DIR/wasm/AlgorithmSDK.wasm"

# Build SDK
if [ ! -e $SDK_WASM -o ! -e $SDK_JS ] 
then
    cmake --build "$WASM_DIR"
fi

# Copy SDK
cp "$SDK_JS" "$WEB_DIR/"
cp "$SDK_WASM" "$WEB_DIR/"

# Setup http server
python -m SimpleHTTPServer 8080