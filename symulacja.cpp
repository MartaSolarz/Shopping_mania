/*
--- SYMULACJA PROGRAMU ZALICZENIOWEGO ---
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <random>
#include <cstdio>

#include "miasto.cpp"


Miasto& stworzenieMiasta() {
    string nazwaMiasta;
    cout << "Podaj nazwę miasta: ";
    cin >> nazwaMiasta;
    static Miasto miasto(nazwaMiasta);
    return miasto;
}

void wypiszStanSymulacjiSklepy(Miasto& miasto) {
    cout << "SKLEPY: \n";
    for (Sklep* s : miasto.getSklepy()) { s->wypisz(); }
}

void wypiszStanSymulacjiKupujacy(Miasto& miasto) {
    cout << "KUPUJĄCY: \n";
    for (Kupujacy* k : miasto.getKupujacy()) { k->wypisz(); }
}

void wypiszStanSymulacji(Miasto& miasto) {
    wypiszStanSymulacjiSklepy(miasto);
    wypiszStanSymulacjiKupujacy(miasto);
    cout << "\n";
}

void zacznijZakupy(Miasto& miasto) {
    int ile_tur = 0;
    string input;
    while (ile_tur <= 0) {
        cout << "Ile tur zakupów? ";
        cin >> input;
        try { ile_tur = stoi(input); if (ile_tur <= 0) { cout << "Podaj liczbę całkowitą większą od zera.\n"; ile_tur = 0; } } 
        catch (const invalid_argument& e) { cout << "Podaj liczbę całkowitą.\n"; }
    }

    for (int i=0; i<ile_tur; i++) { miasto.tura(); }
}

void kontynuuj() {
    char start;
    cout << "Naciśnij dowolny klawisz aby kontynuować: ";
    cin >> start;
}

void symulacja() {
    cout << "WITAJ W ZAKUPOMANII! \n\n";
    cout << setprecision(3);

    // KROK 1: utworzenie miasta i sklepów zwykłych z towarami
    Miasto miasto = stworzenieMiasta();
    
    cout << "\nTwoje miasto to: " << miasto.getNazwa() << "\n\n";

    Sklep s1("Sklep A", Adres(12, 3));
    Sklep s2("Sklep B", Adres(3, 6));
    Sklep s3("Sklep C", Adres(9, 8));
    Sklep s4("Sklep D", Adres(7, 1));
    Sklep s5("Sklep E", Adres(5, 9));
    Sklep s6("Sklep F", Adres(14, 3));

    for (Sklep* s : {&s1, &s2, &s3, &s4, &s5, &s6}) { miasto.dodajSklep(s); }

    Towar t1("smartfon");
    Towar t2("aparat");
    Towar t3("telewizor");
    Towar t4("konsola do gier");
    Towar t5("tablet");
    Towar t6("czapka zimowa");
    Towar t7("sukienka");
    Towar t8("kubek termiczny");
    Towar t9("torba podróżna");
    Towar t10("koszula");
    Towar t11("książka");
    Towar t12("karnet na siłownię");

    mt19937 gen(random_device{}());
    uniform_int_distribution<int> cena_elektronika_dist(2000, 4500);
    uniform_int_distribution<int> cena_inne(40, 400);
    uniform_int_distribution<int> ilosc_dist(10, 30);
    
    for (Towar t : {t1, t2, t3}) { s1.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_elektronika_dist(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t1, t4, t5}) { s2.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_elektronika_dist(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t6, t7, t8, t10, t12}) { s3.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_inne(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t11, t12, t8, t9}) { s4.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_inne(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t1, t2, t4, t5}) { s5.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_elektronika_dist(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t6, t11, t12, t10, t9}) { s6.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_inne(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }

    cout << "...Sukces! - Utworzono sklepy, dodano do nich produkty i powiadomino odpowiedni organ w mieście o założeniu nowego sklepu.\n\nSklepy w mieście " << miasto.getNazwa() << " to:\n\n";
    
    wypiszStanSymulacjiSklepy(miasto);

    kontynuuj();

    // KROK 2: stworzenie dwóch sieci
    cout << "\n...z ostatniej chwili - W mieście zostaną otworzone dwie nowe sieci sklepów!\n\n";

    SiecSklepow siec1("Sieć A", 0.1);
    SiecSklepow siec2("Sieć B", 0.15);

    siec1.przejmijSklep(&s1).przejmijSklep(&s3);
    siec2.przejmijSklep(&s2).przejmijSklep(&s4);

    cout << "Miasto zaktualizowało dane o sklepach i sieciach.\n\n";

    wypiszStanSymulacjiSklepy(miasto);

    kontynuuj();

    // KROK 3: przejęcie jednej sieci przez drugą
    cout << "...ups! - Wygląda na to, że sieć pierwszego przedsiębiorcy przejmie sieć drugiego!\n\n";

    siec1.przejmijSiec(&siec2);

    siec1.wypisz();
    siec2.wypisz();

    kontynuuj();

    // KROK 4: utworzenie klientów różnych rodzajów
    cout << "...a oto zbliża się pora przedświąteczna i mieszkańcy miasta zakładają konta członkowskie zrzeszające zakupomanów w mieście " << miasto.getNazwa() << ".\n\n";

    Maksymalista k1("Jan Kowalski", 3500.00, Adres(14,9), &s1);
    Minimalista k2("Marek Nowak", 380.99, Adres(8,17), &s6);
    Losowy k3("Agata Kowalczyk", 599.87, Adres(-4,2), &s3);
    Oszczedny k4("Kacper Zalewski", 1308.00, Adres(-9,-6), "koszula");
    Tradycjonalista k5("Maria Lisowska", 3890.00, Adres(16,-8), "smartfon");
    Inny k6("Julia Adamska", 1540.00, Adres(5,-2), "kubek termiczny"); 

    vector<Kupujacy*> k {&k1, &k2, &k3, &k4, &k5, &k6};
    for (auto el: k) { miasto.dodajKupujacego(el); }
  
    // KROK 5: wypisanie stanu symulacji:
    cout << "Obecny stan sklepów i kupujących przed nadejściem zakupowej gorączki:\n\n";
    wypiszStanSymulacji(miasto);
    kontynuuj();
    
    // KROK 6: wywołanie metody kupującej:
    zacznijZakupy(miasto);

    // KROK 7: wypisanie stanu symulacji: 
    cout << "Obecny stan sklepów i kupujących po zakupowej gorączce:\n\n";
    wypiszStanSymulacji(miasto);
}

int main() {
    symulacja();
    return 0;
}