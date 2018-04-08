#include "graph.h"
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "grman/grman.h"
#include <list>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        //m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}

EdgeInterface::~EdgeInterface()
{
    //m_top_edge.attach_from(nullptr);
    //m_top_edge.attach_to(nullptr);
}

/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    /*
    /// Dans ce sous cadre on ajoute 2 ascenseurs verticaux
    m_boite_regles.add_child( m_regle_reel );
    m_regle_reel.set_range(1, 10); // Par défaut valeurs réelles
    m_regle_reel.set_frame(4,5,15,70);

    m_boite_regles.add_child( m_regle_entier );
    m_regle_entier.set_range(1, 10, true); // 3ème param true => valeurs entières
    m_regle_entier.set_frame(20,5,15,70);
    m_regle_entier.set_value(6);*/

    ///Boite a bouton legerement modifiee pour etre une 'toolbar' a droite de l'ecran (pris dans l'autre code)
    m_top_box.add_child( m_boite_boutons );
    m_boite_boutons.set_dim(60,700);
    m_boite_boutons.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up );
    m_boite_boutons.set_bg_color(FUCHSIACLAIR);
    m_boite_boutons.set_moveable();

    ///Le bouton DELETE
    m_boite_boutons.add_child( m_bouton1 );
    m_bouton1.set_frame(3,300,48,16);
    m_bouton1.set_bg_color(BLANC);
    m_bouton1.add_child(m_bouton1_label);
    m_bouton1_label.set_message("DELETE");

    ///Le bouton ADD
    m_boite_boutons.add_child( m_bouton2 );
    m_bouton2.set_frame(3,200,48,16);
    m_bouton2.set_bg_color(BLANC);
    m_bouton2.add_child(m_bouton2_label);
    m_bouton2_label.set_message("ADD");

    ///Le bouton forte connexite
    m_boite_boutons.add_child( m_bouton3 );
    m_bouton3.set_frame(3,400,48,16);
    m_bouton3.set_bg_color(BLANC);

    m_bouton3.add_child(m_bouton3_label);
    m_bouton3_label.set_message("CNX");

} ///L'objectif est : quand j'appuie sur SELECT, je peux select un sommet
///Quand j'appuie sur DELETE, je delete le sommet que j'ai select


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_graph_1()
{
    std::ifstream graphe ("graphe_1.txt");
    std::ifstream arc ("arc_1.txt");
    int idx=0, x=0, y=0;
    std::string pic_name="";
    double value=0.0;
    int id_vert1=0, id_vert2=0;
    double weight=0.0;

    int nbr_img=0, nbr_arcs=0;

    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    ///Lecture des coordonées dans des fichiers
    if(graphe)
    {
        while(!graphe.eof())
        {
            graphe >> idx >> value >> x >> y >> pic_name;
            add_interfaced_vertex(idx, value, x, y, pic_name);
            nbr_img++;
        }
        graphe.close();
    }
    else
        std::cout << "Erreur fichier !" << std::endl;

    set_nbr_img(nbr_img);

    if(arc)
    {
        while(!arc.eof())
        {
            arc >> idx >> id_vert1 >> id_vert2 >> weight;
            add_interfaced_edge(idx, id_vert1, id_vert2, weight);
            nbr_arcs++;
        }
        arc.close();
    }

    else
        std::cout << "Erreur fichier !" << std::endl;

    set_nbr_arcs(nbr_arcs);
    /*
    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);*/
}

void Graph::make_graph_2()
{
    std::ifstream graphe ("graphe_2.txt");
    std::ifstream arc ("arc_2.txt");
    int idx=0, x=0, y=0;
    std::string pic_name="";
    double value=0.0;
    int id_vert1=0, id_vert2=0;
    double weight=0.0;

    int nbr_img=0, nbr_arcs=0;

    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    ///Lecture des coordonées dans des fichiers
    if(graphe)
    {
        while(!graphe.eof())
        {
            graphe >> idx >> value >> x >> y >> pic_name;
            add_interfaced_vertex(idx, value, x, y, pic_name);
            nbr_img++;
        }
        graphe.close();
    }
    else
        std::cout << "Erreur fichier !" << std::endl;

    set_nbr_img(nbr_img);

    if(arc)
    {
        while(!arc.eof())
        {
            arc >> idx >> id_vert1 >> id_vert2 >> weight;
            add_interfaced_edge(idx, id_vert1, id_vert2, weight);
            nbr_arcs++;
        }
        arc.close();
    }

    else
        std::cout << "Erreur fichier !" << std::endl;

    set_nbr_arcs(nbr_arcs);
    /*
    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);*/
}

