#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

/*
* Fonction qui retourne un float de manière aléatoire entre 0.0 et 1.0.
*/
struct Math {
  static float random() {
    static mt19937 engine(time(nullptr));
    uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(engine);
  }
};

/**
*
*@param Structure permettant de donner la position et la dimension d'un objet
*@param x : l'abscisse d'un objet
*@param y : l'ordonnée d'un objet 
*@param width : //peut être changer directement dans la structure et juste mettre le diamètre ? 
*@param height :
*/
struct PosDim {
  float x;
  float y;
  int width;
  int height;
};

/**
*
*Structure permettant de modéliser une bille 
*
*@param posDimBille La variable de type PosDim qui donne les *coordonnées sur le plateau d'une bille ainsi que sa dimension
*@param color La variable de type Color qui donne la couleur d'une bille
*/
struct Bille {
  PosDim posBille;
  Color color;
};

/**
*
*Structure permettant de modéliser la grenouille
*
*@param posGre La position de la grenouille sur le plateau
*@param lance  La bille prête à être lancée par l'utilisateur
*@param reserve La bille en réserve (la bille qui sera *lancée par l'utilisateur après qu'il ait utilisé la bille lance)
*/
struct Grenouille{
  PosDim posGre;
  Color color ;
  Bille lance;
  Bille reserve;
};

/**
*
*Fonction permettant de tirer au hasard le numéro d'une couleur
*
*@param n Le nombre de couleurs encore disponibles dans le circuit
*
*@return i Un entier tiré au hasard dans le nombre de *couleurs restantes
*/
int randomColor(int n) {
  int i = ((int)(Math::random()*(n-1)));
  return i;
}

/**
*
*Fonction permettant d'assigner un entier à une couleur
*
*@param i Entier tiré au hasard dans la fonction randomColor(n)
*@param tabCol[4] Tableau où sont répertoriées toutes les *couleurs représentées par des caractères
*
*@return color Caractère tiré au hasard dans le tableau
*/
char assignementChar(int i, char tabCol[4]) {
  char color = tabCol[i];
  return color;
}

/**
*
*Fonction permettant d'associer un caractère à une couleur
*
*@param charColor Un caractère déterminé par la fonction assignementChar(i, tabcol[4])
*
*@return color Une couleur associée au caractère charColor
*/
Color assignementCol(char charColor) {
  Color color;
  switch(charColor) {
    case 'r' :
        color = Color::Red;
    break;

    case 'j' :
       color = Color::Yellow;
    break;

    case 'v' :
        color = Color::Green;
    break;

    case 'b' :
        color = Color::Blue;
    break;
  }
  return color;
}


int main() {
  
  // A modif avec taille et titre qu'on veut pr la fenetre de jeu
  const int SCREENW=800;
  const int SCREENH=600;
  RenderWindow window(VideoMode(SCREENW, SCREENH), "Titre");

  //Initialisation de la grenouille ainsi que de sa position au centre du plateau
  Grenouille grenouille;
  grenouille.posGre.width = grenouille.posGre.height = 64;
  grenouille.posGre.x = (SCREENW - grenouille.posGre.width/2) / 2;
  grenouille.posGre.y = (SCREENH - grenouille.posGre.height/2) / 2 ;
  grenouille.color = Color::Green;

  ////VARIABLES////
  //Les constantes sont initiées au hasard pr que ça compile c'est a changer
  //constante nb de bille crées en début de partie
  const int NBILLESINIT=10;

  //nb de billes restantes sur le circuit 
  //ititialisé grace a la constante 
  int nBilles=NBILLESINIT;

  //vrai quand l'utilisateur a perdu:une bille atteint fin circuit 
  //si vrai jeu s'arrete
  bool perdu;

  //vrai quand uti a gagné  qd y'a plus de bille sur circuit 
  //si vrai jeu s'arrete
  bool gagne;

  //Constante modélisant la vitesse des billes sur le circuit
  const int SPEED=100;

  //Constante modélisant la vitesse de la bille lancée par l'utilisateur
  const int speedLance=150;

  //Diamètre de la bille
  const int DIAMBILLE=10;

  //longueur et hauteur du plateau
  const int longueur=500;
  const int hauteur=500;

  //caracters associes aux couleurs
  //faut utiliser une constante pr la taille
  char tabCol[4];
  //Initialisation des couleurs dans chaque case du tableau
  tabCol[0] = 'r';
  tabCol[1] = 'j';
  tabCol[2] = 'v';
  tabCol[3] = 'b';

  //Nombre de couleur(s) restante(s) sur le circuit 
  int n;

  //Tableau répertoriant toutes les billes présentes sur le circuit
  Bille tabBille[nBilles];

  //bille permettant de modéliser la bille en déplacement (qui a été lancée par l'utilisateur)
  Bille billeLance;

  for (int i = 0; i < nBilles; ++i) {
      tabBille[i].posBille.x = 20*i;
      tabBille[i].posBille.y = 10;
      tabBille[i].posBille.width = DIAMBILLE;
      tabBille[i].color = Color::Black;
  }



  Clock clock;

  while (window.isOpen()) {

    Event event;
    while (window.pollEvent(event)) {

      //pour fermer la fenetre quand on clique sur la croix
      if (event.type == Event::Closed) {
        window.close();
      }

      //si l'on appuie sur la barre espace la couleur des billes de reverve et prete a etre lancée sont echangées
      if (event.key.code == sf::Keyboard::Escape){
        Color echange = grenouille.lance.color;
	      grenouille.lance.color=grenouille.reserve.color;
	      grenouille.reserve.color=echange;
      }

    }//fin de while window.pollEvent


    float dt = clock.restart().asSeconds();
    double distance = SPEED*dt;

    for (int i = 0; i < nBilles; ++i) {
      tabBille[i].posBille.x += distance ;
     // tabBille[i].posBille.y += distance;
    }

    //couleur=la couleur de fond (a changer plus tard)
    window.clear(Color::White);
      
    //Affichage grenouille:
    CircleShape shape(grenouille.posGre.width);
    shape.setPosition(grenouille.posGre.x -grenouille.posGre.width, grenouille.posGre.y-grenouille.posGre.width);
    shape.setFillColor(grenouille.color);
    window.draw(shape);
    

    //C'est pour afficher les billes ? 
    for (int i = 0; i < 5; ++i) {
    CircleShape shape(tabBille[i].posBille.width);
    shape.setPosition(tabBille[i].posBille.x, tabBille[i].posBille.y);
    shape.setFillColor(tabBille[i].color);
    window.draw(shape);
    }
      
    window.display();
  }//fin de la while de window.isOpen
  return 0;
}//fin de Main