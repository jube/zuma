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

/**
*Fonction permettant de savoir si une couleur de bille est présente sur le circuit ou non
*
*@param nBilles le nombre de billes présentes sur le circuit
*@param tabBille[] Le tableau répertoriant toutes les billes du circuit
*@param couleur La couleur que l'on cherche
*
*@return un booléen, true si la couleur est présente sur le circuit, false sinon
*/
bool colorExist(int nBilles, Bille tabBille[], Color couleur){
  bool exist=false;
  for(int i = 0 ; i < nBilles; ++i){
      if(tabBille[i].color== couleur){
      	exist=true;
      }
  }
  return exist;
}

/**
*Fonction permettant de connaitre l'indice auquel se trouve une couleur dans le tableau
*
*@param tabCol un tableau de caractères représentants les couleurs existantes 
*@param couleur un caractère représentant la couleur recherchée
*
*@return un entier,l'indice auquel se trouve le caractère dans le tableau
*/
int indiceCouleur(char tabCol[], char couleur){
  int indice=-1;
  for (int n=0;n<4;n++){
       if(tabCol[n] == couleur){
	  indice=n;
       }
  }
  return indice;
}
	
/**
*Fonction permettant de déplacer un caractère vers la gauche du tableau représentant les couleurs
*
*@param indice l'indice du caractère a déplacer
*@param tabCol un tableau de caractères représentants les couleurs existantes 
*/
void decaler(int indice, char tabCol[]){
  char tempo=tabCol[indice];
  for (int w=indice;w>0;w--){
	tabCol[w]=tabCol[w-1];
  }
  tabCol[0]=tempo;
}

/**
*Fonction permettant ranger le tableau représentant les couleurs selon celles qui sont encore sur le circuit et de savoir combien il y reste de couleurs
*
*@param nBilles le nombre de billes présentes sur le circuit
*@param tabBille[] Le tableau répertoriant toutes les billes du circuit
*@param tabCol un tableau de caractères représentants les couleurs existantes 
*
*@return un entier, le nombre de couleurs restantes sur le circuit 
*/
int rangerTabCol(int nBilles, Bille tabBille[],char tabCol[]){
  int comp=0;
  if(colorExist(nBilles,tabBille,Color::Red)){
        decaler(indiceCouleur(tabCol,'r'),tabCol);
	comp+=1;
  }
  if(colorExist(nBilles,tabBille,Color::Blue)){
	decaler(indiceCouleur(tabCol,'b'),tabCol);
	comp+=1;
  }
  if(colorExist(nBilles,tabBille,Color::Yellow)){
	decaler(indiceCouleur(tabCol,'j'),tabCol);
	comp+=1;
  }
  if(colorExist(nBilles,tabBille,Color::Green)){
	decaler(indiceCouleur(tabCol,'v'),tabCol);
	comp+=1;
  }
  return comp;
}


/**
*Fonction permettant de savoir si une bille est entrée en collision avec une autre bille
*
*@param bille1
*@param bille2
*
*@return un booléen, true si les billes sont entrées en collision, false sinon
*/

bool collision(Bille bille1, Bille bille2) {
  return !( (bille1.rayon + bille2.rayon)*(bille1.rayon + bille2.rayon) <= (bille1.x - bille2.x)*(bille1.x - bille2.x) + (bille1.y - bille2.y)*(bille1.y - bille2.y) );
}

/**
*Fonction permettant d'incruster une bille dans le circuit
*
*@param billeLance La bille lancée qui va être incrustée dans le circuit
*@param tabBille[] Le tableau répertoriant toutes les billes du circuit
*@param i rang auquel la bille va être incrustée
*@param nBilles le nombre de billes présentes sur le circuit
*/

