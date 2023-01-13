/*
--- Klasa przechowująca adres (współrzędne X i Y) ---
*/

#include <iostream>

using namespace std;

class Adres {
private:
    int x;
    int y;

public:
    // Konstruktory:
    Adres() : x(0), y(0) {}
    Adres(int x, int y) : x(x), y(y) {}

    // Reguła trzech - mało złożona klasa:
    Adres(const Adres& a) = default;
    Adres& operator=(const Adres& a) = default;
    ~Adres() = default;

    // Gettery:
    int getX() const { return x; }
    int getY() const { return y; }

    // Pozostałe użyteczne operatory:
    bool operator==(const Adres& a) const { return getX() == a.getX() && getY() == a.getY(); }
    
    // Metody:
    void wypisz() const { cout << "Adres: X=" << to_string(x) << ", Y=" << to_string(y) << ";"; }
};