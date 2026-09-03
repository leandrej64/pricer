#pragma once // Evite d'inclure ce fichier plusieurs fois lors de la compilation
#include <string>

class MonteCarlo {
public:

    int iterationNumber;
    std::string text;
    
    // Le Constructeur
    // On passe la string par "référence constante" (const &) pour éviter de la copier en mémoire
    MonteCarlo(const std::string& Text);

    // Les Méthodes pour modifier l'état de l'objet
    void Display() const;

};