/*
 * Pour compiler ce code, il suffit de taper `make` dans un terminal.
 * Si tout ce passe bien, un exécutable du nom `skel` apparaît dans
 * le répertoire et vous pouvez l'exécuter en tapant `./skel`.
 *
 * Ne faites pas attention au warning qui vous indique que la variable
 * `dt` n'est pas utilisée. C'est normal pour l'instant.
 */

#include <random>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

/*
 * Ce morceau de code pour permet de tirer un nombre flottant au hasard
 * entre 0.0 et 1.0. Pour cela, il suffit de l'appeler de la manière
 * suivante:
 *
 *     float r = Math::random();
 */
struct Math {
  static float random() {
    static mt19937 engine(time(nullptr));
    uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(engine);
  }
};

/*
 * La structure Rectangle contient des champs pour manipuler un rectangle
 * avec sa position (x,y) qui correspond au coin en haut à gauche, sa
 * taille width x height, et sa couleur.
 *
 * Le type Color correspond à une structure de SFML:
 * http://www.sfml-dev.org/documentation/2.1/classsf_1_1Color.php
 *
 * Il existe des constantes pour les principales couleurs.
 *
 * Pour pouvoir dessiner des formes, vous pouvez lire le tutoriel suivant:
 * http://www.sfml-dev.org/tutorials/2.1/graphics-shape.php
 */
struct Rectangle {
  float x;
  float y;
  float width;
  float height;
  Color color;
};

// Question 3.3: fonction permettant de faire tourner les couleurs
Color toggleColor(Color color) {
  if (color == Color::Red) {
    return Color::Green;
  }

  if (color == Color::Green) {
    return Color::Blue;
  }

  if (color == Color::Blue) {
    return Color:: Red;
  }

  return Color::Red;
}

// Question 3.4: fonction de collision entre deux rectangles
bool collision(Rectangle rect1, Rectangle rect2) {
  return !(rect1.x + rect1.width <= rect2.x ||
           rect1.x >= rect2.x + rect2.width ||
           rect1.y + rect1.height <= rect2.y ||
           rect1.y >= rect2.y + rect2.height);
}

// Question 3.5: fonction de collision entre le rectangle et les obstacles
bool collisionWithObstacles(Rectangle rect, Rectangle obs[5]) {
  for (int i = 0; i < 5; ++i) {
    if (collision(rect, obs[i])) {
      return true;
    }
  }

  return false;
}

