//Jeu réalisé dans le cadre du cours Projet d'initiation à l'ingénierie logiciel du parcours CMI Informatique par Hafsa Demnati, Éléa Jacquin et Marie-Almina Gindre.


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
*@param color : La variable de type Color qui donne la couleur d'une bille
*@param x : l'abscisse d'un objet
*@param y : l'ordonnée d'un objet 
*@param rayon : le rayon d'un cercle
*/
struct Bille {
  float x;
  float y;
  float rayon;
  Color color;
};

/*Structure permettant de rassembler le score et le nombre de billes dans une partie
*
*@param nBilles : Le nombre de billes présentes dans la partie
*@param score : Le score dans la partie
*
*/
struct Partie {
  int nBilles;
  int score;
};

/**
*
*Structure permettant de modéliser la grenouille
*
*@param x L'abscisse de la grenouille
*@param y L'ordonnée de la grenouille
*@param lance  La bille prête à être lancée par l'utilisateur
*@param reserve La bille en réserve (la bille qui sera *lancée par l'utilisateur après qu'il ait utilisé la bille lance)
*/
struct Grenouille{
  float x;
  float y;
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
*@param tabCol un tableau de caractères représentants les couleurs existantes 
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
int incruster(Bille billeLance, Bille tabBille[],int i, int nBilles){
  int newPlace = i;
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
   newPlace = i+1;
  }
  return newPlace ;
}

/**
*Fonction permettant de faire disparaître des billes du circuit
*
*@param j Le rang de la bille qui a été lancée et qui provoque une potentielle explosion
*@param borne1 Le nombre de billes de la même couleur que la bille lancée se plaçant avant dans le tableau
*@param borne2 Le nombre de billes de la même couleur que la bille lancée se plaçant après dans le tableau
*@param tabBille[] Le tableau répertoriant toutes les billes du circuit 
*@param partie La partie en cours
*
*@return Le nombre de billes retirées du circuit multiplié par 10 afin de calculer le score associé à la disparition
*/
int disparaitre(int j, int borne1, int borne2, Bille tabBille[], Partie partie) {
  for (int i = 0; i < partie.nBilles - (borne1-borne2+1);i++){
        tabBille[borne2+i]=tabBille[borne1+1+i];
  }
  return ((borne1-borne2)*10);
}

/**
*Fonction permettant de provoquer une explosion dans le circuit, et une suite de combos s'il y en a 
*
*@param j Le rang de la bille qui a été lancée et qui provoque une potentielle explosion
*@param partie La partie en cours comportant le score et le nombre de bille à mettre à jour
*@param tabBille[] le tableau répertoriant toutes les billes du circuit
*
*@return newNBilles Le nombre de billes encore sur le circuit après une explosion (si une explosion a été effectuée, celui-ci diminue, sinon il reste inchangé)
*/
Partie explosion(int j, Bille tabBille[], Partie partie) {
  int borne1 = j;
  int borne2 = j;
  int newNBilles = partie.nBilles;
  int i = 0;
  do {
      borne1 = j ;
      borne2 = j ;
      while((borne1+1<partie.nBilles) &&(tabBille[borne1+1].color == tabBille[j].color)) {
	    ++borne1;
      }
      while((borne2-1>=0) && (tabBille[borne2-1].color == tabBille[j].color)) {
	    --borne2;
      }
      if(borne1-borne2>=2) {
      	    i++;
	    newNBilles = partie.nBilles - (borne1-borne2+1);
	    partie.score += disparaitre(j, borne1, borne2, tabBille, partie);
	    if(i>1) {
	        partie.score +=10;
	    }
	    partie.nBilles = newNBilles;
	    j = borne2-1;	  
      }
  } while(borne1-borne2>=2);
  return partie;
}


