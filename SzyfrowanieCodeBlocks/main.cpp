#include "szyfrowanie.h"

int main(void) {
    std::ofstream plik("tekstowy.txt", std::ios::app);
    if (!plik.is_open()) {
             std::cout << "\nBlad otwarcia pliku" << std::endl;
        return -1;
    }

    std::cout << "\nPrzed mixem:" << znaki_do_95 << std::endl;
    std::cout << "-----------------------------------------------------------------------------------------------------------------------" << std::endl;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dis(10000000, 99999999);
    std::uniform_int_distribution<int> dismacierz(0, 10);

    for (int i = 0; i < 20000; i++) {
        int numer = dis(gen);
        std::cout << "\n\nWygenerowana liczba: " << numer;

        plik<<"\nWygenerowana liczba: " << numer << std::endl;

        unsigned seed_macierz = std::chrono::system_clock::now().time_since_epoch().count();
        std::vector<std::vector<int>> macierz = generuj_macierz(gen, dismacierz);



        Szyfrowanie* liczba = new Szyfrowanie(numer);

        std::string zaszyfrowana = liczba->Szyfruj(macierz);

        zapisz_macierz_w_pliku(macierz, "tekstowy.txt");

        plik<<"Zaszyfrowana liczba:"<<zaszyfrowana<<std::endl;
        std::cout << "\nWygenerowana macierz: \n";
        wypisz_macierz(macierz);

        std::cout << "Zaszyfrowana liczba:" << zaszyfrowana << std::endl;

        int odszyfrowana = liczba->Deszyfrowanie();
        std::cout << "\nOdszyfrowana liczba:" << odszyfrowana << std::endl;
        plik<<"Odszyforwana liczba:"<<odszyfrowana<<std::endl;
        std::cout << "-----------------------------------------------------------------------------------------------------------------------" << std::endl;

        zaszyfrowana = "";
        macierz.clear();
        delete liczba;
    }

     plik.close();
    return 0;
}
