// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main()
{

    Navigator nav;
    nav.loadMapData("/Users/Joey/Desktop/mapdata.txt");
    vector<NavSegment> empty;
    if(nav.navigate("Southern Regional Library Facility", "gardenia", empty) == NAV_SUCCESS)
        cout << "Found A Route" << endl;
    else
        cout << "Did Not Find A Route OR Bad Location" << endl;
    
    
    /**
    
    Navigator nav;
    nav.loadMapData("/Users/Joey/Desktop/mapdata.txt");
    vector<NavSegment> empty;
    if(nav.navigate("Acacia", "Getty Center Helipad", empty) == NAV_SUCCESS)
        cout << "Found A Route" << endl;
    else
        cout << "Did Not Find A Route OR Bad Location" << endl;
    
    
    /**
    
    
    MapLoader mapTest;
    mapTest.load("/Users/Joey/Desktop/mapdata.txt");
    AttractionMapper attMap;
    attMap.init(mapTest);
    SegmentMapper segTest;
    segTest.init(mapTest);
    
    //34.0779524, -118.4311453
    GeoCoord onlyOne = GeoCoord("34.0779524", "-118.4311453");
    vector<StreetSegment> test;
    test = segTest.getSegments(onlyOne);
    cout << "Charing Something" << endl << "34.0779524, -118.4311453" << endl
    << "Associated Street Segments" << endl;
    for(int i = 0; i < test.size(); i++) {
        cout << endl;
        cout << test[i].streetName << endl;
        cout << "Start Coords: " + test[i].segment.start.latitudeText + ", " +
        test[i].segment.start.longitudeText << endl;
        cout << "End Coords: " + test[i].segment.end.latitudeText + ", " +
        test[i].segment.end.longitudeText << endl;
    }
    
    
    
    /**
    Navigator nav;
    nav.loadMapData("/Users/Joey/Desktop/mapdata.txt");
    vector<NavSegment> empty;
    if(nav.navigate("The Playboy Mansion", "Acacia", empty) == NAV_SUCCESS)
        cout << "Found A Route" << endl;
    else
        cout << "Did Not Find A Route OR Bad Location" << endl;
    
    
    
    /**
    MapLoader mapTest;
    mapTest.load("/Users/Joey/Desktop/mapdata.txt");
    AttractionMapper attMap;
    attMap.init(mapTest);
    SegmentMapper segTest;
    segTest.init(mapTest);
    
    // 34.0636863, -118.4447958
    GeoCoord testCoord = GeoCoord("34.0636863","-118.4447958");
    vector<StreetSegment> testVec;
    testVec = segTest.getSegments(testCoord);
    for(auto it: testVec) {
        cerr << "Found Seg: " + it.streetName + "   =>   " << endl
        << it.segment.start.latitudeText + ", " + it.segment.start.longitudeText << endl
        << it.segment.end.latitudeText + ", " + it.segment.end.longitudeText << endl;
    }
    
    /**
    Navigator nav;
    nav.loadMapData("/Users/Joey/Desktop/mapdata.txt");
    vector<NavSegment> empty;
    if(nav.navigate("Geffen Playhouse", "School of Public Health", empty) == NAV_SUCCESS)
        cout << "Found A Route" << endl;
    else
        cout << "Did Not Find A Route OR Bad Location" << endl;
    
    
    /**
    
    MapLoader mapTest;
    mapTest.load("/Users/Joey/Desktop/mapdata.txt");
    AttractionMapper attMap;
    attMap.init(mapTest);
    SegmentMapper segTest;
    segTest.init(mapTest);
    

    GeoCoord coords;
    string place = "The Playboy Mansion";
    if(attMap.getGeoCoord(place, coords) == true) {
        cout << place + " is located at: " << endl;
        cout << coords.latitudeText << endl << coords.longitudeText << endl;
    }
    
    
    /**
    // 34.0544590,-118.4801137
    GeoCoord onlyOne = GeoCoord("34.0544590", "-118.4801137");
    vector<StreetSegment> test;
    test = segTest.getSegments(onlyOne);
    cout << "Helena" << endl << "34.0544590, -118.4801137" << endl
    << "Associated Street Segments" << endl;
    for(int i = 0; i < test.size(); i++) {
        cout << endl;
        cout << test[i].streetName << endl;
        cout << "Start Coords: " + test[i].segment.start.latitudeText + ", " +
        test[i].segment.start.longitudeText << endl;
        cout << "End Coords: " + test[i].segment.end.latitudeText + ", " +
        test[i].segment.end.longitudeText << endl;
    }
    
    
    
    /**
    for(int i = 0; i < mapTest.getNumSegments(); i++) {
        StreetSegment fillMe;
        mapTest.getSegment(i, fillMe);
        for(int j = 0; j < fillMe.attractions.size(); j++) {
            cout << "GeoCoordinate of Attraction: " + fillMe.attractions[j].name << endl << fillMe.attractions[j].geocoordinates.latitudeText
            << " latitude and " << fillMe.attractions[j].geocoordinates.longitudeText << " longitude." << endl;
            vector<StreetSegment> streetSegs;
            streetSegs = segTest.getSegments(fillMe.attractions[j].geocoordinates);
            cout << "Segments Associated:" << endl;
            for(int k = 0; k < streetSegs.size(); k++) {
                cout << streetSegs[k].streetName << endl;
            }
            cout << endl;
        }
    }
    
    
    
    /**
    ifstream validLocs;
    validLocs.open("/Users/Joey/Desktop/validlocs.txt");
    if (validLocs.is_open())
    {
        string line;
        while ( getline (validLocs,line) ) // Go through every line in the file
        {
            cout << "Attraction Name: " + line.substr(0,line.find('|')-1) << endl;
            GeoCoord coords;
            attMap.getGeoCoord(line.substr(0,line.find('|')-1), coords);
            cout << "Latitude: " + coords.latitudeText << endl
            << "Longitude: " + coords.longitudeText << endl;
            cout << endl;
        }
    }
     
     
     /**
     
     GeoCoord coords;
     string place = "The Playboy Mansion";
     if(attMap.getGeoCoord(place, coords) == true) {
     cout << place + " is located at: " << endl;
     cout << coords.latitudeText << endl << coords.longitudeText << endl;
     }
     
    
    /**
    string tester = "34.0602175, -118.4464952 34.0597400,-118.4460477";
    char limit[2] = {','};
    vector<string> splitUp;
    tokenize(tester, limit, 1, splitUp);
    for(int i = 0; i < 4; i++) {
        cout << splitUp[i] << endl;
    }
    
    
    /**
    vector<int> test;
    cout << "Max default capacity is " << test.capacity() << endl;
    cout << "Current size is " << test.size() << endl;
    cout << endl;
    
    test.push_back(5);
    cout << "Max default capacity is " << test.capacity() << endl;
    cout << "Current size is " << test.size() << endl;
    cout << endl;
    
    test.push_back(5);
    test.push_back(5);
    test.push_back(5);
    test.push_back(5);
    test.push_back(5);
    test.push_back(5);
    test.push_back(5);
    cout << "Max default capacity is " << test.capacity() << endl;
    cout << "Current size is " << test.size() << endl;
    cout << endl;
    
    
    
    MyMap<string, int> test;
    cerr << test.size() << endl;
    test.associate("Joey", 10);
    if(test.find("Joey") != nullptr)
        cerr << *(test.find("Joey")) << endl;
    cerr << test.size() << endl;
    test.associate("Joey", 12);
    if(test.find("Joey") != nullptr)
        cerr << *(test.find("Joey")) << endl;
    cerr << test.size() << endl;
    test.associate("Zach", 10);
    cerr << test.size() << endl;
    test.clear();
    cerr << test.size() << endl;
    if(test.find("Joey") != nullptr)
        cerr << *(test.find("Joey")) << endl;
    **/
    
    
}

