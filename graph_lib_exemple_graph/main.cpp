#include "grman/grman.h"
#include <iostream>
#include <stdio.h>
#include "graph.h"
#include <fstream>

int main()
{
    int choix;
    std::ifstream graphe1("graphe_1.txt");
    std::ifstream graphe2("graphe_2.txt");
    std::ifstream graphe3("graphe_3.txt");

    std::ifstream arc1("arcs_1.txt");
    std::ifstream arc2("arcs_2.txt");
    std::ifstream arc3("arcs_3.txt");

    std::ofstream graphe_1("graphe_1.txt");
    std::ofstream graphe_2("graphe_2.txt");
    std::ofstream graphe_3("graphe_3.txt");

    std::ofstream arc_1("arcs_1.txt");
    std::ofstream arc_2("arcs_2.txt");
    std::ofstream arc_3("arcs_3.txt");
    do
    {
        std::cout << "1. Choisir un graphe" << std::endl;
        std::cout << "2. Nouveau graphe" << std::endl;
        std::cout << "0. Quitter" << std::endl;
        std::cin >> choix;

        switch(choix)
        {
        case 1:
            /// A appeler en 1er avant d'instancier des objets graphiques etc...
            grman::init();

            /// Le nom du répertoire où se trouvent les images à charger
            grman::set_pictures_path("pics");

            /// Un exemple de graphe
            Graph g;
            g.make_example(graphe1, arc1);


            /// Vous gardez la main sur la "boucle de jeu"
            /// ( contrairement à des frameworks plus avancés )
            while ( !key[KEY_ESC] )
            {
                /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
                //g.pre_update(graphe1, arc1);
                g.post_update(graphe_1, arc_1);


                /// Mise à jour générale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
            }

            grman::fermer_allegro();
            break;
        }
    }
    while (choix!=0);

    return 0;
}
END_OF_MAIN();