int main() {
 

  //Constantes représentant la largeur et la hauteur de la fenêtre de jeu
  const int SCREENW=1200;
  const int SCREENH=900;
  RenderWindow window(VideoMode(SCREENW, SCREENH), "ZUMA");

  //Initialisation de la grenouille ainsi que de sa position au centre du plateau
  Grenouille grenouille;
  grenouille.x = (SCREENW - 32) / 2;
  grenouille.y = (SCREENH - 32) / 2;
  
  //Initialisation de la partie
  Partie partie;
  partie.nBilles = 10;
  partie.score = 0;
  
  
  ////VARIABLES////

  //constante nombre de billes créées en début de partie
  const int NBILLESINIT=100;

  //vrai quand l'utilisateur a perdu (une bille atteint la fin du circuit) 
  //si vrai, l'écran perdu s'affiche
  bool perdu = false;

  //vrai quand l'utilisateur a gagné (quand il a éliminé toutes les billes du circuit) 
  //si vrai, l'écran victoire s'affiche
  bool gagne = false;
  
  //Vrai si on choisit de rejouer, et si la partie vient de commencer
  bool ecranA = true;
  
  //vrai si on choisit de rejouer
  bool rejouer= false;

  //vrai si le joueur clique sur info
  bool info=false;

  //Entier modélisant la vitesse des billes sur le circuit
  int speed=10;

  //Constante modélisant la vitesse de la bille lancée par l'utilisateur
  const int SPEEDLANCE=500;

  //Rayon de la bille
  const int RAYONBILLE=15;
  
  //Nombre de parties jouées par le joueur
  int nbParties=1;

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
  for (int i = 0; i < partie.nBilles; ++i) {
      tabBille[i].x = 200-(2*RAYONBILLE*i);
      tabBille[i].y = 40;
      tabBille[i].rayon = RAYONBILLE;
      tabBille[i].color = getRandomCol(n, tabCol);
  }
  
  //Pour ranger le tableau de couleurs disponibles et mettre n à jour
  n=rangerTabCol(partie.nBilles,tabBille,tabCol);

  //bille permettant de modéliser la bille en déplacement (qui a été lancée par l'utilisateur)
  Bille billeLance;
  billeLance.rayon = RAYONBILLE;
  billeLance.x = (SCREENW+400) / 2;
  billeLance.y = (SCREENH+280) / 2;
  billeLance.color = getRandomCol(n, tabCol);

  //bille permettant de modéliser la bille en réserve (qui peut être échangée avec la bille lance en cas d'appui sur la barre espace, et qui est la prochaine bille proposée pour être lancée)
  Bille billeReserve;
  billeReserve.rayon = RAYONBILLE;
  billeReserve.x = (SCREENW+625)/2;
  billeReserve.y = (SCREENH+510)/2;
  billeReserve.color = getRandomCol(n, tabCol);
 
  //Booléen qui prend la valeur true lorsqu'une bille lance est en déplacement (ce qui sert à empêcher l'utilisateur de modifier sa direction lorsqu'elle est déjà lancée)
  bool deplacer = false;
  
  //Position initiale de la bille qui est lancée
  double initx = (SCREENW+400) / 2;
  double inity =(SCREENH+280) / 2;
  
  //Variables qui servent à récupérer la position du clic de la souris lorsque l'utilisateur souhaite lancer une bille
  double sourisx = 0;
  double sourisy = 0;
  
  //Les textures
  Texture grenouilleTexture;
  Texture perduTexture;
  Texture victoireTexture;
  Texture ecranATexture;
  Texture infoTexture;
  
  //Les sprite
  Sprite grenouilleSprite;
  Sprite perduSprite;
  Sprite victoireSprite;
  Sprite ecranASprite;
  Sprite infoSprite;
  
  //Création du sprite grenouille, qui sert à afficher l'image de la grenouille
  if(!grenouilleTexture.loadFromFile("images/Grenouille.png")) {
   	window.close();
    	cout <<"ERROR : texture failed to load.";
  }
  grenouilleSprite.setTexture(grenouilleTexture);
  grenouilleSprite.setPosition(grenouille.x-50, grenouille.y-200);
    
  //Création du sprite perdu, qui sert à afficher l'image de fin lorsque l'utilisateur a perdu
  if(!perduTexture.loadFromFile("images/perdu.png")) {
   	window.close();
    	cout <<"ERROR : texture failed to load.";
  }
  perduSprite.setTexture(perduTexture);
  perduSprite.setPosition(0,0);
  
  //Création du sprite victoire, qui sert à afficher l'image de fin lorsque l'utilisateur a gagné
  if(!victoireTexture.loadFromFile("images/victoire.png")) {
   	window.close();
    	cout <<"ERROR : texture failed to load.";
  }
  victoireSprite.setTexture(victoireTexture);
  victoireSprite.setPosition(0,0);

  //Création du sprite d'écran d'accueil, qui sert à afficher l'image de l'écran d'accueil en début de partie
  if(!ecranATexture.loadFromFile("images/ecranAccueil.png")) {
   	window.close();
    	cout <<"ERROR : texture failed to load.";
  }
  ecranASprite.setTexture(ecranATexture);
  ecranASprite.setPosition(0,0);
  
  //Création du sprite d'écran d'information, qui sert à afficher l'image de l'écran d'information à partir de l'écran d'accueil
  if(!infoTexture.loadFromFile("images/info.png")) {
   	window.close();
    	cout <<"ERROR : texture failed to load.";
  }
  infoSprite.setTexture(infoTexture);
  infoSprite.setPosition(0,0);
  
  //Font et texte pour les niveaux et le score
  String numNiveau = "Niveau : " + to_string(nbParties);
  Font font;
  if(!font.loadFromFile("arial.ttf")) {
  	window.close();
    	cout <<"ERROR : texture failed to load.";
  }
  
  Text text;
  text.setFont(font);
  text.setString(numNiveau);
  text.setCharacterSize(30);
  text.setFillColor(Color::Black);
  text.setPosition(40, 70);
  
  String scoreNum = "Score : " + to_string(partie.score);
  
  Text text2;
  text2.setFont(font);
  text2.setString(scoreNum);
  text2.setCharacterSize(30);
  text2.setFillColor(Color::Black);
  text2.setPosition(40, 100);
  
  
  Clock clock;

while (window.isOpen()) {

  Event event;
  while (window.pollEvent(event)) {

    //Pour récupérer les coordonnées du clic de la souris (afin de déplacer la bille lance) lorsqu'il n'y a pas de bille déjà lancée
    if ((event.type == sf::Event::MouseButtonPressed) and (!deplacer)and !ecranA and !rejouer and !info and !gagne and !perdu){
        deplacer=true;
        sourisx = event.mouseButton.x;
        sourisy = event.mouseButton.y;
    }

    //pour fermer la fenetre quand on clique sur la croix
    if (event.type == Event::Closed) {
        window.close();
    }
    
    //pour rejouer
    if ((event.type == sf::Event::MouseButtonPressed) and (perdu or gagne) and ((SCREENW-175<event.mouseButton.x) and(event.mouseButton.x<SCREENW)) and ((0<event.mouseButton.y) and (event.mouseButton.y<157))){
        rejouer=true;
    }  
      
    //Pour jouer a partir de l'écran d'accueil
    if ((event.type == sf::Event::MouseButtonPressed) and ecranA and ((0<event.mouseButton.x) and (event.mouseButton.x<385))and ((0<event.mouseButton.y)and(event.mouseButton.y<165))){
        ecranA=false;
    }  
    
     //Pour aller au infos à partir de l'écran d'accueil
     if ((event.type == sf::Event::MouseButtonPressed) and ecranA and ((0<event.mouseButton.x) and (event.mouseButton.x<84)) and ((SCREENH-86<event.mouseButton.y)and(event.mouseButton.y<SCREENH))){
        ecranA=false;
        info=true;
    }
    
    //Pour quitter les infos
   if ((event.type == sf::Event::MouseButtonPressed) and info and ((SCREENW-87<event.mouseButton.x) and (event.mouseButton.x<SCREENW)) and ((0<event.mouseButton.y)and(event.mouseButton.y<87))){
            ecranA=true;
            info=false;
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
  double distance = speed*dt;
  double distanceLance = SPEEDLANCE *dt;
     
  //Pour ranger le tableau de couleurs disponibles et mettre a jour n 
  n=rangerTabCol(partie.nBilles,tabBille,tabCol);
   
   //pour que les billes avancent et attendent lorsqu'une explosion se produit
   if(!ecranA){
 	 for (int i = partie.nBilles-1; i >=0; --i) {
    	   if (i==partie.nBilles-1){
    	 	 tabBille[i].x += distance ;
    	 	 tabBille[i].y = 40 + cos(tabBille[i].x);
   	   } else if (collision(tabBille[i+1],tabBille[i])){
    	 	 tabBille[i].x += distance;
   	  	     tabBille[i].y = 40 + cos(tabBille[i].x);
  	   }
	 }
  }

  if (deplacer and !ecranA and !info and !perdu and !gagne){
    if ((billeLance.x<0 )||(billeLance.x + billeLance.rayon > SCREENW)||(billeLance.y<0 )||(billeLance.y + billeLance.rayon > SCREENH)){
      	billeLance.x = initx;
      	billeLance.y = inity;
     	deplacer = false;
     	billeLance.color = getRandomCol(n, tabCol);
     	Color echange = billeLance.color;
	billeLance.color= billeReserve.color;
	billeReserve.color=echange;
     }
      
    for (int j = 0;j<partie.nBilles; ++j){
      if(collision(billeLance, tabBille[j])){
	  ++partie.nBilles;
	  j=incruster(billeLance, tabBille, j, partie.nBilles);
	  partie =explosion(j, tabBille, partie);
	  scoreNum = "Score : " + to_string(partie.score);
	  text2.setString(scoreNum);
	  int nAvant=n;
   	  n=rangerTabCol(partie.nBilles,tabBille,tabCol);
      //pour retirer la couleur de la bille si l'explosion a retiré une couleur du circuit
   	  if(nAvant!=n){
	  	billeReserve.color=getRandomCol(n, tabCol);
   	  }
	  
	  //calcul de la victoire ou non de l'utilisateur (true s'il a gagné)
	  if (partie.nBilles == 0) {
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
  window.clear(Color (164, 255, 237));
    
  //Affichage de la grenouille
  window.draw(grenouilleSprite);
   
  //Affichage billes:
  for (int i = 0; i < partie.nBilles; ++i) {
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
  
  //Affichage du niveau 
  window.draw(text);
    
  //Affichage du score
  window.draw(text2);

  //Affichage de l'écran final
  if(gagne) {
	window.draw(victoireSprite);
	text2.setPosition(40,50);
	window.draw(text2);
  	text2.setPosition(40, 100);
	partie.score = 0;
	} else if (perdu) {
	window.draw(perduSprite);
	text2.setFillColor(Color::White);
	text2.setPosition(40, 60);
	window.draw(text2);
	text2.setFillColor(Color::Black);
	text2.setPosition(40, 100);
	partie.score = 0;
  }
  
  //affichage de l'écran d'accueil 
  if(ecranA){
 	 window.draw(ecranASprite);
  }
  
  //affichage de l'écran d'informations
  if(info){
 	 window.draw(infoSprite);
  }
  
  //Remise à zero des variables nécéssaire pour rejouer et met a jour la difficulté suivant le niveau
  if(rejouer){
  	if(gagne){
  		nbParties+=1;
  		speed+=5;
 		partie.nBilles=10+nbParties*2;
 		numNiveau = "Niveau : " + to_string(nbParties);
 		text.setString(numNiveau);
  	}else if(perdu){
  		nbParties=1;
  		speed=10;
  		partie.nBilles=10;
 		numNiveau = "Niveau : " + to_string(nbParties);
 		text.setString(numNiveau);
  	}
	perdu=false;
	gagne=false;
	partie.score=0;
        scoreNum = "Score : " + to_string(partie.score);
        text2.setString(scoreNum);
	n=4;
	for (int i = 0; i < partie.nBilles; ++i) {
           tabBille[i].x = 200-(2*RAYONBILLE*i);
           tabBille[i].y = 40;
           tabBille[i].rayon = RAYONBILLE;
           tabBille[i].color = getRandomCol(n, tabCol);
 	}
 	rejouer=false;
 	ecranA=true;
  }
  
  window.display();
}//fin de la while de window.isOpen
return 0;
}//fin de Main
