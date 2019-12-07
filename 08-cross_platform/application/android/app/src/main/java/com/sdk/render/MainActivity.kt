package com.sdk.render

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import com.sdk.sdk.IAlgorithmInterface
import com.sdk.sdk.SDKKotlin

class MainActivity : AppCompatActivity() {

    private val sdk = SDKKotlin()
    private var button: Button? = null
    private var textView: TextView? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        button = findViewById(R.id.invoke_button)
        textView = findViewById(R.id.text_view)

        button?.setOnClickListener {
            sdk.invokeAlgorithmFunc(5, object: IAlgorithmInterface {
                override fun onCallback(number: Int, message: String) {
                    textView?.text = message
                    println(message)
                }
            })
        }
    }

}
