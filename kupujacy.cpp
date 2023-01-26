/*
--- Skrypt przechowujący Kupujących i klasy dziedziczące po niej ---

Uwagi:
# wyróżniono dwa typy nadrzędne kupujących -> fani Sklepów (Maksymaliści, Minimaliści, Losowi), fani Towarów (Inni, Oszczędni, Tradycjonaliści);
# maksymaliści cenowi kupują w ulubionym sklepie najdroższy dostępny prezent na jaki ich stać;
# minimaliści najtańszy;
# losowi kupujący próbują trzy razy wybrać losowo wybrany (z oferowanych w ulubionym sklepie) prezent, jeśli jest dostępny (tj. liczba sztuk jest większa od zera) i mieści się w
budżecie, to go wybierają (i już nie próbują ponownie losować), wpp. losują ponownie (o ile nie była to ostatnia próba)
# inni próbują kupować w dowolnych sklepach w mieście, za to szukają konkretnego ulubionego produktu;
# oszczędni pobierają listę sklepów z miasta, dowiadują się o cenę poszukiwanego produktu we wszystkich sklepach i kupują tam, gdzie jest najtaniej (jeśli kilka sklepów oferuje taką samą cenę, to wybierają dowolny z nich);
# tradycjonaliści nie kupują przez Internet (oferta sklepów, które są w sieci dostępna jest online) i szukają najbliższego im zwykłego sklepu z poszukiwanym ulubionym towarem (jeśli jest kilka w tej samej odległości wybierają dowolny z nich).
# metoda kup przyjmuje wektor Sklepów - kupujących (poza Tradycjonalistą) nie interesuje czy sklep (w szczególności ich ulubiony sklep) jest częścią sieci, a osoby szukające konkretnego towaru i tak najpierw porównują oferty ze wszystkich sklepów w mieście (i na tym etapie nie ma dla nich znaczenia przynależność sklepu do sieci).
*/

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "sklepy.cpp"

using namespace std;

// --- Klasa Kupujacy ---

class Kupujacy {
protected:
    string nazwa;
    double budzet;
    Adres adres;

    bool czyGoStac(double cena) {  // ta metoda jest protected ponieważ tylko dany Kupujący wie czy go stać na dany towar
        if (budzet >= cena) { return true; }
        return false;
    }

public:
    // Konstruktor:
    Kupujacy() = delete;
    Kupujacy(string nazwa, double budzet, Adres wsp) : nazwa(nazwa), budzet(budzet), adres(wsp) {}

    // Reguła trzech:
    Kupujacy(const Kupujacy& k) = default;
    Kupujacy& operator=(const Kupujacy& k) = default;
    virtual ~Kupujacy() = 0;

    // Gettery:
    string getNazwa() const { return nazwa; }
    double getBudzet() const { return budzet; }
    Adres getAdres() const { return adres; }

    // Pozostałe operatory:
    bool operator==(const Kupujacy& k) { return getNazwa() == k.getNazwa() && getAdres() == k.getAdres(); }
        
    // Settery:
    void setBudzet(double b) { budzet = b; }

    // Metody:
    virtual void wypisz() const = 0;
    virtual bool kup(vector<Sklep*> sklepy) = 0;
};

Kupujacy::~Kupujacy() = default;

// --- Klasa FanSklepu ---

class FanSklepu : public Kupujacy {
protected:
    Sklep* ulubiony_sklep;

    bool czySklepIstnieje(vector<Sklep*> sklepy);
    bool dokonajTranzakcji(string nazwa);

public:
    // Konstruktor:
    FanSklepu(string nazwa, double budzet, Adres wsp, Sklep* s) : Kupujacy(nazwa, budzet, wsp) { ulubiony_sklep = s; }

    // Destruktor:
    virtual ~FanSklepu() = 0;

    // Setter:
    void setUlubionySklep(Sklep &s) { ulubiony_sklep = &s; }

    // Metody:
    void wypisz() const override {
        ostringstream stream;
        stream << fixed << setprecision(2) << budzet;
        cout << "Imię i nazwisko: " << nazwa << " | Budżet: " << stream.str();
        cout << " | ";
        adres.wypisz();
        cout << " | Ulubiony sklep: " << ulubiony_sklep->getNazwa() << "\n";
    }
    virtual bool kup(vector<Sklep*> sklepy) = 0;
};

FanSklepu::~FanSklepu() = default;

bool FanSklepu::czySklepIstnieje(vector<Sklep*> sklepy) {
    for (Sklep* s : sklepy) {
        if (s == ulubiony_sklep) { return true; }
    }
    return false;
}

