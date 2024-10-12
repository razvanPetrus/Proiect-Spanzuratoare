#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

// Functie pentru citirea fisierului CSV
vector<tuple<int, string, string>> citeste_fisier_csv(const string& nume_fisier) {
    vector<tuple<int, string, string>> date_joc;
    ifstream fisier(nume_fisier);
    string linie;

    // Verificare daca fisierul s-a deschis
    if (!fisier.is_open()) {
        cerr << "Eroare: Nu s-a putut deschide fisierul " << nume_fisier << endl;
        return date_joc; // Returneaza vectorul gol
    }

    while (getline(fisier, linie)) {
        stringstream ss(linie);
        int cod;
        string cuvant_mascat, cuvant_complet;

        // Citeste codul, cuvantul mascat si cuvantul complet
        string temp;
        if (getline(ss, temp, ';')) {
            try {
                cod = stoi(temp);
            } catch (const invalid_argument& e) {
                cerr << "Eroare: Cod invalid in linia: " << linie << endl;
                continue; // Sarim la urmatoarea linie
            }
        }

        if (!getline(ss, cuvant_mascat, ';') || !getline(ss, cuvant_complet, ';')) {
            cerr << "Eroare: Linia \"" << linie << "\" nu are formatul corect.\n";
            continue; // Sarim la urmatoarea linie
        }

        date_joc.emplace_back(cod, cuvant_mascat, cuvant_complet);
    }
    fisier.close(); // Inchide fisierul dupa citire
    return date_joc;
}

// Functie pentru a verifica daca cuvantul mascat este complet
bool este_complet(const string& cuvant_mascat) {
    return cuvant_mascat.find('*') == string::npos;
}

// Functie pentru a afisa cuvantul mascat
void afiseaza_cuvant(const string& cuvant_mascat) {
    cout << "Cuvantul mascat: " << cuvant_mascat << endl;
}

// Functie principala a jocului
void joc_hangman(const string& cuvant_mascat, const string& cuvant_complet) {
    string cuvant_actual = cuvant_mascat;
    int incercari_ramase = 7; // numar de incercari

    cout << "Incepe jocul! Ai " << incercari_ramase << " incercari.\n";

    while (incercari_ramase > 0 && !este_complet(cuvant_actual)) {
        afiseaza_cuvant(cuvant_actual);

        char litera;
        cout << "Introdu o litera: ";
        cin >> litera;
        litera = tolower(litera); // Normalizeaza litera

        bool gasit = false;
        // Verifica daca litera se afla in cuvantul complet
        for (size_t i = 0; i < cuvant_complet.size(); i++) {
            if (tolower(cuvant_complet[i]) == litera) {
                cuvant_actual[i] = cuvant_complet[i];
                gasit = true;
            }
        }

        if (!gasit) {
            incercari_ramase--;
            cout << "Litera nu este in cuvant! Incercari ramase: " << incercari_ramase << endl;
        }
    }

    if (este_complet(cuvant_actual)) {
        cout << "Felicitari! Ai ghicit cuvantul: " << cuvant_complet << endl;
    } else {
        cout << "Ai pierdut! Cuvantul era: " << cuvant_complet << endl;
    }
}

int main() {
    // Citeste cuvintele din fisierul CSV
    vector<tuple<int, string, string>> date_joc = citeste_fisier_csv("cuvinte_de_verificat.csv");

    // Verificare daca datele au fost citite
    if (date_joc.empty()) {
        cerr << "Nu s-au gasit date de joc in fisier!" << endl;
        return 1; // Terminarea programului cu eroare
    }

    // Parcurge fiecare cuvant din lista si joaca jocul
    for (const auto& [cod, cuvant_mascat, cuvant_complet] : date_joc) {
        cout << "Cod: " << cod << endl;
        joc_hangman(cuvant_mascat, cuvant_complet);
        cout << endl; // Linie de separare intre jocuri
    }

    return 0;
}
