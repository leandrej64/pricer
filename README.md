# Projet de couverture de produits dérivés

**Avertissement**. Il est strictement interdit de:

- Cloner ce dépôt ou de copier son contenu sur un dépôt public.
- Transférer le code ou la documentation fournis à un outil d'IA générative pour essayer d'engendrer automatiquement une solution au projet.

Ce projet a pour but de développer un outil de valorisation et de couverture de produits financiers en C++ en mettant en application d'une part les principes de conception et de programmation étudiés dans les cours de 2A et d'autre part la théorie de Black Scholes présentée dans le cours _Introduction aux Produits Dérivés_.

## Projet

- Sujet : [pricer.pdf](pricer.pdf)

### Grandes étapes du projet

1. Du formalisme mathématique à la conception informatique
    - Comment représenter le modèle mathématique d’un point de vue informatique ?
    - A quoi correspondent les différents paramètres du modèle mathématique ?
    - De quelles fonctionnalités a-t-on besoin ?
1. Proposer une architecture pour votre outil.
1. Proposer des tests pour les différentes fonctionnalités.
1. Implémenter les différents composants : simulation stochastique, moteur Monte-Carlo, ...
    - Penser à tester séparément les différentes fonctionnalités au fur et à mesure.
    - Comment optimiser le code ?
    - Ne pas hésiter à refactoriser le code.

### Fonctionnalités à implémenter

- Calcul du prix de l'option en 0
- Calcul du prix de l'option en `t`  : votre classe `Model` devra implémenter une méthode

    ```cpp
    void asset(const PnlMat *past, double t, PnlMath *path, PnlRng *rng)
    ```

    où
  - `past` est une matrice contenant les valeurs $`(s_{t_0}, \cdots, s_{t_i}, s_t)`$
  - `t`est la date à laquelle le prix est calculé
  - `path` est une matrice de taille $`(N+1) \times d`$ contenant en sortie une simulation conditionnelle des sous-jacents
  - `rng` est le générateur de nombres aléatoires.

- Calcul du delta de l'option en `t`
- Calcul du portefeuille de couverture et du P&L

## Organisation

Le projet devra être réalisé par équipes de 4 en C++.

