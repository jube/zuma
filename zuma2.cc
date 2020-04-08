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
*Structure permettant de modéliser une bille 
*
*@param posDimBille La variable de type PosDim qui donne les *coordonnées sur le plateau d'une bille ainsi que sa dimension
*@param color La variable de type Color qui donne la couleur d'une bille
*@param x : l'abscisse d'un objet
*@param y : l'ordonnée d'un objet 
*@param rayon : le rayon d'un cercle
*@param centre :
*/
struct Bille {
  float x;
  float y;
  int rayon;
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
  float x;
  float y;
  int rayon;
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
  int i = ((int)(Math::random()*(n)));
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
char asChar(int i, char tabCol[]) {
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

//FOnction pour la couleur aléatoire 
Color getRandomCol(int n, char tabCol[]) {
  int i = randomColor(n);
  char color = asChar(i, tabCol);
  return assignementCol(color);
}


bool collision(Bille bille1, Bille bille2) {
  return !( (bille1.rayon + bille2.rayon)*(bille1.rayon + bille2.rayon) <= (bille1.x - bille2.x)*(bille1.x - bille2.x) + (bille1.y - bille2.y)*(bille1.y - bille2.y) );
}

void incruster(Bille billeLance, Bille tabBille[],int i, int nBilles){
  for (int k = nBilles-1; k>0 ; --k){
    tabBille[k].x = tabBille[k-1].x;
    tabBille[k].y = tabBille[k-1].y;
    tabBille[k].rayon = tabBille[0].rayon;
  }
  tabBille[0].x +=tabBille[0].rayon*2;
  for(int j = nBilles-1 ; j>i+1 ; --j){
    tabBille[j].color=tabBille[j-1].color;
  }
  tabBille[i+1].color = billeLance.color;
}


int main() {
  
  // A modif avec taille et titre qu'on veut pr la fenetre de jeu
  const int SCREENW=800;
  const int SCREENH=600;
  RenderWindow window(VideoMode(SCREENW, SCREENH), "Titre");

  //Initialisation de la grenouille ainsi que de sa position au centre du plateau
  Grenouille grenouille;
  grenouille.rayon = 64;
  grenouille.x = (SCREENW - grenouille.rayon/2) / 2;
  grenouille.y = (SCREENH - grenouille.rayon/2) / 2 ;
  grenouille.color = Color::Green;

  ////VARIABLES////
  //Les constantes sont initiées au hasard pr que ça compile c'est a changer
  //constante nb de bille crées en début de partie
  const int NBILLESINIT=100;

  //nb de billes restantes sur le circuit 
  //ititialisé grace a la constante 
  int nBilles=10;

  //vrai quand l'utilisateur a perdu:une bille atteint fin circuit 
  //si vrai jeu s'arrete
  bool perdu;

  //vrai quand uti a gagné  qd y'a plus de bille sur circuit 
  //si vrai jeu s'arrete
  bool gagne;

  //Constante modélisant la vitesse des billes sur le circuit
  const int SPEED=10;

  //Constante modélisant la vitesse de la bille lancée par l'utilisateur
  const int SPEEDLANCE=300;

  //Diamètre de la bille
  const int DIAMBILLE=20;

  //longueur et hauteur du plateau
  const int longueur=500;
  const int hauteur=500;

  //caracters associes aux couleurs
  //faut utiliser une constante pr la taille
  char tabCol[4];
  //Initialisation des couleurs dans chaque case du tableau
  tabCol[0] = 'r';
  tabCol[1] = 'j';
  tabCol[2] = 'b';
  tabCol[3] = 'v';

  //Nombre de couleur(s) restante(s) sur le circuit 
  int n = 4;

  //Tableau répertoriant toutes les billes présentes sur le circuit
  Bille tabBille[NBILLESINIT];

  //bille permettant de modéliser la bille en déplacement (qui a été lancée par l'utilisateur)
  Bille billeLance;
  billeLance.rayon = 10;
  billeLance.x = SCREENW / 2;
  billeLance.y = SCREENH / 2;
  billeLance.color = Color::Black;

  bool deplacer = false;
  double initx = SCREENW / 2;
  double inity = SCREENH / 2;
  double sourisx = 0;
  double sourisy = 0;

  for (int i = 0; i < nBilles; ++i) {
      tabBille[i].x = 100-(20*i);
      tabBille[i].y = 10;
      tabBille[i].rayon = DIAMBILLE/2;
      tabBille[i].color = getRandomCol(n, tabCol);
  }



  Clock clock;

  while (window.isOpen()) {

    Event event;
    while (window.pollEvent(event)) {

      //pour fermer la fenetre quand on clique sur la croix
      if (event.type == Event::Closed) {
        window.close();
      }

      if ((event.type == sf::Event::MouseButtonPressed)&&(deplacer==false)){
        deplacer=true;
        sourisx = event.mouseButton.x;
        sourisy = event.mouseButton.y;
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
    double distanceLance = SPEEDLANCE *dt;
    double hyp ;

    for (int i = 0; i < nBilles; ++i) {
      tabBille[i].x += distance ;
     // tabBille[i].posBille.y += distance;
    }
    
    if (deplacer){
      if ((billeLance.x<0 )||(billeLance.x + billeLance.rayon > 800)||(billeLance.y<0 )||(billeLance.y + billeLance.rayon > 600)){
      billeLance.x = initx;
      billeLance.y = inity;
      deplacer = false;
      }
      
      for (int j = 0;j<nBilles; ++j){
        if(collision(billeLance, tabBille[j])){
	  billeLance.x = initx;
          billeLance.y = inity;
          ++nBilles;
          incruster(billeLance, tabBille, j, nBilles);
	  deplacer = false;
        }
       }

      hyp = sqrt((sourisx-initx)*(sourisx-initx)+(sourisy-inity)*(sourisy-inity));
      billeLance.x+= ((sourisx-initx)/hyp)*distanceLance;
      billeLance.y+= ((sourisy-inity)/hyp)*distanceLance;
    }

    //couleur=la couleur de fond (a changer plus tard)
    window.clear(Color::White);
      
    //Affichage grenouille:
    CircleShape shape(grenouille.rayon);
    shape.setPosition(grenouille.x -grenouille.rayon, grenouille.y-grenouille.rayon);
    shape.setFillColor(grenouille.color);
    window.draw(shape);
    

    //C'est pour afficher les billes ? 
    for (int i = 0; i < nBilles; ++i) {
    CircleShape shape(tabBille[i].rayon);
    shape.setPosition(tabBille[i].x, tabBille[i].y);
    shape.setFillColor(tabBille[i].color);
    window.draw(shape);
    }
      
    CircleShape lancer(billeLance.rayon);
    lancer.setPosition(billeLance.x-billeLance.rayon, billeLance.y-billeLance.rayon);
    lancer.setFillColor(billeLance.color);
    window.draw(lancer);

    window.display();
  }//fin de la while de window.isOpen
  return 0;
}//fin de Main
