/*
--- Klasa przechowująca towar (prezent) ---

Uwagi:
# Cenę i ilość towaru ustala sklep - stąd brak konstruktora przyjmującego te wartości;
*/

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

class Towar {
private:
    string nazwa;
    int ilosc_towaru;
    double cena_towaru;

public:
    // Konstruktory:
    Towar() : nazwa(""), ilosc_towaru(0), cena_towaru(0) {}
    Towar(string nazwa) : nazwa(nazwa), ilosc_towaru(0), cena_towaru(0) {}

    // Reguła trzech - mało złożona klasa:
    Towar(const Towar& t) = default;
    Towar& operator=(const Towar& t) = default;
    ~Towar() = default;

    // Gettery:
    string getNazwa() { return nazwa; }
    int getIloscTowaru() { return ilosc_towaru; }
    double getCenaTowaru() { return cena_towaru; }

    // Settery:
    void setIloscTowaru(int i) { ilosc_towaru = i; }
    void setCenaTowaru(double c) { cena_towaru = c; }

    // Metody:
    void wypisz() const;
};

void Towar::wypisz() const {
    ostringstream stream;
    stream << fixed << setprecision(2) << cena_towaru;
    cout << " Nazwa towaru: " << nazwa << " | Ilość towaru: " << to_string(ilosc_towaru) << " | Cena towaru: " << stream.str() << "\n";
}