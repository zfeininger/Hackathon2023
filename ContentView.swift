//
//  ContentView.swift
//  TestDemo
//
//  Created by Zachary Feininger on 2/3/23.
//

import SwiftUI
import UIKit
import MapKit
import CoreLocation
import Combine


struct ContentView: View {
    @State private var showText = false
    @State private var showTextHigh = false
    @State private var showTextLow = false

    
    var body: some View {
        NavigationView {
            ZStack {
                Image("IMG_002")
                        .resizable()
                        .scaledToFill()
                        .frame(width:750, height:500)
                VStack(spacing: 5) {
                    Text("Welcome")
                        .fontWeight(.bold)
                        .font(.system(size: 60))
                        .opacity(showText ? 1 : 0)
                        .frame(width: 300, height:200,  alignment: .top)
                        .animation(Animation.linear(duration: 1).delay(0.5))
                        .onAppear() {
                            self.showText = true
                        }
                    HStack {
                        NavigationLink(destination: SecondPage()) {
                            Rectangle()
                                .fill(Color.green.opacity(0.75))
                                .cornerRadius(15)
                                .frame(width: 200, height: 70)
                                .overlay(
                                    Text("Share Data")
                                        .fontWeight(.bold)
                                        .foregroundColor(.black)
                                        .font(.title)
                                        .opacity(self.showTextHigh ? 1 : 0)
                                        .animation(Animation.linear(duration: 1).delay(1.5))
                                        .onAppear() {
                                            self.showTextHigh = true
                                        }
                                )
                                .opacity(self.showTextHigh ? 1 : 0)
                                .animation(Animation.linear(duration: 1).delay(1.5))
                                .onAppear() {
                                    self.showTextHigh = true
                                }
                                .padding(.all, 50)
                            
                        }
                        NavigationLink(destination:
                                        infoPage()) {
                            Text("?")
                                .fontWeight(.bold)
                        }
                        .opacity(self.showTextHigh ? 1 : 0)
                        .animation(Animation.linear(duration: 1).delay(1.5))
                        .onAppear() {
                            self.showTextHigh = true
                        }
                        .font(.system(size: 50))
                    }
                    HStack {
                        NavigationLink(destination:
                                        SecondPage()) {
                            Rectangle()
                                .fill(Color.red.opacity(0.75))
                                .cornerRadius(15)
                                .frame(width: 200, height: 70)
                                .overlay(
                                    Text("Hide Data")
                                        .fontWeight(.bold)
                                        .foregroundColor(.black)
                                        .font(.title)
                                        .opacity(self.showTextLow ? 1 : 0)
                                        .animation(Animation.linear(duration: 1).delay(1.5))
                                        .onAppear() {
                                            self.showTextLow = true
                                        }
                                )
                                .opacity(self.showTextLow ? 1 : 0)
                                .animation(Animation.linear(duration: 1).delay(1.5))
                                .onAppear() {
                                    self.showTextLow = true
                                }
                                .padding(.all, 50)
                        }
                        NavigationLink(destination:
                                        infoPage()) {
                            Text("?")
                                .fontWeight(.bold)
                        }

                        .opacity(self.showTextHigh ? 1 : 0)
                        .animation(Animation.linear(duration: 1).delay(1.5))
                        .onAppear() {
                            self.showTextHigh = true
                        }
                        .font(.system(size: 50))
                    }
                }
            }
        }
    }
}

struct infoPage: View {
    var body: some View {
        ZStack {
            Image("IMG_002")
                .resizable()
                .scaledToFill()
                .frame(width:750, height:500)
            
            VStack(alignment: .center) {
                Text("Share Data")
                    .fontWeight(.bold)
                    .font(.system(size: 65))
                Text("Share location to enhance the experience of other users.")
                    .fontWeight(.bold)
                    .font(.system(size: 20))
                    .frame(width:200,height:100)
                Text("Hide Data")
                    .fontWeight(.bold)
                    .font(.system(size: 65))
                Text("Hide location data but keep access to our content.")
                    .fontWeight(.bold)
                    .font(.system(size:20))
                    .frame(width:200,height:100)
            }
            .padding(50)

        }
        .multilineTextAlignment(.center)
    }

}

struct SecondPage: View {
    @State private var showStreetSmarts = false
    @StateObject var deviceLocationService = DeviceLocationService.shared
    @State var tokens: Set<AnyCancellable> = []
    @State var coordinates: (lat: Double, lon: Double) = (0,0)
    var body: some View {
            NavigationView {
                VStack(spacing: 175) {
                    Text("Street Smarts")
                        .fontWeight(.bold)
                        .font(.system(size: 50))
                        .opacity(self.showStreetSmarts ? 1 : 0)
                        .animation(Animation.linear(duration: 1).delay(1.0))
                        .onAppear() {
                            self.showStreetSmarts = true
                        }
                    Text("Latitude: \(coordinates.lat)")
                        .font(.largeTitle)
                        .onAppear {
                            observeCoordinateUpdates()
                            observeLocationAccessDenied()
                            deviceLocationService.requestlocationUpdates()
                        }
                    Text("Longitude: \(coordinates.lon)")
                        .font(.largeTitle)
                        .onAppear {
                            observeCoordinateUpdates()
                            observeLocationAccessDenied()
                            deviceLocationService.requestlocationUpdates()
                        }
                    
                    Text("")
                        .navigationBarTitle("", displayMode: .inline)
                        .toolbar {
                            ToolbarItemGroup(placement:.bottomBar) {
                                NavigationLink(destination: ThirdPage()) {
                                    Image(systemName: "house")
                                        .font(.system(size: 40))
                                }
                                NavigationLink(destination: ThirdPage()) {
                                    Image(systemName: "map")
                                        .font(.system(size: 40))
                                }
                                NavigationLink(destination: statPage()) {
                                    Image(systemName: "chart.bar")
                                        .font(.system(size: 40))
                                        .frame(width:90,height:10)
                                }
                                NavigationLink(destination: ThirdPage()) {
                                    Image(systemName: "gear")
                                        .font(.system(size: 40))
                                }
                            }
                        }
                }
        }
            .navigationBarHidden(true)
    }
    func observeCoordinateUpdates() {
        deviceLocationService.coordinatesPublisher
            .receive(on: DispatchQueue.main)
            .sink { completion in
                if case .failure(let error) = completion {
                    print(error)
                }
            } receiveValue: { coordinates in
                self.coordinates = (coordinates.latitude, coordinates.longitude)
            }
            .store(in: &tokens)
    }
    func observeLocationAccessDenied() {
        deviceLocationService.deniedLocationAccessPublisher
            .receive(on: DispatchQueue.main)
            .sink {
                print("Show some kind of alert to the user")
            }
            .store(in: &tokens)
    }
}


struct Location: Identifiable {
    let id = UUID()
    let name: String
    let coordinate: CLLocationCoordinate2D
}

let locations = [
    Location(name:"Test", coordinate: CLLocationCoordinate2D(latitude: 51.501, longitude: -0.141)) // currLat, currLong
]

struct ThirdPage: View {
    @State private var region = MKCoordinateRegion( center: CLLocationCoordinate2D(latitude: 40, longitude: 14), span: MKCoordinateSpan(latitudeDelta: 0.03, longitudeDelta: 0.03))
    var body: some View {
        Map(coordinateRegion: $region)
            .edgesIgnoringSafeArea(.all)
        }
    }
    
    struct ContentView_Previews: PreviewProvider {
        static var previews: some View {
            ContentView()
        }
    }

struct statPage: View {
    var body: some View {
        Text("test")
    }
}

