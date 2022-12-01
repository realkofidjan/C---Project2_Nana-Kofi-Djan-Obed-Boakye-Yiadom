//
// Created by Obed Boakye-Yiadom on 30/11/2022.
//

#include "airports.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

airports::airports(string id, string name, string city, string country, string iata) {
    this->airportID = id;
    this->name = name;
    this->city = city;
    this->country = country;
    this->iata = iata;
}

void airports::reader() {
    ifstream file;

    file.open("/Users/xander/Downloads/C++ Individual Project 2/airports.csv");
    string temp, ID, name, city, country, iata;



    while(getline(file, temp)){
        stringstream filestream(temp);

        getline(filestream, ID, ',');
        getline(filestream,name,',');
        getline(filestream,city,',');
        getline(filestream,country,',');
        getline(filestream,iata,',');

        string key = city + ", " + country;

        if (airports::portMap.find(key) != airports::portMap.end()) {
            airports p = airports(ID, name, city, country, iata);
            vector<airports> list = airports::portMap.at(key);
            list.emplace_back(p);
            airports::portMap.erase(key);
            airports::portMap.insert({key,list});
        }
        else{
            airports p = airports(ID, name, city, country, iata);
            vector<airports> list;
            list.emplace_back(p);
            airports::portMap.insert({key,list});
        }

    }


}
