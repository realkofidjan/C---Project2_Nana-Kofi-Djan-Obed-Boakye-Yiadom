//
// Created by Obed Boakye-Yiadom and Nana Kofi Djan on 29/11/2022.
//
// This file represents an implementation of the Airport class

#include "airports.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

/**
 * An airport constructor
 * @param id an id of the airport
 * @param name the name of the airport
 * @param city the city location of the airport
 * @param country the country location of the airport
 * @param iata the iata code of the airport
 *
 * */
airports::airports(string id, string name, string city, string country, string iata) {
    this->airportID = id;
    this->name = name;
    this->city = city;
    this->country = country;
    this->iata = iata;
}


/**
 * A function to read the airport file and store each airport in the file created in a map
 *
 * */


void airports::reader() {
    ifstream file;
    file.open("airports.csv");
    string temp, ID, name, city, country, iata;


//the while loop gets each line of the file and stores it in a string called temp
// the stringstream then retrieves each line (now a string). The lines are now separated by a comma delimiter
// and allocated to each string variable created above. These variables are now used in the object definiton as attributes
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