void incruster(Bille billeLance, Bille tabBille[],int i, int nBilles){
  if (billeLance.x-tabBille[i].x >= 0){
  //incrustation à droite de la bille
    if (sqrt((tabBille[i].x-tabBille[i-1].x)*(tabBille[i].x-tabBille[i-1].x)+(tabBille[i].y-tabBille[i-1].y)*(tabBille[i].y-tabBille[i-1].y))<4*tabBille[0].rayon){
      for(int j = nBilles-1 ; j>=i ; --j){
        tabBille[j]=tabBille[j-1];
      }
      tabBille[i].color = billeLance.color;
      for (int k = i-1; k>0 ; --k){
        tabBille[k].x = tabBille[k-1].x;
        tabBille[k].y = tabBille[k-1].y;
      }
      tabBille[0].x +=tabBille[0].rayon*2;
    //cas sans collision
    }else{
      for(int j = nBilles-1 ; j>i ; --j){
        tabBille[j]=tabBille[j-1];
      }
      tabBille[i].color = billeLance.color;
      tabBille[i].x= tabBille[i+1].x+tabBille[0].rayon*2;
    }
  }else{
  //incrustation à gauche de la bille
    if (i+1==nBilles-1){
      tabBille[i+1].color = billeLance.color;
      tabBille[i+1].x= tabBille[i].x-tabBille[0].rayon*2;
      tabBille[i+1].y= tabBille[i].y;
      tabBille[i+1].rayon= tabBille[i].rayon;
    }else{
      if (sqrt((tabBille[i].x-tabBille[i+1].x)*(tabBille[i].x-tabBille[i+1].x)+(tabBille[i].y-tabBille[i+1].y)*(tabBille[i].y-tabBille[i+1].y))<4*tabBille[0].rayon){
        //cas où il y a collision
        for(int j = nBilles-1 ; j>i+1 ; --j){
        tabBille[j]=tabBille[j-1];
        }
        tabBille[i+1].color = billeLance.color;

        for (int k = i+1; k>0 ; --k){
          tabBille[k].x = tabBille[k-1].x;
          tabBille[k].y = tabBille[k-1].y;
        }
        tabBille[0].x +=tabBille[0].rayon*2;
      //cas sans collision
      }else{
        for(int j = nBilles-1 ; j>i+1 ; --j){
          tabBille[j]=tabBille[j-1];
        }
        tabBille[i+1].color = billeLance.color;
        tabBille[i+1].x= tabBille[i].x-tabBille[0].rayon*2;
      }
    }
  }
}

/**
*Fonction permettant de faire disparaître des billes du circuit
*
*@param j Le rang de la bille qui a été lancée et qui provoque une potentielle explosion
*@param borne1 Le nombre de billes de la même couleur que la bille lancée se plaçant avant dans le tableau
*@param borne2 Le nombre de billes de la même couleur que la bille lancée se plaçant après dans le tableau
*@param nBilles Le nombre de billes présentes sur le circuit
*@param tabBille[] Le tableau répertoriant toutes les billes du circuit 
*
*@return Le nombre de billes retirées du circuit multiplié par 10 afin de calculer le score associé à la disparition
*/

int disparaitre(int j, int borne1, int borne2, int nBilles, Bille tabBille[]) {
  for (int i = 0; i < nBilles - (borne1-borne2+1);i++){
        tabBille[borne2+i]=tabBille[borne1+1+i];
  }
  return ((nBilles-(borne1-borne2+1))*10);
}

/**
*Fonction permettant de provoquer une explosion dans le circuit, et une suite de combos s'il y en a 
*
*@param j Le rang de la bille qui a été lancée et qui provoque une potentielle explosion
*@param score Le score du joueur durant la partie, mis à jour si une explosion a eu lieu
*@param nBilles le nombre de billes présentes sur le circuit
*@param tabBille[] le tableau répertoriant toutes les billes du circuit
*
*@return newNBilles Le nombre de billes encore sur le circuit après une explosion (si une explosion a été effectuée, celui-ci diminue, sinon il reste inchangé)
*/

int explosion(int j, Bille tabBille[], int nBilles, int score) {
  int borne1 = j;
  int borne2 = j;
  int newNBilles = nBilles;
  do {
      borne1 = j ;
      borne2 = j ;
      while((borne1+1<nBilles) &&(tabBille[borne1+1].color == tabBille[j].color)) {
	    ++borne1;
      }
      while((borne2-1>=0) && (tabBille[borne2-1].color == tabBille[j].color)) {
	    --borne2;
      }
      if(borne1-borne2>=2) {
	    newNBilles = nBilles - (borne1-borne2+1);
	    score = disparaitre(j, borne1, borne2, nBilles, tabBille);
	    nBilles = newNBilles;
	    j = borne2-1;	  
      }
  } while(borne1-borne2>=2);
  return newNBilles;
}