bool FanSklepu::dokonajTranzakcji(string nazwa) {
    if (ulubiony_sklep->sprzedajTowar(nazwa)) {
        budzet -= ulubiony_sklep->podajCeneISztuki(nazwa).first;
        return true;
    }
    return false;
}

// --- Klasa FanTowaru ---

class FanTowaru : public Kupujacy {
protected:
    string ulubiony_towar;

    bool dokonajTranzakcji(Sklep* sklep);

public:
    // Konstruktor:
    FanTowaru(string nazwa, double budzet, Adres wsp, string n) : Kupujacy(nazwa, budzet, wsp) { ulubiony_towar = n; }

    // Destruktor:
    virtual ~FanTowaru() = 0;

    // Setter:
    void setUlubionyTowar(string n) { ulubiony_towar = n; }

    // Metody:
    void wypisz() const override {
        ostringstream stream;
        stream << fixed << setprecision(2) << budzet;
        cout << "Imię i nazwisko: " << nazwa << " | Budżet: " << stream.str();
        cout << " | ";
        adres.wypisz();
        cout << " | Ulubiony towar: " << ulubiony_towar << "\n";
    }
    virtual bool kup(vector<Sklep*> sklepy) = 0;
};

FanTowaru::~FanTowaru() = default;

bool FanTowaru::dokonajTranzakcji(Sklep* sklep) {
    if (sklep->sprzedajTowar(ulubiony_towar)) {
        budzet -= sklep->podajCeneISztuki(ulubiony_towar).first;
        return true;
    }
    return false;
}

// --- Klasy pochodne od FanSklepu ---

class Maksymalista : public FanSklepu {
private:
    string znajdzNajdrozszyTowar(); 

public:
    Maksymalista(string nazwa, double budzet, Adres wsp, Sklep* s) : FanSklepu(nazwa, budzet, wsp, s) {}
    ~Maksymalista() = default;
        
    bool kup(vector<Sklep*> sklepy) override;
};

class Minimalista : public FanSklepu {
private:
    string znajdzNajtanszyTowar();

public:
    Minimalista(string nazwa, double budzet, Adres wsp, Sklep* s) : FanSklepu(nazwa, budzet, wsp, s) {}
    ~Minimalista() = default;

    bool kup(vector<Sklep*> sklepy) override;
};

class Losowy : public FanSklepu {
protected:
    const int LICZBA_PROB = 3;
    bool runda();

public:
    Losowy(string nazwa, double budzet, Adres wsp, Sklep* s) : FanSklepu(nazwa, budzet, wsp, s) {}
    ~Losowy() = default;

    bool kup(vector<Sklep*> sklepy) override;
};

// --- Klasy pochodne od FanTowaru ---

class Inny : public FanTowaru {
public:
    Inny(string nazwa, double budzet, Adres wsp, string n) : FanTowaru(nazwa, budzet, wsp, n) {}
    ~Inny() = default;

    bool kup(vector<Sklep*> sklepy) override;
};

class Oszczedny : public FanTowaru {
private:
    Sklep* znajdzNajtanszyUlubionyTowar(vector<Sklep*> sklepy);

public:
    Oszczedny(string nazwa, double budzet, Adres wsp, string n) : FanTowaru(nazwa, budzet, wsp, n) {}
    ~Oszczedny() = default;

    bool kup(vector<Sklep*> sklepy) override;
};

class Tradycjonalista : public FanTowaru {    
private:
    double odlEuklidesowa(Adres adres_sklepu) const { return sqrt(pow(adres.getX() - adres_sklepu.getX(), 2) + pow(adres.getY() - adres_sklepu.getY(), 2)); }
    Sklep* znajdzNajbliszySklep(vector<Sklep*> sklepy);

public:
    Tradycjonalista(string nazwa, double budzet, Adres wsp, string n) : FanTowaru(nazwa, budzet, wsp, n) {}
    ~Tradycjonalista() = default;

    bool kup(vector<Sklep*> sklepy) override;
};

// Implementacja metody `kup` w zależności od typu Kupującego:

bool Maksymalista::kup(vector<Sklep*> sklepy) { 
    string towar = ""; 
    if (czySklepIstnieje(sklepy)) { towar = znajdzNajdrozszyTowar(); }
    
    if (towar != "") { 
        if (dokonajTranzakcji(towar)) { return true; }
    }
    return false;
}

