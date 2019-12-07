package com.sdk.sdk



class SDKKotlin {

    external fun invokeAlgorithmFunc(number: Int, callback: IAlgorithmInterface): Int

    companion object {

        init {
            System.loadLibrary("AlgorithmSDK")
        }
    }

}