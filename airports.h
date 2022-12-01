//
// Created by Obed Boakye-Yiadom & Nana Kofi Djanon 30/11/2022.
//
//This file represents a header file to instantiate and declare an Airport class

#ifndef FINAL_AIRPORTS_H
#define FINAL_AIRPORTS_H

#include <map>
#include <string>
#include <vector>
using namespace std;


class airports {
public:
    string airportID, name, city, country, iata;
    static map<string, vector<airports>> portMap;
    airports(string id, string name, string city, string country, string iata);
    static void reader();
};


#endif //FINAL_AIRPORTS_H