void Graph::make_graph_3()
{
    std::ifstream graphe ("graphe_3.txt");
    std::ifstream arc ("arc_3.txt");
    int idx=0, x=0, y=0;
    std::string pic_name="";
    double value=0.0;
    int id_vert1=0, id_vert2=0;
    double weight=0.0;

    int nbr_img=0, nbr_arcs=0;

    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    ///Lecture des coordonées dans des fichiers
    if(graphe)
    {
        while(!graphe.eof())
        {
            graphe >> idx >> value >> x >> y >> pic_name;
            add_interfaced_vertex(idx, value, x, y, pic_name);
            nbr_img++;
        }
        graphe.close();
    }
    else
        std::cout << "Erreur fichier !" << std::endl;

    set_nbr_img(nbr_img);

    if(arc)
    {
        while(!arc.eof())
        {
            arc >> idx >> id_vert1 >> id_vert2 >> weight;
            add_interfaced_edge(idx, id_vert1, id_vert2, weight);
            nbr_arcs++;
        }
        arc.close();
    }

    else
        std::cout << "Erreur fichier !" << std::endl;

    set_nbr_arcs(nbr_arcs);
    /*
    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);*/
}


void Graph::save_coords_1()
{
    std::ofstream graphe ("graphe_1.txt");
    std::ofstream arc ("arc_1.txt");

    if(graphe)
    {
        for(int i=0; i<get_nbr_img(); i++)
        {
            if (m_vertices[i].m_interface->m_img.get_img_state())
            {graphe << i << " " << m_vertices[i].m_value << " " << m_vertices[i].m_interface->m_top_box.get_posx() << " " << m_vertices[i].m_interface->m_top_box.get_posy()
                    << " " << m_vertices[i].m_interface->m_img.get_pic_name() << std::endl;}
        }

        graphe.close();
    }

    else
        std::cout << "Erreur fichier !" << std::endl;

    if(arc)
    {
        for(int i=0; i<get_nbr_arcs(); i++)
        {
            if (m_edges[i].m_interface->m_top_edge.get_arc_state())
            {arc << i << " " << m_edges[i].m_from << " " << m_edges[i].m_to << " " << m_edges[i].m_interface->m_slider_weight.get_value() << std::endl;}
        }

        arc.close();
    }

    else
        std::cout << "Erreur fichier !" << std::endl;

}

void Graph::save_coords_2()
{
    std::ofstream graphe ("graphe_2.txt");
    std::ofstream arc ("arc_2.txt");

    if(graphe)
    {
        for(int i=0; i<get_nbr_img(); i++)
        {
            if (m_vertices[i].m_interface->m_img.get_img_state())
            {graphe << i << " " << m_vertices[i].m_value << " " << m_vertices[i].m_interface->m_top_box.get_posx() << " " << m_vertices[i].m_interface->m_top_box.get_posy()
                    << " " << m_vertices[i].m_interface->m_img.get_pic_name() << std::endl;}
        }

        graphe.close();
    }

    else
        std::cout << "Erreur fichier !" << std::endl;

    if(arc)
    {
        for(int i=0; i<get_nbr_arcs(); i++)
        {
            if (m_edges[i].m_interface->m_top_edge.get_arc_state())
            {arc << i << " " << m_edges[i].m_from << " " << m_edges[i].m_to << " " << m_edges[i].m_interface->m_slider_weight.get_value() << std::endl;}
        }

        arc.close();
    }

    else
        std::cout << "Erreur fichier !" << std::endl;

}

