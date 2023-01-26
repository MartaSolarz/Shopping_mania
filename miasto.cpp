/*
--- Klasa przechowująca Miasto ---

Uwagi:
# miasto przechowuje wektor obiektów handlowych - sklepów (sklep wie czy jest w sieci i jeśli tak to w której);
# nie dodajemy sprawdzenia czy dany kupujący w wektorze jest unikalny, bo teoretycznie mogą istnieć w mieście dwie osoby o tej samej nazwie, budzecie i sposobie podejścia do zakupów
*/

#include <string>
#include <vector>
#include <algorithm>

#include "kupujacy.cpp"

class Miasto {
    private:
        string nazwa;
        vector<Sklep*> sklepy;
        vector<Kupujacy*> kupujacy;

    public: 
    // Konstruktor:
    Miasto() : nazwa("") {}
    Miasto(string nazwa) : nazwa(nazwa) {}

    // Destruktor:
    ~Miasto() = default;
    // nie ma potrzeby definiowania operatora = i konstruktora kopiującego

    // Gettery:
    string getNazwa() const { return nazwa; }
    vector<Sklep*> getSklepy() const { return sklepy; }
    vector<Kupujacy*> getKupujacy() const { return kupujacy; }

    // Metody:
    Miasto& dodajSklep(Sklep* sklep);
    Miasto& dodajKupujacego(Kupujacy* k);
    Miasto& usunSklep(Sklep* sklep);
    Miasto& tura();
};

Miasto& Miasto::dodajSklep(Sklep* sklep) {
    auto it = find_if(sklepy.begin(), sklepy.end(), [&](const Sklep* s) { return s == sklep; });
    if (it == sklepy.end()) { sklepy.push_back(sklep); }
    return *this;
}

Miasto& Miasto::dodajKupujacego(Kupujacy* k) { 
    kupujacy.push_back(k);
    return *this;
}

Miasto& Miasto::usunSklep(Sklep* sklep) {
    auto it = find(sklepy.begin(), sklepy.end(), sklep);
    if (it != sklepy.end()) {
        sklepy.erase(it);
    }
    return *this;
}

Miasto& Miasto::tura() {
    cout << "\n...zakupomani wyruszają na zakupy!\n\n";
    
    cout << "Kupiono: \n";
    for (auto el: getKupujacy()) { 
        cout << el->getNazwa();
        auto result = (*el).kup(getSklepy());
        cout << ": " << result << "\n";
    }

    cout << "\n...stany sklepów i salda kontów zakupomanów zostały zaktualizowane w systemie!\n\n";
    return *this;
}