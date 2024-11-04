#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <cctype>
#include <set>

using namespace std;


vector<tuple<int, string, string>> citeste_fisier_csv(const string& nume_fisier) {
    vector<tuple<int, string, string>> date_joc;
    ifstream fisier(nume_fisier);
    string linie;


    if (!fisier.is_open()) {
        cerr << "Eroare: Nu s-a putut deschide fisierul " << nume_fisier << endl;
        return date_joc;
    }

    while (getline(fisier, linie)) {
        stringstream ss(linie);
        int cod;
        string cuvant_mascat, cuvant_complet;


        string temp;
        if (getline(ss, temp, ';')) {
            try {
                cod = stoi(temp);
            } catch (const invalid_argument& e) {
                cerr << "Eroare: Cod invalid in linia: " << linie << endl;
                continue;
            }
        }

        if (!getline(ss, cuvant_mascat, ';') || !getline(ss, cuvant_complet, ';')) {
            cerr << "Eroare: Linia \"" << linie << "\" nu are formatul corect.\n";
            continue;
        }

        date_joc.emplace_back(cod, cuvant_mascat, cuvant_complet);
    }
    fisier.close();
    return date_joc;
}


bool este_complet(const string& cuvant_mascat) {
    return cuvant_mascat.find('*') == string::npos;
}


int ghiceste_cuvant_optimizat(const string& cuvant_mascat, const string& cuvant_complet) {
    string cuvant_actual = cuvant_mascat;
    int incercari_folosite = 0;


    set<char> litere_ghicite;


    string vocale = "aeiou";

    for (char litera : vocale) {
        bool gasit = false;
        for (size_t i = 0; i < cuvant_complet.size(); i++) {
            if (tolower(cuvant_complet[i]) == litera) {
                cuvant_actual[i] = cuvant_complet[i];
                gasit = true;
            }
        }
        if (gasit) incercari_folosite++;
        litere_ghicite.insert(litera);
        if (este_complet(cuvant_actual)) {
            return incercari_folosite;
        }
    }


    string consoane_frecvente = "rtnslcdpm";

    for (char litera : consoane_frecvente) {
        if (litere_ghicite.find(litera) != litere_ghicite.end()) continue;
        bool gasit = false;
        for (size_t i = 0; i < cuvant_complet.size(); i++) {
            if (tolower(cuvant_complet[i]) == litera) {
                cuvant_actual[i] = cuvant_complet[i];
                gasit = true;
            }
        }
        if (gasit) incercari_folosite++;
        litere_ghicite.insert(litera);
        if (este_complet(cuvant_actual)) {
            return incercari_folosite;
        }
    }


    for (char litera : cuvant_complet) {
        if (litere_ghicite.find(tolower(litera)) != litere_ghicite.end()) continue;
        bool gasit = false;
        for (size_t i = 0; i < cuvant_complet.size(); i++) {
            if (tolower(cuvant_complet[i]) == tolower(litera)) {
                cuvant_actual[i] = cuvant_complet[i];
                gasit = true;
            }
        }
        if (gasit) incercari_folosite++;
        litere_ghicite.insert(tolower(litera));
        if (este_complet(cuvant_actual)) {
            return incercari_folosite;
        }
    }

    return incercari_folosite;
}

int main() {

    vector<tuple<int, string, string>> date_joc = citeste_fisier_csv("cuvinte_de_verificat.csv");


    if (date_joc.empty()) {
        cerr << "Nu s-au gasit date de joc in fisier!" << endl;
        return 1;
    }

    int incercari_totale = 0;


    for (const auto& [cod, cuvant_mascat, cuvant_complet] : date_joc) {
        int incercari_folosite = ghiceste_cuvant_optimizat(cuvant_mascat, cuvant_complet);
        incercari_totale += incercari_folosite;
    }


    cout << "Numar total de incercari: " << incercari_totale << endl;

    return 0;
}