void Graph::save_coords_3()
{
    std::ofstream graphe ("graphe_3.txt");
    std::ofstream arc ("arc_3.txt");

    if(graphe)
    {
        for(int i=0; i<get_nbr_img(); i++)
        {
            if (m_vertices[i].m_interface->m_img.get_img_state())
            {graphe << i << " " << m_vertices[i].m_value << " " << m_vertices[i].m_interface->m_top_box.get_posx() << " " << m_vertices[i].m_interface->m_top_box.get_posy()
                    << " " << m_vertices[i].m_interface->m_img.get_pic_name() << std::endl;}
        }

        graphe.close();
    }

    else
        std::cout << "Erreur fichier !" << std::endl;

    if(arc)
    {
        for(int i=0; i<get_nbr_arcs(); i++)
        {
            if (m_edges[i].m_interface->m_top_edge.get_arc_state())
            {arc << i << " " << m_edges[i].m_from << " " << m_edges[i].m_to << " " << m_edges[i].m_interface->m_slider_weight.get_value() << std::endl;}
        }

        arc.close();
    }

    else
        std::cout << "Erreur fichier !" << std::endl;

}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    if ( m_interface->m_bouton1.clicked() ) ///Cette boucle et celle d'apres verifie que le bouton est clique et met le booleen
    {                                       ///corresondant a true
        m_add_mode = false;
        m_cnx_mode = false;
        m_delete_mode = true;
        //std::cout << "delete button hit \n";
    }
    else if ( m_interface->m_bouton2.clicked() )
    {
        m_add_mode = true;
        m_delete_mode = false;
        m_cnx_mode = false;
        //std::cout << "add button hit \n";
    }

    else if ( m_interface->m_bouton3.clicked() )
    {
        m_add_mode = false;
        m_delete_mode = false;
        m_cnx_mode = true;
    }

    if ( m_delete_mode &&
         grman::mouse_click ) ///Si on est dans le delete et qu'on a clique :
    {
        for (auto &elt : m_vertices) ///Parcours des sommets
        {
            auto& vrtx( elt.second ); ///C'est le sommet ( elt.second c'est le sommet )

            if ( vrtx.is_mouse_over() ) ///Si la souris clique un sommet
            {
                const auto vrtx_idx( elt.first ); ///c'est l'indice du sommet ( le elt.first c'est la cle de la map )

                std::cout << "Erasing vertex: " << vrtx_idx << std::endl;
                std::cout << "  Incoming edges: " << vrtx.m_in.size() << std::endl;
                std::cout << "  Outgoing edges: " << vrtx.m_out.size() << std::endl;
                for (const auto edge_idx : vrtx.m_in) ///Pour effacer les aretes qui arrivent vers le sommet
                {
                    std::cout << "Erasing incoming edge: " << edge_idx << std::endl;

                    m_edges[edge_idx].removeFrom( m_interface->m_main_box ); ///On enleve ces aretes de l'interface graphique
                    std::cout << "Erased" << std::endl;
                    m_edges[edge_idx].m_interface->m_top_edge.set_arc_state(false);
                }
                for (const auto edge_idx : vrtx.m_out) ///Pour effacer les aretes qui partent de ce sommet
                {
                    std::cout << "Erasing outgoing edge: " << edge_idx << std::endl;

                    m_edges[edge_idx].removeFrom( m_interface->m_main_box );
                    std::cout << "Erased" << std::endl;
                    m_edges[edge_idx].m_interface->m_top_edge.set_arc_state(false);

                }
                std::cout << "Zapping vertex..." << std::endl;
                m_vertices[vrtx_idx].removeFrom(m_interface->m_main_box); ///Enfin, on enleve le sommet de l'interface graphique
                m_deleted_vertices.push_back( vrtx_idx ); ///Cette ligne ajoute l'index du sommet a un vector de sommets supprimes, qui seront utiles pour le mode ajout

                m_vertices[vrtx_idx].m_interface->m_img.set_img_state(false);


                break;
            }
        }
    }
    if ( m_add_mode )
    {
        //std::cout << "clicked" << std::endl;
        if ( !m_deleted_vertices.empty() ) ///Si on a deja supprime un sommet :
        {
            const auto vrtx_idx( m_deleted_vertices.back() ); ///Le dernier element qu'on a rajoute (son indice)
            auto&      vrtx( m_vertices[vrtx_idx] );          ///Le sommet

            m_deleted_vertices.pop_back(); ///On enleve ce sommet du vector
            std::cout << "Adding vertex..." << std::endl;
            vrtx.addTo(m_interface->m_main_box); ///On le rerajoute a la l'interface graphique
            std::cout << "Added vertex." << std::endl;

            for (const auto edge_idx : vrtx.m_in)
            {
                std::cout << "Adding incoming edge: " << edge_idx << std::endl;

                m_edges[edge_idx].addTo( m_interface->m_main_box ); ///On rerajoute les aretes entrantes

                std::cout << "Added" << std::endl;
            }
            for (const auto edge_idx : vrtx.m_out) ///On rerajoute les aretes sortantes
            {
                std::cout << "Adding outgoing edge: " << edge_idx << std::endl;

                m_edges[edge_idx].addTo( m_interface->m_main_box );
                std::cout << "Added" << std::endl;
            }
        }
        m_add_mode = false; ///Pour finir, add mode est remis a false pour ne pas rajouter tous les sommets d'un coup
    }

    if ( m_cnx_mode &&
         grman::mouse_click )
    {
        tarjan();
    }
    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name)
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

