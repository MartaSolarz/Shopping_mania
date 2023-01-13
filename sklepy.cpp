/*
--- Klasa bazowa JednostkaHandlowa i dziedziczące po niej klasy: Sklep i SiecSklepow ---

Uwagi:
# zgodnie z treścią polecenia sklep ustala ilość i cenę towarów - stąd odpowiednie metody;
# sklep wie czy i w jakiej jest sieci (identyfikowanej poprzez nazwę i marżę);
# przyjęłam założenie, że sieć jest bytem czysto biznesowym (stąd wszystkie operacje dodawania towarów, sprzedaży itd. są po stronie sklepu)
# w tym układzie rola sieci sprowadza się do optymalizacji sprzedaży (np. odpowiada za organizowanie kampanii reklamowych, prowadzi stronę internetową gdzie można porównać i zamówić towary ze sklepów w sieci) stąd otrzymuje marżę, od sprzedanego towaru ze sklepu w sieci

*/ 

#include <string>
#include <vector>

#include "adres.cpp"
#include "towar.cpp"

class JednostkaHandlowa {
protected:
    string nazwa;
    double marza;

public:
    // Konstruktory:
    JednostkaHandlowa() : nazwa(""), marza(0) {}
    JednostkaHandlowa(string nazwa) : nazwa(nazwa), marza(0) {}
    JednostkaHandlowa(string nazwa, double marza) : nazwa(nazwa), marza(marza) {}

    // Reguła trzech:
    JednostkaHandlowa(const JednostkaHandlowa& jh) = default;
    JednostkaHandlowa& operator=(const JednostkaHandlowa& jh) = default;
    ~JednostkaHandlowa() = default;

    // Gettery:
    string getNazwa() const { return nazwa; }
    double getMarza() const { return marza; }

    // Metody:
    virtual bool czyJestTowar(string n) const = 0;
    virtual pair<double, int> podajCeneISztuki(string n) const = 0;
    virtual void wypisz() const = 0;
};

// --- Sklep zwykły ---

class Sklep : public JednostkaHandlowa {
private:
    vector<Towar> towary;
    Adres adres;
    bool czy_w_sieci;
    double marza;
    string nazwa_sieci;

    void wypisz_info() const { 
        if (czy_w_sieci) { cout << "Nazwa sklepu: " << nazwa << " - sklep znajduje się w sieci: " << nazwa_sieci << " | "; 
        } else { cout << "Nazwa sklepu: " << nazwa << " | "; }
        adres.wypisz();
        cout << "\n";
    }

    void wyswietlWszystkieTowary() const {
        for (const Towar& t : towary) { t.wypisz(); }
        cout << "\n";
    }

    double cenaWSieci(Towar& t) { return t.getCenaTowaru() * (1 + marza); }
    double cenaPozaSiecia(Towar& t) { return t.getCenaTowaru() / (1 + marza); }

public:
    // Konstruktory:
    Sklep() : JednostkaHandlowa() {
        adres = Adres(0,0);
        nazwa_sieci = "";
        czy_w_sieci = false;
    }
    Sklep(string nazwa, Adres wsp) : JednostkaHandlowa(nazwa) {
        adres = wsp;
        nazwa_sieci = "";
        czy_w_sieci = false;
    }
    Sklep(string nazwa, Adres wsp, vector<Towar>& wektor_towary) : JednostkaHandlowa(nazwa) {
        adres = wsp;
        nazwa_sieci = "";
        czy_w_sieci = false;
        this->towary = wektor_towary; 
    }

    // Gettery:
    vector<Towar> getTowary() const { return towary; }
    Adres getAdres() const { return adres; }    
    bool getCzywSieci() const { return czy_w_sieci; }

    // Metody:
    bool czyJestTowar(string n) const override;
    pair<double, int> podajCeneISztuki(string n) const override;
    void wypisz() const override;
    
    Sklep& zmienIloscTowaru(const string& n, int ilosc);
    Sklep& zmienCeneTowaru(const string& n, double cena);

    Sklep& dodajTowar(Towar& t);
    bool sprzedajTowar(string n);

    Sklep& wejdzDoSieci(string n, double marza);
    Sklep& wystapZSieci();
};

void Sklep::wypisz() const {
    wypisz_info();
    wyswietlWszystkieTowary();
}

bool Sklep::czyJestTowar(string n) const {
    for (Towar& t : getTowary()) {
        if (t.getNazwa() == n && t.getIloscTowaru() > 0) { return true; }
    }
    return false;
}

pair<double, int> Sklep::podajCeneISztuki(string n) const {
    if (czyJestTowar(n)) {
        for (Towar& t : getTowary()) {
            if (t.getNazwa() == n) { return make_pair(t.getCenaTowaru(), t.getIloscTowaru()); }
        }
    }
    return make_pair(-1,-1);
}

Sklep& Sklep::zmienIloscTowaru(const string& n, int ilosc) {
    for (Towar& t : towary) {
        if (t.getNazwa() == n) { t.setIloscTowaru(ilosc); }
    }
    return *this;
}

