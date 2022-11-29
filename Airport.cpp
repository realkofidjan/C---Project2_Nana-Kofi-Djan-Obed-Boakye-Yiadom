/**
 * @author Nana Kofi Djan & Obed Boakye Yiadom
 */

#include "Airport.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


/**
 * @param id
 * @param name
 * @param city
 * @param country
 * @param iataCode
 * @param icaoCode
 * @param latitude
 * @param longitude
 */
Airport::Airport(string id, string name, string city, string country, string iataCode, string icaoCode, double latitude, double longitude) {
    this->airportID = id;
    this->name = name;
    this->city = city;
    this->country = country;
    this->iataCode = iataCode;
    this->icaoCode = icaoCode;
    this->latitude = latitude;
    this->longitude = longitude;
}


/**
 * getting the name of the airport
 */
string Airport::getName() {
    return this->name;
}

/**
 * getting the id of the airport
 */
string Airport::getID() {
    return this->airportID;
}

/**
 * getting the airport country
 */
string Airport::getCountry() {
    return this->country;
}


/**
 * IATA Code of that airport
 */
string Airport::getIataCode() {
    return iataCode;
}

/**
 * ICAO Code of that airport
 */
string Airport::getIcaoCode() {
    return this->icaoCode;
}

/**
 * getting the city of the airport
 */
string Airport::getCity() {
    return this->city;
}


/**
 * reading the CSV
 */
void Airport::readFile(string csvFile) {
    ifstream fileStream;

    try{
        fileStream.open(csvFile);
        string line, id, name, city, country, iata, icao, tempLatitude, tempLongitude;
        double latitude = 0, longitude;
        int count = 0;


        while(getline(fileStream, line)){
            if (count == 0){
                count++;
                continue;
            }
            stringstream stream(line);

            getline(stream, id, ',');
            getline(stream,name,',');
            getline(stream,city,',');
            getline(stream,country,',');
            getline(stream,iata,',');
            getline(stream,icao,',');
            getline(stream,tempLatitude,',');
            getline(stream,tempLongitude,',');

            if (tempLatitude == " ")
                continue;

            else
                latitude = stod(tempLatitude);

            longitude = stod(tempLongitude);



            string key = city + ", " + country;
            Airport tempAirport = Airport(id, name, city, country, iata, icao, latitude, longitude);

            Airport::codes.insert({iata, tempAirport});

            if (Airport::airports.find(key) != Airport::airports.end()) {
                vector<Airport> tempList = Airport::airports[key];
                tempList.emplace_back(tempAirport);
                Airport::airports.erase(key);
                Airport::airports.insert({key,tempList});
            }
            else{
                vector<Airport> tempList;
                tempList.emplace_back(tempAirport);
                Airport::airports.insert({key,tempList});
            }

        }

    }

    catch (std::exception const& e){
        cout << "There was an error: " << e.what() << endl;
    }
    fileStream.close();


}

/**
 * @param source
 */
vector<Airport> Airport::getAirport(string source) {
    vector<Airport> portList = Airport::airports[source];
    return portList;
}

/**
 * @param iataCode
 */
Airport Airport::getObject(string iataCode) {
    Airport tempPort = Airport::codes.at(iataCode);
    return tempPort;
}
