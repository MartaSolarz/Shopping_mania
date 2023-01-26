/*
--- Symulacja prezentująca zadanie ---
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
    cout << "Podaj dowolny znak aby kontynuować: ";
    cin >> start;
}

void symulacja() {
    cout << "WITAJ W ZAKUPOMANII! \n\n";
    cout << setprecision(3);

    // KROK 1: utworzenie miasta i sklepów zwykłych z towarami
    Miasto miasto = stworzenieMiasta();
    
    cout << "\nTwoje miasto to: " << miasto.getNazwa() << "\n\n";

    Sklep s1("NeoNET", Adres(12, 3));
    Sklep s2("Media Expert", Adres(3, 6));
    Sklep s3("Niezbędnik", Adres(9, 8));
    Sklep s4("Empik", Adres(7, 1));
    Sklep s5("Media Markt", Adres(5, 9));
    Sklep s6("Mydło i powidło", Adres(14, 3));
    Sklep s7("U Zbyszka", Adres(13, 15));
    Sklep s8("Smyk", Adres(3, 17));

    for (Sklep* s : {&s1, &s2, &s3, &s4, &s5, &s6, &s7, &s8}) { miasto.dodajSklep(s); }

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
    Towar t13("pluszowy miś");
    Towar t14("zestaw LEGO");
    Towar t15("zastawa kuchenna");
    Towar t16("okulary VR");
    Towar t17("słuchawki");

    mt19937 gen(random_device{}());
    uniform_int_distribution<int> cena_elektronika_dist(2000, 4500);
    uniform_int_distribution<int> cena_inne(40, 400);
    uniform_int_distribution<int> ilosc_dist(10, 30);
    
    for (Towar t : {t1, t2, t3,  t16, t17}) { s1.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_elektronika_dist(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t1, t2, t4, t5, t16}) { s2.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_elektronika_dist(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t6, t7, t8, t10, t12}) { s3.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_inne(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t11, t12, t8, t9, t7}) { s4.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_inne(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t1, t2, t4, t5, t17, t3}) { s5.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_elektronika_dist(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t6, t11, t12, t10, t9}) { s6.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_inne(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t13, t14, t7, t6, t10, t12}) { s7.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_inne(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }
    for (Towar t : {t9, t15, t14, t13, t11}) { s8.dodajTowar(t).zmienCeneTowaru(t.getNazwa(), cena_inne(gen)).zmienIloscTowaru(t.getNazwa(), ilosc_dist(gen)); }

    cout << "...Sukces! - Utworzono sklepy, dodano do nich produkty i powiadomino odpowiedni organ w mieście o założeniu nowego sklepu.\n\nSklepy w mieście " << miasto.getNazwa() << " to:\n\n";
    
    wypiszStanSymulacjiSklepy(miasto);

    // KROK 2: stworzenie trzech sieci
    cout << "\n...z ostatniej chwili - W mieście zostaną otworzone trzy nowe sieci sklepów!\n\n";

    kontynuuj();

    SiecSklepow siec1("Sieć 'Niezawodna'", 0.1);
    SiecSklepow siec2("Sieć 'To czego szukasz'", 0.15);
    SiecSklepow siec3("Sieć 'Postaw na jakość'", 0.22);

    siec1.przejmijSklep(&s1).przejmijSklep(&s3);
    siec2.przejmijSklep(&s2).przejmijSklep(&s4);
    siec3.przejmijSklep(&s6).przejmijSklep(&s7).przejmijSklep(&s8);

    cout << "Miasto zaktualizowało dane o sklepach i sieciach.\n\n";

    wypiszStanSymulacjiSklepy(miasto);


    // KROK 3: przejęcie jednej sieci przez drugą i sklep występuje z sieci
    cout << "\n...ups! - Wygląda na to, że sieć 'Niezawodna' przejmie sieć 'To czego szukasz'! Ponadto Sklep 'U Zbyszka' zdecydował się wystąpić ze swojej sieci!\n\n";

    kontynuuj();

    siec1.przejmijSiec(&siec2);

    s7.wystapZSieci();

    wypiszStanSymulacjiSklepy(miasto);

    kontynuuj();

    // KROK 4: utworzenie klientów różnych rodzajów
    cout << "...a oto zbliża się pora przedświąteczna i mieszkańcy miasta zakładają konta członkowskie zrzeszające zakupomanów w mieście " << miasto.getNazwa() << ".\n\n";

    kontynuuj();

    Maksymalista k10("Robert Lewandowski", 3500.00, Adres(14,9), &s1), k11("Arkadiusz Milik", 450.00, Adres(4,5), &s8);
    Minimalista k20("Anna Lewandowska", 380.99, Adres(8,17), &s6), k21("Wojciech Szczęsny", 300.00, Adres(19,-2), &s4);
    Losowy k30("Viki Gabor", 599.87, Adres(-4,2), &s3), k31("Sylwia Grzeszczak", 2700.00, Adres(-8,-2), &s5);
    Oszczedny k40("Dawid Podsiadło", 2800.00, Adres(-9,-6), "smartfon"), k41("Maryla Rodowicz", 199.99, Adres(1,10), "książka");
    Tradycjonalista k50("Wojciech Cejrowski", 590.00, Adres(16,-8), "koszula"), k51("Sara James", 1500.00, Adres(2,8), "sukienka");
    Inny k60("Kamil Stoch", 3540.00, Adres(5,-2), "tablet"), k61("Roksana Węgiel", 950.5, Adres(-2,-9), "torba podróżna"); 

    vector<Kupujacy*> k {&k10, &k11, &k20, &k21, &k30, &k31, &k40, &k41, &k50, &k51, &k60, &k61};
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