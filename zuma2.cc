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
  float rayon;
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
  float rayon;
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
*Fonction permettant d'associer un caractère à une couleur
*
*@param charColor Un caractère déterminé par la fonction assignementChar(i, tabcol[4])
*
*@return color Une couleur associée au caractère charColor
*/
Color assignementCol(char col) {
  Color color;
  switch(col) {
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

/**
*
*Fonction permettant de tirer une couleur aléatoire
*
*@param n Un entier, le nombre de couleur encore disponible dans le circuit
*@param  tabCol un tableau de caractères représentants les couleurs existantes 
*
*@return Une couleur tirée au hasard parmi celles encore disponibles
*/
Color getRandomCol(int n, char tabCol[]) {
  int i = randomColor(n);
  char color = tabCol[i];
  return (assignementCol(color));
}


bool collision(Bille bille1, Bille bille2) {
  return !( (bille1.rayon + bille2.rayon)*(bille1.rayon + bille2.rayon) <= (bille1.x - bille2.x)*(bille1.x - bille2.x) + (bille1.y - bille2.y)*(bille1.y - bille2.y) );
}

void incruster(Bille billeLance, Bille tabBille[],int i, int nBilles){
  for(int j = nBilles-1 ; j>=i ; --j){
    tabBille[j]=tabBille[j-1];
  }
  tabBille[i].color = billeLance.color;

  for (int k = i-1; k>0 ; --k){
    tabBille[k].x = tabBille[k-1].x;
    tabBille[k].y = tabBille[k-1].y;
  }
  tabBille[0].x +=tabBille[0].rayon*2;
}

//Le Color::Black ne sert qu'à des tests
int explosion(int j, Bille tabBille[], int nBilles) {
  int compt1 = 1;
  int compt2 = 1;
  while((tabBille[j+compt1+1].color == tabBille[j].color) && (j+compt1<nBilles)) {
	++compt1;
	  }
  while((tabBille[j+compt2+1].color == tabBille[j].color) && (j+compt2>=0)) {
	--compt2;
	  }
  if(compt1-compt2>1) {
	for(int i = j; i<=j+compt1; i++) {
		tabBille[i].color = Color::Black;
	}
	for(int k = j; k>=j+compt2; k--) {
		tabBille[k].color = Color::Black;
	}
  }
  return (nBilles-(compt1+compt2+1));
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
  grenouille.color = Color::Black;

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
  const int LONGUEUR=500;
  const int HAUTEUR=500;

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
  billeLance.color = getRandomCol(n, tabCol);

  Bille billeReserve;
  billeReserve.rayon = 10;
  billeReserve.x = (SCREENW-40) / 2;
  billeReserve.y = (SCREENH-40) / 2;
  billeReserve.color = getRandomCol(n, tabCol);



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
        Color echange = billeLance.color;
	    billeLance.color= billeReserve.color;
	    billeReserve.color=echange;
      }

      //si l'on appuie sur la barre espace la couleur des billes de reverve et prete a etre lancée sont echangées
      if (event.type == Event::KeyPressed) {
	 if (event.key.code == sf::Keyboard::Space){
           Color echange = billeLance.color;
	       billeLance.color= billeReserve.color;
	       billeReserve.color=echange;
	 }
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
     	billeLance.color = getRandomCol(n, tabCol);
      }
      
      for (int j = 0;j<nBilles; ++j){
        if(collision(billeLance, tabBille[j])){
          ++nBilles;
          incruster(billeLance, tabBille, j, nBilles);
	  explosion(j, tabBille, nBilles);
          billeLance.x = initx;
          billeLance.y = inity;
	  	  billeLance.color = getRandomCol(n, tabCol);
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
    

	//Affichage billes:
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

    CircleShape reserve(billeReserve.rayon);
    reserve.setPosition(billeReserve.x-billeReserve.rayon, billeReserve.y-billeReserve.rayon);
    reserve.setFillColor(billeReserve.color);
    window.draw(reserve);

    window.display();
  }//fin de la while de window.isOpen
  return 0;
}//fin de Main
