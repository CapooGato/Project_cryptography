#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>


extern std::string znaki_do_95;
class Szyfrowanie {
private:
    int numer;
    unsigned int kluczyk = { 0 };
    std::vector<int> torba_na_klucze;
    std::string L95 = "";
    std::string znaki_do_mieszania = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

    void Zamaz();
    std::string Dec_to_95();
    void Sprawdz_macierz(std::vector<std::vector<int>>& macierz);
    void Podziel_na_czesci();

public:
    Szyfrowanie(int numer) { this->numer = numer; }
    int Deszyfrowanie();
    std::string  Szyfruj(std::vector<std::vector<int>>& macierz);


};

void reverse_string(std::string& s);
void wypisz_macierz(std::vector<std::vector<int>>const& macierz);
int powrot_na_10(std::string& slowo);
std::vector<std::vector<int>> generuj_macierz(std::mt19937& gen, std::uniform_int_distribution<int>& dis);
void zapisz_macierz_w_pliku(const std::vector<std::vector<int>>& macierz, const std::string& nazwaPliku);

