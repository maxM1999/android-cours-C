package com.example.jni_1

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.example.jni_1.databinding.ActivityMainBinding

class Person(var name:String, var age:Int){

}

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val person = Person("Julien", 12)
        SendPerson(person)

        // Example of a call to a native method
        binding.sampleText.text = Add(2f, 4f).toString()
    }

    /**
     * A native method that is implemented by the 'jni_1' native library,
     * which is packaged with this application.
     */
    external fun Add(a:Float, b:Float): Float
    external fun SendPerson(p:Person):Unit

    companion object {
        // Used to load the 'jni_1' library on application startup.
        init {
            System.loadLibrary("jni_1")
        }
    }
}