string Maksymalista::znajdzNajdrozszyTowar() {  // przeszukanie sklepu pod kątem najdroższego towaru, na który stać Maksymalistę
    double najwyzsza_cena = 0;
    string najdrozszy_towar = "";
    for (Towar& t : ulubiony_sklep->getTowary()) {
        if (czyGoStac(t.getCenaTowaru()) && t.getCenaTowaru() > najwyzsza_cena) {
            najwyzsza_cena = t.getCenaTowaru();
            najdrozszy_towar = t.getNazwa();
        }
    }
    return najdrozszy_towar;
}

bool Minimalista::kup(vector<Sklep*> sklepy) {
    string towar = "";
    if (czySklepIstnieje(sklepy)) { towar = znajdzNajtanszyTowar(); }

    if (towar != "") { 
        if (dokonajTranzakcji(towar)) { return true; }
    }
    return false;
}

string Minimalista::znajdzNajtanszyTowar() {  // przeszukanie sklepu pod kątem najtańszego towaru, na który stać Minimalistę
    double najnizsza_cena = -1;
    string najtanszy_towar = "";
    for (Towar& t : ulubiony_sklep->getTowary()) {
        if ((najnizsza_cena == -1 || najnizsza_cena > t.getCenaTowaru()) && czyGoStac(t.getCenaTowaru())) {
            najnizsza_cena = t.getCenaTowaru();
            najtanszy_towar = t.getNazwa();
        }
    }
    return najtanszy_towar;
}

bool Losowy::kup(vector<Sklep*> sklepy) {
    if (czySklepIstnieje(sklepy)) {
        int proba = 0;
        while (proba < LICZBA_PROB) {
            if (runda()) { return true; }
            proba++;    
        }
    }
    return false;
}

bool Losowy::runda() {
    size_t wielkosc = ulubiony_sklep->getTowary().size();
    auto strzal = rand() % wielkosc;
    vector<Towar> wektor_towarow = ulubiony_sklep->getTowary();
    Towar& towar = wektor_towarow[strzal];

    if (ulubiony_sklep->czyJestTowar(towar.getNazwa()) && czyGoStac(towar.getCenaTowaru())) {
        if (dokonajTranzakcji(towar.getNazwa())) { return true; }
    }
    return false;
}

bool Inny::kup(vector<Sklep*> sklepy) {
    for (Sklep* sklep : sklepy) {
        if (sklep->czyJestTowar(ulubiony_towar) && czyGoStac(sklep->podajCeneISztuki(ulubiony_towar).first)) {
            if (dokonajTranzakcji(sklep)) { return true; }
        }
    }
    return false;
}

bool Oszczedny::kup(vector<Sklep*> sklepy) {
    Sklep* sklep = znajdzNajtanszyUlubionyTowar(sklepy);
    if (sklep != nullptr) {
        if (dokonajTranzakcji(sklep)) { return true; }
    }
    return false;
}

Sklep* Oszczedny::znajdzNajtanszyUlubionyTowar(vector<Sklep*> sklepy) {
    double cena_najnizsza = -1;
    Sklep* wybrany_sklep = nullptr;
    for (Sklep* sklep : sklepy) {
        if (sklep->czyJestTowar(ulubiony_towar)) {
            if ((cena_najnizsza == -1 || sklep->podajCeneISztuki(ulubiony_towar).first < cena_najnizsza) && czyGoStac(sklep->podajCeneISztuki(ulubiony_towar).first)) {
                cena_najnizsza = sklep->podajCeneISztuki(ulubiony_towar).first;
                wybrany_sklep = sklep;
            }
        } 
    }
    return wybrany_sklep;
}

bool Tradycjonalista::kup(vector<Sklep*> sklepy) {  // nie kupują w sklepach, które są w sieci
    Sklep* najblizszy_sklep = znajdzNajbliszySklep(sklepy);
    if (najblizszy_sklep != nullptr) {
        if (dokonajTranzakcji(najblizszy_sklep)) { return true; }
    }
    return false;
}

Sklep* Tradycjonalista::znajdzNajbliszySklep(vector<Sklep*> sklepy) {
    double odleglosc_min = -1, odleglosc = 0;
    Sklep* najblizszy_sklep = nullptr;
    for (Sklep* s : sklepy) {
        if (!s->getCzywSieci()) {
            odleglosc = odlEuklidesowa(s->getAdres());
            if ((odleglosc_min == -1 || odleglosc_min > odleglosc) && s->czyJestTowar(ulubiony_towar) && czyGoStac(s->podajCeneISztuki(ulubiony_towar).first)) {
                odleglosc_min = odleglosc;
                najblizszy_sklep = s;
            }
        }
    }
    return najblizszy_sklep;
}