Sklep& Sklep::zmienCeneTowaru(const string& n, double cena) {
    for (Towar& t : towary) {
        if (t.getNazwa() == n) { t.setCenaTowaru(cena); }
    }
    return *this;
}

Sklep& Sklep::dodajTowar(Towar& t) {
    bool czy_istnieje = false;
    for (Towar& t2 : towary) {
        if (t.getNazwa() == t2.getNazwa()) {
            czy_istnieje = true;
            break;
        }
    }
    if (!czy_istnieje) { towary.push_back(t); }
    return *this;
}

bool Sklep::sprzedajTowar(string nazwa) {
    int ilosc = 1;
    if (czyJestTowar(nazwa)) { 
        zmienIloscTowaru(nazwa, podajCeneISztuki(nazwa).second - ilosc);
        return true;
    }

    return false;
}

Sklep& Sklep::wejdzDoSieci(string n, double marza_z_sieci) {
    marza = marza_z_sieci;
    nazwa_sieci = n;
    czy_w_sieci = true;
    for (Towar& t : towary) {
        zmienCeneTowaru(t.getNazwa(), cenaWSieci(t));
    }
    return *this;
}

Sklep& Sklep::wystapZSieci() {
    czy_w_sieci = false;
    for (Towar& t : towary) {
        zmienCeneTowaru(t.getNazwa(), cenaPozaSiecia(t));
    }
    marza = 0;
    nazwa_sieci = "";
    return *this;
}

bool operator== (const Sklep& s1, const Sklep& s2) {
    return s1.getNazwa() == s2.getNazwa() && s1.getAdres() == s2.getAdres();
}

// --- Sieć sklepów ---

class SiecSklepow : public JednostkaHandlowa {
private:
    vector<Sklep*> sklepy_w_sieci;

    void wypiszSklepyITowaryWSieci() const {
        for (Sklep* s : getSklepy()) { s->wypisz(); }
        cout << "\n";
    }
    
public:
    // Konstruktory:
    SiecSklepow() = delete;
    SiecSklepow(string nazwa, double marza) : JednostkaHandlowa(nazwa, marza) {}
    
    // Gettery:
    vector<Sklep*> getSklepy() const { return sklepy_w_sieci; }
        
    // Settery:
    void setMarza(double m) { marza = m; }
    
    // Metody:
    bool czyJestTowar(string n) const override;
    pair<double, int> podajCeneISztuki(string n) const override;
    void wypisz() const override;

    bool czySklepNalezyDoSieci(Sklep* s);

    SiecSklepow& przejmijSklep(Sklep* s);
    SiecSklepow& przejmijSiec(SiecSklepow* s);
    SiecSklepow& zmniejszSiecOSklep(Sklep* s);
};

void SiecSklepow::wypisz() const { 
    cout << "Nazwa Sieci: " << nazwa << " | marza: " << to_string(marza) << "\n"; 
    wypiszSklepyITowaryWSieci();
}

bool SiecSklepow::czyJestTowar(string n) const {
    for (Sklep* s : getSklepy()) {
        if (s->czyJestTowar(n)) { return true; }
    }
    return false;
}

pair<double, int> SiecSklepow::podajCeneISztuki(string n) const {
    if (czyJestTowar(n)) {
        for (Sklep* s : getSklepy()) {
            if (s->czyJestTowar(n)) {
                return s->podajCeneISztuki(n);
            }
        }
    }
    return make_pair(-1,-1);
}

bool SiecSklepow::czySklepNalezyDoSieci(Sklep* s) {
    for (Sklep* sklep : sklepy_w_sieci) {
        if (sklep == s) { return true; }
    }
    return false;
} 

SiecSklepow& SiecSklepow::przejmijSklep(Sklep* s) {
    if (!czySklepNalezyDoSieci(s)) { 
        s->wejdzDoSieci(nazwa, marza);
        sklepy_w_sieci.push_back(s); 
    }
    return *this;
}

SiecSklepow& SiecSklepow::przejmijSiec(SiecSklepow* s) {
    if (this != s) {
        for (Sklep* sklep : s->getSklepy()) {
            s->zmniejszSiecOSklep(sklep);
            sklep->wystapZSieci();
            przejmijSklep(sklep);
        }
        s->getSklepy().clear();
    }
    return *this;
}

SiecSklepow& SiecSklepow::zmniejszSiecOSklep(Sklep* s) {
    for (auto it = sklepy_w_sieci.begin(); it != sklepy_w_sieci.end(); ++it) {
        if (*it == s) {
            sklepy_w_sieci.erase(it);
            s->wystapZSieci();
            break;
        }
    }
    return *this;
}

bool operator== (const SiecSklepow& s1, const SiecSklepow& s2) { return (s1.getNazwa() == s2.getNazwa() && s1.getMarza() == s2.getMarza() && s1.getSklepy() == s2.getSklepy()); }
