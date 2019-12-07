//
//  ContentView.swift
//  Render
//
//  Copyright © 2019 Alanli7991. All rights reserved.
//

import SwiftUI

struct ContentView: View {
    
    @State var message = "Hello World!"
    let sdk = SDKObjectiveC()
    
    var body: some View {
        VStack {
            Text(verbatim: message)
                .offset(x: 0, y: -100)
                .padding(.horizontal)
                .padding(.vertical)
            Button<Text>("Invoke Native") {
                self.sdk.invokeAlgorithmFunc(5) { (number, message) in
                    self.message = message
                }
            }
            .offset(x: 0, y: 100)
            
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