///Ces deux fonctions ont ete faites a partir du pseudo code de l'algorithme de Tarjan sur wikipedia

void Graph::fortement_connexe(const int vertex_index) ///Prends les numero de sommet en parametre
{
    auto& vrtx( m_vertices [vertex_index]);

    vrtx.m_tarjan_index = m_tarjan_index;
    vrtx.m_tarjan_lowlink = m_tarjan_index;
    m_tarjan_index += 1;
    m_tarjan_stack.push_back(vertex_index);
    vrtx.m_tarjan_on_stack = true;
    for (const auto edge_idx : vrtx.m_out)
    {
        const auto& edge( m_edges[edge_idx]);
        auto& w( m_vertices[edge.m_to]);
        if( w.m_tarjan_index == -1 )
        {
            fortement_connexe( edge.m_to );
            vrtx.m_tarjan_lowlink = std::min(vrtx.m_tarjan_lowlink, w.m_tarjan_lowlink);
        }
        else if( w.m_tarjan_on_stack )
        {
            vrtx.m_tarjan_lowlink = std::min(vrtx.m_tarjan_lowlink, w.m_tarjan_index);
        }
    }
    if( vrtx.m_tarjan_lowlink == vrtx.m_tarjan_index)
    {
        std::vector<int> liste_fortement_connexe;
        while ( true )
        {
            const auto w_idx( m_tarjan_stack.back() );
            auto& w( m_vertices[ w_idx ]);
            m_tarjan_stack.pop_back();
            w.m_tarjan_on_stack = false;
            liste_fortement_connexe.push_back(w_idx);

            if ( w_idx == vertex_index )
            {
                break;
            }
        }
        std::cout << "Les composantes fortement connexes sont: ";
        for (std::size_t i = 0; i < liste_fortement_connexe.size(); ++i)
        {
            std::cout << liste_fortement_connexe[i]  << " ";
        }
        std::cout << std::endl;
    }
}

void Graph::tarjan()
{
    m_tarjan_index = 0;
    m_tarjan_stack.clear(); ///Pour que le tableau soit vide
    for (auto &elt : m_vertices) ///Parcours des soemmets pour initialiser tout les sommets avec les valeurs de l'algo
    {
        auto& vrtx( elt.second );
        vrtx.tarjan_init();
    }
    for (auto &elt : m_vertices)
    {
        auto& vrtx( elt.second );
        if ( vrtx.m_tarjan_index == -1) ///Toujours vrai, on test alors la forte connexite
        {
            fortement_connexe(elt.first);
        }
    }
}