int main() {
 

  //Constantes représentant la largeur et la hauteur de la fenêtre de jeu
  const int SCREENW=800;
  const int SCREENH=600;
  RenderWindow window(VideoMode(SCREENW, SCREENH), "ZUMA");

  //Initialisation de la grenouille ainsi que de sa position au centre du plateau
  Grenouille grenouille;
  grenouille.rayon = 64;
  grenouille.x = (SCREENW - grenouille.rayon/2) / 2;
  grenouille.y = (SCREENH - grenouille.rayon/2) / 2 ;
  grenouille.color = Color::Black;

  ////VARIABLES////
  //Les constantes sont initialisées au hasard (à changer)
  //constante nombre de billes créées en début de partie
  const int NBILLESINIT=100;

  //nombre de billes restantes sur le circuit 
  //initialisé grâce à la constante NBILLESINIT
  int nBilles=10;

  //vrai quand l'utilisateur a perdu (une bille atteint la fin du circuit) 
  //si vrai, le jeu s'arrête
  bool perdu = false;

  //vrai quand l'utilisateur a gagné (quand il a éliminé toutes les billes du circuit) 
  //si vrai, le jeu s'arrête
  bool gagne = false;
  
  //Vrai si on choisit de rejouer, et si la partie vient de commencer
  bool ecranA = true;

  //Constante modélisant la vitesse des billes sur le circuit
  const int SPEED=10;

  //Constante modélisant la vitesse de la bille lancée par l'utilisateur
  const int SPEEDLANCE=300;

  //Rayon de la bille
  const int RAYONBILLE=10;

  //Variable modélisant le score (initialisé à 0)
  int score = 0;

  //caractères associés aux couleurs
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

  //Variable dont on se sert afin de récupérer la direction de la bille à lancer 
  double hyp;
  
  //Initialisation de toutes les billes du tableau en début de partie 
  for (int i = 0; i < nBilles; ++i) {
      tabBille[i].x = 100-(20*i);
      tabBille[i].y = 10;
      tabBille[i].rayon = RAYONBILLE;
      tabBille[i].color = getRandomCol(n, tabCol);
  }
  
  //Pour ranger le tableau de couleurs disponibles et mettre a jour n 
  n=rangerTabCol(nBilles,tabBille,tabCol);

  //bille permettant de modéliser la bille en déplacement (qui a été lancée par l'utilisateur)
  Bille billeLance;
  billeLance.rayon = 10;
  billeLance.x = (SCREENW+260) / 2;
  billeLance.y = (SCREENH+120) / 2;
  billeLance.color = getRandomCol(n, tabCol);

  //bille permettant de modéliser la bille en réserve (qui peut être échangée avec la bille lance en cas d'appui sur la barre espace, et qui est la prochaine bille proposée pour être lancée)
  Bille billeReserve;
  billeReserve.rayon = 10;
  billeReserve.x = (SCREENW+430)/2;
  billeReserve.y = (SCREENH+300)/2;
  billeReserve.color = getRandomCol(n, tabCol);
 
 
  //Booléen qui prend la valeur true lorsqu'une bille lance est en déplacement (ce qui sert à empêcher l'utilisateur de modifier sa direction lorsqu'elle est déjà lancée)
  bool deplacer = false;
  
  //Position initiale de la bille qui est lancée
  double initx = (SCREENW+270) / 2;
  double inity =(SCREENH+130) / 2;
  
  //Variables qui servent à récupérer la position du clic de la souris lorsque l'utilisateur souhaite lancer une bille
  double sourisx = 0;
  double sourisy = 0;
  
  //Les textures
  Texture grenouilleTexture;
  Texture perduTexture;
  Texture victoireTexture;
  Texture ecranATexture;
  
  //Les sprite
  Sprite grenouilleSprite;
  Sprite perduSprite;
  Sprite victoireSprite;
  Sprite ecranASprite;
  
  //Création du sprite grenouille, qui sert à afficher l'image de la grenouille
  if(!grenouilleTexture.loadFromFile("Grenouille.png")) {
   	window.close();
    	cout <<"ERROR : texture failed to load.";
  }
  grenouilleSprite.setTexture(grenouilleTexture);
  grenouilleSprite.setPosition(grenouille.x-50, grenouille.y-200);
    
    
  //Création du sprite perdu, qui sert à afficher l'image de fin lorsque l'utilisateur a perdu
  if(!perduTexture.loadFromFile("perdu.png")) {
   	window.close();
    	cout <<"ERROR : texture failed to load.";
  }
  perduSprite.setTexture(perduTexture);
  perduSprite.setPosition(0,0);
  
  //Création du sprite victoire, qui sert à afficher l'image de fin lorsque l'utilisateur a gagné
  if(!victoireTexture.loadFromFile("victoire.png")) {
   	window.close();
    	cout <<"ERROR : texture failed to load.";
  }
  victoireSprite.setTexture(victoireTexture);
  victoireSprite.setPosition(0,0);


  //Création du sprite d'écran d'accueil, qui sert à afficher l'image de l'écran d'accueil en début de partie
  if(!ecranATexture.loadFromFile("ecranAccueil.png")) {
   	window.close();
    	cout <<"ERROR : texture failed to load.";
  }
  ecranASprite.setTexture(ecranATexture);
  ecranASprite.setPosition(0,0);

  Clock clock;

while (window.isOpen()) {

  Event event;
  while (window.pollEvent(event)) {

    //pour fermer la fenetre quand on clique sur la croix
    if (event.type == Event::Closed) {
        window.close();
    }
      
    //Pour récupérer les coordonnées du clic de la souris (afin de déplacer la bille lance) lorsqu'il n'y a pas de bille déjà lancée
    if ((event.type == sf::Event::MouseButtonPressed)&&(!deplacer)){
        deplacer=true;
        sourisx = event.mouseButton.x;
        sourisy = event.mouseButton.y;
    }

    //si l'on appuie sur la barre espace, les couleurs des billes de réserve et prête à être lancée sont échangées
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
     
  //Pour ranger le tableau de couleurs disponibles et mettre a jour n 
  n=rangerTabCol(nBilles,tabBille,tabCol);
   
  //pour que les billes attendent lorsqu'une explosion se produit
  for (int i = nBilles-1; i >=0; --i) {
     if (i==nBilles-1){
     	 tabBille[i].x += distance ;
     } else if (collision(tabBille[i+1],tabBille[i])){
     	 tabBille[i].x += distance;
     }
  }

  if (deplacer){
    if ((billeLance.x<0 )||(billeLance.x + billeLance.rayon > 800)||(billeLance.y<0 )||(billeLance.y + billeLance.rayon > 600)){
      	billeLance.x = initx;
      	billeLance.y = inity;
     	deplacer = false;
     	billeLance.color = getRandomCol(n, tabCol);
     	Color echange = billeLance.color;
	billeLance.color= billeReserve.color;
	billeReserve.color=echange;
     }
      
    for (int j = 0;j<nBilles; ++j){
      if(collision(billeLance, tabBille[j])){
	  ++nBilles;
	  incruster(billeLance, tabBille, j, nBilles);
	  int nAvant=n;
	  nBilles=explosion(j, tabBille, nBilles, score);
   	  n=rangerTabCol(nBilles,tabBille,tabCol);
          //pour retirer la couleur de la bille si l'explosion a retiré une couleur du circuit
   	  if(nAvant!=n){
	  	billeReserve.color=getRandomCol(n, tabCol);
   	}
	  
	//calcul de la victoire ou non de l'utilisateur (true s'il a gagné)
	if (nBilles == 0) {
	     gagne = true;
	}
	  
	billeLance.x = initx;
	billeLance.y = inity;
	billeLance.color = getRandomCol(n, tabCol);
	deplacer = false;
	Color echange = billeLance.color;
	billeLance.color= billeReserve.color;
	billeReserve.color=echange;
      }
      }
      hyp = sqrt((sourisx-initx)*(sourisx-initx)+(sourisy-inity)*(sourisy-inity));
      billeLance.x+= ((sourisx-initx)/hyp)*distanceLance;
      billeLance.y+= ((sourisy-inity)/hyp)*distanceLance;
    }
  

  //Pour savoir si on a perdu
  float dist = tabBille[0].x +tabBille[0].rayon;
  if (dist >= SCREENW) {
	perdu = true;
  }

  //couleur=la couleur de fond (a changer plus tard)
  window.clear(Color::White);
    
  //Affichage de la grenouille
  window.draw(grenouilleSprite);
   
  //Affichage billes:
  for (int i = 0; i < nBilles; ++i) {
   	CircleShape shape(tabBille[i].rayon);
    	shape.setPosition(tabBille[i].x-billeLance.rayon, tabBille[i].y-billeLance.rayon);
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
    
  //Affichage de l'écran final
  if(perdu) {
	window.draw(perduSprite);
  } else if(gagne) {
    	window.draw(victoireSprite);
  } 
  window.display();
}//fin de la while de window.isOpen
return 0;
}//fin de Main