Des fichiers de données pour tester votre code sont disponibles dans le répertoire [data](data/). Le champs `time` indique un temps de calcul. Pour comparer des temps de calcul, il convient de compiler votre code en mode `Release` (voir [ici](#chaîne-de-compilation)).
**Les lignes std\_dev ou standard deviation correspondent à l'écart type de l'estimateur Monte Carlo associé. Avec les notations du sujet, cela correspond à $\xi\_M / \sqrt{M}$.**

## Instructions pour le rendu

### Arborescence du rendu (A respecter scrupuleusement)

Le projet sera rendu sous la forme d'une archive au format `.tar.gz` dont le nom sera construit sur le format `Equipe_i.tar.gz` où `i` est le numéro de l'équipe. Cette archive créera à l'extraction l'arborescence suivante (**à respecter scrupuleusement**)

```
Equipe_i/
   |
   |---- AUTHORS
   |---- CMakeLists.txt
   |---- README
   |---- src/
          |--- *.cpp
          |--- *.h, *.hpp
```

Vous pouvez vérifier la structure de votre archive, l'extraire et la compiler en utilisant le script [CheckArchive.py](CheckArchive.py)

```sh
python ./CheckArchive.py --check [--extract --destdir="chemin ou extraire l'archive" --build --pnldir="chemin vers pnl/build"] "chemin vers Equipe_i.tar.gz"
```

Si vous omettez la partie entre `[...]`, seule la structure de l'archive est vérifiée.

### Les exécutables

La compilation créera 2 exécutables différents pour le calcul des prix en 0 et pour la couverture. **Les 2 exécutables doivent pouvoir être compilés sur les machines de l'école.**

- Calcul du prix et des delta en zéro avec les déviations standards des estimateurs correspondant et affichage sur la console en utilisant

  ```cpp
  PricingResults res(prix, prix_std_dev, delta, delta_std_dev);
  std::cout << res << std::endl;
  ```

  Votre exécutable sera invoqué par la commande `./price0 <data_input.json>`

- Calcul des prix et de la couverture à chaque pas de rebalancement de la couverture

    ```cpp
  // jsonParams est l'objet nlohmann::json obtenu à partir de params.json
  Portfolio hedgingPortfolio(jsonParams, monteCarlo);
  // calculer le portefeuille de couverture
  // ....
  nlohmann::json jsonPortfolio = hedgingPortfolio.positions;
  std::ofstream ifout(argv[3], std::ios_base::out);
  if (!ifout.is_open()) {
      std::cout << "Unable to open file " << argv[3] << std::endl;
      std::exit(1);
  }
  ifout << jsonPortfolio.dump(4);
  ifout.close(); // Required to make sure that flush is called
  ```

  Votre exécutable sera invoqué par la commande `./hedge <market_file.txt> <data_input.json> <output_portfolio.json>`

  Attention, vos exécutables ne doivent rien afficher d'autre.

## Tests automatiques

Pour vérifier que votre code fonctionne correctement avec les tests automatiques utilisés pour la correction des projets, télécharger le script Python [testForPCPD.py](testForPCPD.py) et le répertoire [data](data) contenant des fichiers de données et les résultats attendus.

Pour tester vos prix à l'instant 0

```sh
python ./testForPCPD.py --price --exec="chemin vers price0" --datadir="chemin vers le répertoire data" --outdir="répertoire de sortie"
```

Un timeout de 1 minute est appliqué au calcul du prix et des deltas en 0.

```sh
python ./testForPCPD.py --hedge --exec="chemin vers hedge" --datadir="chemin vers le répertoire data" --outdir="répertoire de sortie"
```

Un timeout de 5 minutes est appliqué au calcul de la couverture.

**Les scripts de test doivent tourner sur les machines de l'école.**

## Quelques informations techniques

Votre projet utilisera les outils/libraires suivantes

- `CMake`
  - Exemple de fichier [`CMakeLists.txt`](code/CMakeLists.txt) pour compiler votre projet.
- Une version récente de `g++` ou `clang++`.
- [PNL](https://pnlnum.github.io/pnl) : installer la dernière version à partir de https://github.com/pnlnum/pnl/releases.
  - Les fichiers `win64` sont des versions binaires pour Windows uniquement.
  - Sous Linux et OSX, il faut télécharger le code source et le compiler

    ```sh
    cd /relative/path/to/pnl
    mkdir build
    cd  build
    cmake ..
    make
    make install
    ```

    Sous Ubuntu, il faut préalablement installer `libblas-dev`, `liblapack-dev` et `gfortran`.

  La librairie PNL est déjà installée sur les machines de l'école
  - Version optimisée: `/matieres/5MMPCPD/pnl`
  - Version avec symboles de débuggage: `/matieres/5MMPCPD/pnl-dbg`

  **Lisez la documentation [html](https://pnlnum.github.io/pnl/manual-html/pnl-manual.html) ou [pdf](https://pnlnum.github.io/pnl/pnl-manual.pdf)**
- Une libraire de manipulation de fichiers `.json` en C++ : [nlohmann_json](https://github.com/nlohmann/). Cette librairie est disponible dans la plupart des gestionnaires de paquets
  - Homebrew : `brew install nlohmann-json`
  - Ubuntu : installer `nlohmann-json3-dev`.

  Cette La librairie PNL est déjà installée sur les machines de l'école.

  Voir un exemple d'utilisation dans [`code/src/test_json_reader.cpp`](code/src/test_json_reader.cpp).

- Les objects `PricingResults` et `Portfolio` sont implémentés dans les fichiers `json_helper.{cpp,hpp}`, `pricing_results.{cpp,hpp}`, `portfolio.{cpp,hpp}` disponible dans [`code/src`](code/src/) que vous devez intégrer à votre projet.

### Chaîne de compilation

La compilation du projet se fera à l'aide de l'outil [cmake](http://www.cmake.org/documentation/) qui permet de générer aussi bien des Makefile Unix qu'un projet Visual. C'est un outil permettant de gérer la compilation multi-plateforme.
CMake est conçu pour lancer la compilation hors des sources de la manière suivante.

```sh
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=/matieres/5MMPCPD/pnl ..
```

Il est possible de modifier d'autres variables utilisées par cmake

- `CMAKE_BUILD_TYPE` : `Debug` ou `Release`. Utilisation
  - `-DCMAKE_BUILD_TYPE=Release` ou `-DCMAKE_BUILD_TYPE=Debug`.
- `CMAKE_CXX_COMPILER` : nom du compilateur C++
- `CMAKE_CXX_CFLAGS` : options du compilateur C++

### Profiling et fuites mémoire

Comme indiqué dans le sujet, il est important de rapidement s'assurer que le code tourne efficacement et ne contient pas de fuite mémoire.

La détection de fuites mémoire se fait à l'aide de l'outil Valgrind, via la commande

```bash
valgrind --tool=memcheck --leak-check=full --show-reachable=yes <nom-executable>
```

De manière alternative, vous pouvez l'outil [`AddressSanitizer`](https://clang.llvm.org/docs/AddressSanitizer.html) de LLVM. Il suffit de compiler en passant l'option `-DCLANG_SANITIZE_FLAGS="-fsanitize=address -fno-omit-frame-pointer"` à CMake. La détection des fuites est activée par défaut sous Linux. Sous OSX, vous pouvez utiliser la variable d'environnement `ASAN_OPTIONS=detect_leaks=1`.

Le profiling peut être réalisé avec l'outil `gprof`

1. Compiler votre code avec l'option `-pg` (avec cmake utiliser l'option `-DCMAKE_CXX_FLAGS=-pg`).
1. Lancer votre code (ne pas s'inquiéter d'une exécution ralentie). Cela crée un fichier `gmon.out`.
1. Lancer la commande `gprof _nom_executable_ gmon.out > profile.txt`
1. Etudier le fichier `profile.txt`.

### L'éditeur Visual Studio Code

Nous recommandons l'utilisation de l'éditeur Visual Studio Code

- Installer l'extension C/C++ [ms-vscode.cpptools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- Configurer l'extension: voir [https://code.visualstudio.com/docs/cpp/customize-default-settings-cpp](https://code.visualstudio.com/docs/cpp/customize-default-settings-cpp). Les propriétés sont accessibles depuis la Palette de Commandes en choisissant _C/C++: Edit Configurations (UI)_. Si après avoir configuré correctement la variable \_Include Path\_, l'intellisense ne fonctionne toujours pas, vérifiez que C\_Cpp.intelliSenseEngine = Default.
- Configurer un débuggueur: [https://code.visualstudio.com/docs/cpp/cpp-debug](https://code.visualstudio.com/docs/cpp/cpp-debug).
- Installer l'extension CMake (twxs.cmake) pour activer la coloration syntaxique des fichiers CMakeLists.txt
- Il est également possible de compiler le projet directement à l'intérieur de VSCode en utilisant CMake. Pour ce faire, il faut installer l'extension CMake Tools (ms-vscode.cmake-tools). Penser à ajouter la variable CMAKE\_PREFIX\_PATH à [cmake.configureSettings](https://vector-of-bool.github.io/docs/vscode-cmake-tools/settings.html#cmake-configuresettings)

#### Comment inspecter le contenu d'un `PnlVect` ou `PnlMat` depuis la fenêtre espion (_Watch_) de Visual Studio ou Visual Studio Code

- Pour lire le contenu de l'emplacement `i` d'un `PnlVect* my_vect`, utiliser `my_vect->array[i]`.
- Pour lire l'intégralité d'un `PnlVec* my_vect` taille `n`, utiliser l'une des syntaxes `*(double(*)[n])my_vect->array`, `*(my_vect->array)@n`, `(my_vect->array),n`
- Pour lire l'intégralité de la ligne `i` d'une matrice `PnlMat *my_mat` de taille `m x n`, utiliser `*(double(*)[n])(my_mat->array + i * n`, `*(my_mat->array + i * n)@n`, `(my_mat->array + i * n),n`.
- Voir la discussion https://github.com/Microsoft/vscode-cpptools/issues/172 pour plus d'information.

## FAQ

### Problèmes avec l'interface graphique de VSCode

Ce problème peut être dû à l'accélération gpu mise en place par VSCode pour le rendu graphique. Pour le désactiver:

- Lancer VSCode avec la commande suivante:

  ```bash
  code --disable-gpu
  ```

- Lancer la palette de commande (_ctrl+shift+P_)
- Choisir `Preferences: Configure Runtime Arguments`
- Ajouter l'instruction `"disable-hardware-acceleration": true`
- Redémarrer VSCode

### Configuration de l'intellisense pour VS Code sur les machines de l'Ensimag

Depuis la palette de commande, ouvrir l'interface de configuration C/C++ (_C/C++: Edit configuration (UI)_) et s'assurer de la valeur des paramètres suivants

- Configuration name : `Linux`
- Compiler path : `/usr/bin/g++` (les compilateurs `clang++` ne semblent pas fonctionner)
- Intellisense mode : `${default}`
- Include path : ajouter `/matieres/5MMPCPD/pnl/include`

### Intellisense et détection d'erreurs non fonctionnelles sous VS Code sur les machines de l'Ensimag

Le problème provient de la localisation de la base de données intellisense de VS Code sur votre `$HOME` qui est un montage NFS, il faut la déplacer sur le disque local. Depuis les _Préférences_ de VS Code, configurer la variable `C_Cpp.default.browse.databaseFilename` à `/scratch/<login>`.

### Trouver la dernière date de constatation avant `t`

```cpp
double EPS = 1E-10;

int compute_last_index(double t, double T, int N) {
    double dt = T / N;
    int nearest_index = std::round(t / dt);
    if (std::fabs(nearest_index * dt - t) < EPS) {
        return nearest_index;
    } else {
        return int(t / dt);
    }
}
```

### Lecture des données de marché

Pour lire un fichier de données de marché, vous pouvez utiliser la fonction `pnl_mat_create_from_file`.

### Imported target `"MPI::MPI_C"` includes non-existent path

If running `cmake` to compile the PNL library fails with the error

```
Imported target "MPI::MPI_C" includes non-existent path
```

run `cmake` with the option `-DWITH_MPI=OFF`.