int main() {
  /*
   * Une RenderWindow est une fenêtre qui permet de récupérer des événements
   * d'entrée (comme le clavier et la souris) et d'afficher des entités.
   *
   * La documentation se trouve ici:
   * http://www.sfml-dev.org/documentation/2.1/classsf_1_1RenderWindow.php
   */
  RenderWindow window(VideoMode(800, 600), "Titre");
  Vector2u size = window.getSize();

  // Question 3.1: on définit un rectangle
  Rectangle rect;
  rect.width = rect.height = 64;
  rect.x = (size.x - rect.width) / 2;
  rect.y = (size.y - rect.height) / 2;
  rect.color = Color::Red;

  // Question 3.4: on définit un tableau de rectangle
  Rectangle obstacles[5];

  for (int i = 0; i < 5; ++i) {
    do {
      obstacles[i].x = Math::random() * size.x;
      obstacles[i].y = Math::random() * size.y;
      obstacles[i].width = (Math::random() * 44) + 20;
      obstacles[i].height = (Math::random() * 44) + 20;
    } while (obstacles[i].x > size.x - obstacles[i].width ||
             obstacles[i].y > size.y - obstacles[i].height ||
             collision(obstacles[i], rect));

    obstacles[i].color = Color::Black;
  }

  // Question 3.2: on définit des variables pour savoir quelles touches sont appuyées
  bool leftKey = false;
  bool rightKey = false;
  bool upKey = false;
  bool downKey = false;

  /*
   * Une Clock permet de compter le temps. Vous en aurez besoin pour savoir
   * le temps entre deux frames.
   */
  Clock clock;

  /*
   * La boucle de jeu principale. La condition de fin est la fermeture de la
   * fenêtre qu'on provoque à l'aide d'un appel `window.close()`.
   */
  while (window.isOpen()) {
    /*
     * Un Event est un événement d'entrée. Il contient toutes les informations
     * nécessaires pour traiter tous les événements.
     *
     * Vous pouvez lire ce tutoriel pour comprendre comment récupérer les
     * informations relatives à chaque événement:
     * http://www.sfml-dev.org/tutorials/2.1/window-events.php
     */
    Event event;

    /*
     * Cette boucle permet de traiter tous les événements en attente.
     */
    while (window.pollEvent(event)) {
      /*
       * L'événement Event::Closed est reçu quand on clique sur la croix
       * dans la barre de la fenêtre. À ce moment là, il faut fermer la
       * fenêtre explicitement.
       */
      if (event.type == Event::Closed) {
        window.close();
      }

      /*
       * Les événements dont vous aurez besoin sont Event::KeyPressed,
       * Event::KeyReleased et Event::MouseButtonPressed.
       */

      if (event.type == Event::KeyPressed) {
        switch (event.key.code) {
          case Keyboard::Up:
            upKey = true; downKey = false;
            break;

          case Keyboard::Down:
            downKey = true; upKey = false;
            break;

          case Keyboard::Left:
            leftKey = true; rightKey = false;
            break;

          case Keyboard::Right:
            rightKey = true; leftKey = false;
            break;

          default:
            break;
        }
      }

      if (event.type == Event::KeyReleased) {
        switch (event.key.code) {
          case Keyboard::Up:
            upKey = false;
            break;

          case Keyboard::Down:
            downKey = false;
            break;

          case Keyboard::Left:
            leftKey = false;
            break;

          case Keyboard::Right:
            rightKey = false;
            break;

          default:
            break;
        }
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        // Question 3.3: on vérifie que la souris a cliqué au bon endroit
        if (event.mouseButton.x >= rect.x &&
            event.mouseButton.x <= rect.x + rect.width &&
            event.mouseButton.y >= rect.y &&
            event.mouseButton.y <= rect.y + rect.height) {
          rect.color = toggleColor(rect.color);
        }
      }
    }

    /*
     * Mise à jour de l'état du jeu.
     * On calcule le temps (en secondes) depuis la dernière frame qu'on place
     * dans la variable `dt`. Ensuite, il faut compléter suivant ce qui est
     * demandé.
     */
    float dt = clock.restart().asSeconds();


    // Question 3.2: on déplace le rectangle suivant les touches
    // Question 3.5: on prend en compte les collisions avec les obstacles
    const float speed = 60.0f;
    float distance = speed * dt;

    if (leftKey && rect.x > 0) {
      rect.x -= distance;

      if (collisionWithObstacles(rect, obstacles)) {
        rect.x += distance;
      }
    }

    if (rightKey && rect.x + rect.width < size.x) {
      rect.x += distance;

      if (collisionWithObstacles(rect, obstacles)) {
        rect.x -= distance;
      }
    }

    if (upKey && rect.y > 0) {
      rect.y -= distance;

      if (collisionWithObstacles(rect, obstacles)) {
        rect.y += distance;
      }
    }

    if (downKey && rect.y + rect.height < size.y) {
      rect.y += distance;

      if (collisionWithObstacles(rect, obstacles)) {
        rect.y -= distance;
      }
    }

    /*
     * Affichage de l'état du jeu.
     * À chaque tour de boucle, on efface tout grâce à `clear` (qui prend
     * en paramètre la couleur de fond), puis on dessine tous les éléments,
     * puis on affiche la nouvelle image grâce à `display`.
     */
    window.clear(Color::White);

    // Question 3.1: on l'affiche à l'aide d'un RectangleShape
    RectangleShape shape;
    shape.setSize({ rect.width, rect.height });
    shape.setPosition(rect.x, rect.y);
    shape.setFillColor(rect.color);
    window.draw(shape);

    // Question 3.4: on affiche les obstacles
    for (int i = 0; i < 5; ++i) {
      RectangleShape shape;
      shape.setSize({ obstacles[i].width, obstacles[i].height });
      shape.setPosition(obstacles[i].x, obstacles[i].y);
      shape.setFillColor(obstacles[i].color);
      window.draw(shape);
    }

    window.display();

  }

  return 0;
}
