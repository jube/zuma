#include <random>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Math {
  static float random() {
    static mt19937 engine(time(nullptr));
    uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(engine);
  }
};

struct Rond {
  float x;
  float y;
  float width;
  float height;
  Color color;
};

int main() {

  RenderWindow window(VideoMode(800, 600), "Titre");
  Vector2u size = window.getSize();

  // Question 3.1: on définit un rectangle
  Rond ron;
  ron.width = ron.height = 64;
  ron.x = (size.x - ron.width/2) / 2;
  ron.y = (size.y - ron.height/2) / 2 ;
  ron.color = Color::Red;

  Rond lan;
  lan.width = lan.height = 10;
  lan.x = (size.x ) / 2;
  double initx = (size.x ) / 2;
  double inity = (size.y ) / 2;
  lan.y = (size.y ) / 2;
  lan.color = Color::Black;

  bool deplacer = false;

  double sourisx = 0;
  double sourisy = 0;

  Clock clock;

  while (window.isOpen()) {

    Event event;

    while (window.pollEvent(event)) {
      if (event.type == Event::Closed) {
        window.close();
      }


      if ((event.type == sf::Event::MouseButtonPressed )&& (deplacer==false)) {
        //deplacer(lan.x,lan.y,event.mouseButton.x, event.mouseButton.y)
        deplacer=true;
        sourisx = event.mouseButton.x;
        sourisy = event.mouseButton.y;
      }
    }

    float dt = clock.restart().asSeconds();
    const float speed = 150.0f;
    float distance = speed * dt;
    double hyp ;

    if (deplacer){
      if ((lan.x<0 )||(lan.x + lan.width > 800)||(lan.y<0 )||(lan.y + lan.width > 600)){
      lan.x = initx;
      lan.y = inity;
      deplacer = false;
      }
      hyp = sqrt((sourisx-initx)*(sourisx-initx)+(sourisy-inity)*(sourisy-inity));
      //if(sourisx-initx<0){
       // lan.x-= ((sourisx-initx)/hyp)*distance;
	//lan.y-= ((sourisy-inity)/hyp)*distance;
     // }else{
       // lan.x+= cos(3.14)*distance;
       // lan.y+= sin(3.14)*distance;

	lan.x+= ((sourisx-initx)/hyp)*distance;
        lan.y+= ((sourisy-inity)/hyp)*distance;
     // }
      
      //lan.y+= ((sourisy-inity)/(sourisx-initx))*distance;
    }



   // const float speed = 60.0f;

    window.clear(Color::White);

    // Question 3.1: on l'affiche à l'aide d'un RectangleShape
    
    CircleShape shape(ron.width);
    shape.setPosition(ron.x-ron.width, ron.y-ron.width);
    shape.setFillColor(ron.color);
    window.draw(shape);

    CircleShape lancer(lan.width);
    lancer.setPosition(lan.x-lan.width, lan.y-lan.width);
    lancer.setFillColor(lan.color);
    window.draw(lancer);

    window.display();

  }

  return 0;
}
