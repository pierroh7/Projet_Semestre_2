#include "graph.h"
#include <fstream>
#include <string>
#include <iostream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
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
        m_img.set_pic_idx(pic_idx);
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


/// Gestion du Vertex avant l'appel � l'interface
/*void Vertex::pre_update(std::ifstream &fichier1, std::ifstream &fichier2)
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}*/


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update(std::ofstream &fichier)
{
    int idx;
    double value;
    int x;
    int y;
    std::string pic_name;
    int pic_idx;

    if (!m_interface)
        return;

     if (fichier)
    {
        fichier << idx << value << x << y << pic_name << pic_idx;
        fichier.close();
    }
    else std::cout << "Erreur fichier" << std::endl;


    /// Reprendre la valeur du slider dans la donn�e m_value locale
    //m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
/*void Edge::pre_update(std::ifstream &fichier1, std::ifstream &fichier2)
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}
*/
/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update(std::ofstream &fichier)
{
    int idx;
    int id_vert1;
    int id_vert2;
    double weight;

    if (!m_interface)
        return;

    if (fichier)
    {
        fichier << idx << id_vert1 << id_vert2 << weight;
        fichier.close();
    }

    else std::cout << "Erreur fichier" << std::endl;
    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    //m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
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
}

/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.

///Va lire dans des fichiers
void Graph::make_example(std::ifstream &fichier1, std::ifstream &fichier2)
{
    int idx=0;
    double value=0;
    int x=0;
    int y=0;
    std::string pic_name="";
    int pic_idx=0;
    int id_vert1;
    int id_vert2;
    double weight;
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    ///UTILISATION REMASTERISEE DU SOUS-PROGRAMME void Graph::add_interfaced_vertex DE MR FERCOQ POUR ALLER LIRE DANS UN FICHIER
    if (fichier1)
    {
        while(!fichier1.eof())
        {
            fichier1 >> idx >> value >> x >> y >> pic_name;

            if ( m_vertices.find(idx)!=m_vertices.end() )
            {
                std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
                throw "Error adding vertex";
            }
            // Cr�ation d'une interface de sommet
            VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
            // Ajout de la top box de l'interface de sommet
            m_interface->m_main_box.add_child(vi->m_top_box);
            // On peut ajouter directement des vertices dans la map avec la notation crochet :
            m_vertices[idx] = Vertex(value, vi);
        }
        fichier1.close();
    }
    else
        std::cout << "Erreur fichier !" << std::endl;

    ///UTILISATION REMASTERISEE DU SOUS-PROGRAMME void Graph::add_interfaced_graphe DE MR FERCOQ POUR ALLER LIRE DANS UN FICHIER
    if (fichier2)
    {
        while(!fichier2.eof())
        {
            fichier2 >> idx >> id_vert1 >> id_vert2 >> weight;

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
        }
        fichier2.close();
    }
    else
        std::cout << "Erreur fichier !" << std::endl;

    /* /// Les sommets doivent �tre d�finis avant les arcs
     // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
     add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
     add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
     add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
     add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
     add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
     add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
     add_interfaced_vertex(6,  0.0, 250, 500, "bad_clowns_xx3xx.jpg", 1);
     add_interfaced_vertex(7,  0.0, 400, 500, "bad_clowns_xx3xx.jpg", 2);

     add_interfaced_vertex(8,  20.0, 600, 100, "clown6.jpg");
     add_interfaced_vertex(9,  80.0, 800, 300, "clown7.jpg");
     add_interfaced_vertex(10,  120.0, 550, 500, "clown8.jpg");
     add_interfaced_vertex(11,  10.0, 800, 100, "clown9.jpg");

     /// Les arcs doivent �tre d�finis entre des sommets qui existent !
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
     add_interfaced_edge(9, 3, 7, 80.0);

     add_interfaced_edge(10, 8, 5, 25.0);
     add_interfaced_edge(11, 8, 11, 40.0);
     add_interfaced_edge(12, 10, 1, 50.0);
     add_interfaced_edge(13, 11, 7, 10.0);
     add_interfaced_edge(14, 1, 9, 80.0);
     add_interfaced_edge(15, 7, 9, 25.0);
     add_interfaced_edge(16, 3, 8, 25.0);
     add_interfaced_edge(17, 2, 8, 40.0);
     add_interfaced_edge(18, 0, 11, 50.0);
     add_interfaced_edge(19, 4, 10, 60.0);*/
}


/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
/*void Graph::pre_update(std::ifstream &fichier1, std::ifstream &fichier2)
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update(fichier1, fichier2);

    for (auto &elt : m_edges)
        elt.second.pre_update(fichier1, fichier2);

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update(fichier2);

    for (auto &elt : m_edges)
        elt.second.post_update(fichier2);
}*/

void Graph::post_update(std::ofstream &fichier1, std::ofstream &fichier2)
{
    if (!m_interface)
        return;

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update(fichier2);

    for (auto &elt : m_edges)
        elt.second.post_update(fichier1);
}
/// Aide � l'ajout de sommets interfac�s

///Plus besoin d'elles ici
/*void Graph::add_interfaced_vertex(std::ifstream &fichier, int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if (fichier)
    {
        while(!fichier.eof())
        {
            fichier >> idx >> value >> x >> y >> pic_name;

            if ( m_vertices.find(idx)!=m_vertices.end() )
                {
                    std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
                    throw "Error adding vertex";
                }
            // Cr�ation d'une interface de sommet
            VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
            // Ajout de la top box de l'interface de sommet
            m_interface->m_main_box.add_child(vi->m_top_box);
            // On peut ajouter directement des vertices dans la map avec la notation crochet :
            m_vertices[idx] = Vertex(value, vi);
        }
        fichier.close();
    }
    else std::cout << "Erreur fichier !" << std::endl;

}

/// Aide � l'ajout d'arcs interfac�s
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
}*/

