/* Compilación: g++ -o ejecutable test.cpp
 * Ejecución: ./ejecutable
 *
 * Luego de la ejecución se generarán 8 imagenes.png y una extra que se actualizara en forma de loop, con 
 * mario moviendose hacia la derecha, la que pueden parar con Ctrl+C (o cerrando la terminal)
 */

#include "moving_image.h"
#include<unistd.h> // para sleep (linux). Usar  #include<windows.h> para Windows


int main() {
  moving_image im;

  // original
  im.draw("outputs/original.png");

  // prueba direcciones 
  im.move_left(200);
  im.draw("outputs/left_200.png");
  im.move_right(200);

  im.move_right(200);
  im.draw("outputs/right_200.png");
  im.move_left(200);

  im.move_up(200);
  im.draw("outputs/up_200.png");
  im.move_down(200);

  im.move_down(200);
  im.draw("outputs/down_200.png");
  im.move_up(200);


  // prueba rotacion
  im.rotate();
  im.draw("outputs/rot_1.png");

  im.rotate();
  im.draw("outputs/rot_2.png");

  im.rotate();
  im.draw("outputs/rot_3.png");

  im.rotate();
  im.draw("outputs/rot_4.png");

  // prueba de undo
  im.undo();
  im.draw("outputs/undo_1.png");
 
  im.undo();
  im.draw("outputs/undo_2.png");

  // prueba de redo
  im.redo();
  im.draw("outputs/redo_1.png");

  // prueba de repeat
  im.repeat();
  im.draw("outputs/repeat_1.png");

  // prueba de repeat_all
  im.repeat_all();

  // loop
  while(true) {
    im.draw("outputs/loop.png");
    im.move_right(10);
    im.rotate();
  }

  /* NOTA 1: Si usan el mismo nombre para las imágenes, entonces cada llamada al
  método draw() sobreescribirá a la imagen */

  return 0;
}