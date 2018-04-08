#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    int choix;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;

    do
    {

        std::cout << "1. Graphe ECE" << std::endl;
        std::cout << "2. Graphe Banquise" << std::endl;
        std::cout << "3. Graphe Savane" << std::endl;
        std::cout << "0. Quitter" << std::endl;

        std::cin >> choix;

    switch (choix)
        {
        case 1:

            g.make_graph_1();

            /// Vous gardez la main sur la "boucle de jeu"
            /// ( contrairement � des frameworks plus avanc�s )
            while ( !key[KEY_ESC] )
            {
                /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
                g.update();

                /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
            }
            g.save_coords_1();

            grman::fermer_allegro();
            break;

        case 2:

            g.make_graph_2();

            /// Vous gardez la main sur la "boucle de jeu"
            /// ( contrairement � des frameworks plus avanc�s )
            while ( !key[KEY_ESC] )
            {
                /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
                g.update();

                /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
            }
            g.save_coords_2();

            grman::fermer_allegro();
            break;

        case 3:

            g.make_graph_3();

            /// Vous gardez la main sur la "boucle de jeu"
            /// ( contrairement � des frameworks plus avanc�s )
            while ( !key[KEY_ESC] )
            {
                /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
                g.update();

                /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
            }
            g.save_coords_3();

            grman::fermer_allegro();
            break;
        }
    }
    while(choix!=0);


    return 0;
}
END_OF_MAIN();


