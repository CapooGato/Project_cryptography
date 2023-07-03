#include "szyfrowanie.h"

const int N = 4;

std::string znaki_do_95 = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

void wypisz_macierz(std::vector<std::vector<int>>const& macierz) {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            std::cout << "  " << macierz[x][y];
        }
        std::cout << std::endl;
    }
}

void reverse_string(std::string& s) {
    size_t dlugosc = s.length();
    for (int i = 0; i < dlugosc / 2; i++)
        std::swap(s[i], s[dlugosc - 1 - i]);
}

std::string Szyfrowanie::Dec_to_95() {
    std::ofstream plik("tekstowy.txt", std::ios::app);

    if (!plik.is_open()) {
             std::cout << "\nBlad otwarcia pliku" << std::endl;
        return "";
    }
    std::string wynik = "";
    while (this->numer > 0)
    {
        int reszta = this->numer % 95;
        wynik = znaki_do_95[reszta] + wynik;      //wyszla reszta "1" znaki = 1 + "" potem wyszlo = -> 0+1+"" etc
        this->numer /= 95;
    }
    this->L95 = wynik;
    plik<<"Liczba w systemie 95:"<<this->L95<<std::endl;
    return wynik;
}

std::vector<std::vector<int>> generuj_macierz(std::mt19937& gen, std::uniform_int_distribution<int>& dis) {
    std::vector<std::vector<int>> macierz(N, std::vector<int>(N, 0));
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            macierz[x][y] = dis(gen);
        }
    }
    return macierz;
}


std::string Szyfrowanie::Szyfruj(std::vector<std::vector<int>>& macierz) {
    Sprawdz_macierz(macierz);
    this->L95 = Dec_to_95();
    std::cout << "\nLiczba w systemie 95:" << this->L95 << std::endl;

    Podziel_na_czesci();
    return this->L95;
}

int powrot_na_10(std::string& slowo) {
    int wynik_w_10 = 0;
    int potega = 0;

    for (int i = slowo.length() - 1; i >= 0; i--) {
        char digit = slowo[i];
        int waratoscDigit = znaki_do_95.find(digit);
        wynik_w_10 += waratoscDigit * pow(95, potega);
        potega++;
    }
    return wynik_w_10;
}


/*Suma wn, suma przekatnej, ilosc liczb parszystych nieparzystych*/
void Szyfrowanie::Sprawdz_macierz(std::vector<std::vector<int>>& macierz) {
    int dodaj = 0;
    short suma_w_wierszach[N] = { 0 };
    short suma_przekatnej = 0;
    short liczba_parzystych = 0;
    short liczba_nieparzystych = 0;

    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            suma_w_wierszach[x] += macierz[x][y];
            if (macierz[x][y] % 2 == 0) liczba_parzystych += 1;
            else liczba_nieparzystych += 1;
        }
        suma_przekatnej += macierz[x][x]; //suma przekatnej lewo gora, do prawo dol
        if (suma_w_wierszach[x] % 2 == 0) dodaj += 5;
        else dodaj += 10;
    }

    std::srand(time(NULL));
    int skalar = rand() % 100 + 1;
    int przypadkowa = rand()%40+11;

    dodaj += suma_przekatnej + liczba_parzystych * skalar + liczba_nieparzystych * skalar + skalar;
    //jakby nam wyszlo poza zakres
    if (dodaj >= 94) dodaj %= 94;
    if (dodaj == 0) dodaj = przypadkowa;
    this->kluczyk += dodaj;
}

void Szyfrowanie::Podziel_na_czesci() {
    //Dlugosc czesci liczymy
     std::ofstream plik("tekstowy.txt", std::ios::app);
    if (!plik.is_open()) {
             std::cout << "\nBlad otwarcia pliku" << std::endl;
        return ;
    }

    size_t dlPart = std::floor((this->znaki_do_mieszania.length()-1) / (this->kluczyk) );
    std::vector<std::string> parts(this->kluczyk);

    unsigned int i = 0;
    for (; i < this->kluczyk - 1; i++) {
        parts[i] = this->znaki_do_mieszania.substr(dlPart * i, dlPart);
    }
    parts[i] = this->znaki_do_mieszania.substr(i * dlPart);
    std::string pomocniczy = "";

    int l;
    int r;
    l = 0;
    r = this->kluczyk - 1;
    while (l <= r)
    {
        if (l == r) {
            pomocniczy += parts[l];
            break;
        }
        else {
            pomocniczy += parts[r] + parts[l];  //Np part[4] wyjdzie 3 + 0+ 2 +1
        }
        l++;
        r--;
    }

    this->znaki_do_mieszania = pomocniczy;
    plik<<"Pomieszana tablica:\n"<<this->znaki_do_mieszania<<"\n";
    pomocniczy = "";    //Czyscimy pomocniczny string

    Zamaz();
    parts.clear();
    dlPart = 0;
    std::cout << "\nPo mixie:" << this->znaki_do_mieszania << std::endl;
}

int Szyfrowanie::Deszyfrowanie() {
    std::ofstream plik("tekstowy.txt", std::ios::app);

    if (!plik.is_open()) {
             std::cout << "\nBlad otwarcia pliku" << std::endl;
        return 0;
    }

    std::string pomoc = "";   //Pomocniczy string do odszyfrowania
    for (unsigned int c = 0; c < this->L95.length(); c++) {
        pomoc += this->L95[c] + this->torba_na_klucze[c];
    }

    int m = powrot_na_10(pomoc);
     plik << "Klucze:\n";
    for (const auto& klucz : this->torba_na_klucze) {
    plik << klucz << " ";
    }
plik<<std::endl;

this->L95 = "";
    return m;
}

//Torba na klucze
void Szyfrowanie::Zamaz() {

    torba_na_klucze.clear();
       std::ofstream plik("tekstowy.txt", std::ios::app);


    //jesli klucz jest parzysty to kodujemy od przodu
    if (this->kluczyk % 2 == 0) {
        for (unsigned int k = 0; k < this->L95.length(); k++) {
            this->torba_na_klucze.push_back(L95[k] - znaki_do_mieszania[k]);
            this->L95[k] = znaki_do_mieszania[k];
        }
    }
    else {   //jesli jest nieparzysty to od tylu
        for (unsigned int a = 0, k = this->znaki_do_mieszania.length() - 1; a < this->L95.length(); k--, a++) {
            this->torba_na_klucze.push_back(L95[a] - znaki_do_mieszania[k]);
            this->L95[a] = znaki_do_mieszania[k];
        }
    }

}

void zapisz_macierz_w_pliku(const std::vector<std::vector<int>>& macierz, const std::string& nazwaPliku){
 std::ofstream plik("tekstowy.txt", std::ios::app);

    if (!plik.is_open()) {
             std::cout << "\nBlad otwarcia pliku" << std::endl;
        return ;
    }

    plik<<"Wylosowana macierz:\n";
    if (plik.is_open()) {
        for (const auto& wiersz : macierz) {
            for (const auto& element : wiersz) {
                plik << element << " "; // Wypisanie elementu macierzy do pliku
            }
            plik << std::endl; // Zapis nowej linii po kazdym wierszu
        }
        plik.close();
    } else {
        std::cout << "Nie mo¿na otworzyæ pliku." << std::endl;
    }
}
