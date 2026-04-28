#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include <cstdio>

// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB

class moving_image {
private:
  unsigned char **red_layer;
  unsigned char **green_layer;
  unsigned char **blue_layer;

public:
// Constructor de la imagen. Se crea una imagen por defecto
  moving_image() {
    // Reserva de memoria para las 3 matrices RGB
    red_layer = new unsigned char*[H_IMG];
    green_layer = new unsigned char*[H_IMG];
    blue_layer = new unsigned char*[H_IMG];

    for(int i = 0; i < H_IMG; i++) {
      red_layer[i] = new unsigned char[W_IMG];
      green_layer[i] = new unsigned char[W_IMG];
      blue_layer[i] = new unsigned char[W_IMG];
    }

    // Llenamos la imagen con su color de fondo
    for(int i = 0; i < H_IMG; i++) {
      for(int j = 0; j < W_IMG; j++) {
        red_layer[i][j] = DEFAULT_R;
        green_layer[i][j] = DEFAULT_G;
        blue_layer[i][j] = DEFAULT_B;
      }
    }

    // Dibujamos el objeto en su posición inicial
    for(int i = 0; i < 322; i++) {
      for(int j = 0; j < 256; j++) {
        if(!s_R[i][j] && !s_G[i][j] && !s_B[i][j]) {
          red_layer[INIT_Y + i][INIT_X + j] = DEFAULT_R;
          green_layer[INIT_Y + i][INIT_X + j] = DEFAULT_G;
          blue_layer[INIT_Y + i][INIT_X + j] = DEFAULT_B;
        } else {
          red_layer[INIT_Y + i][INIT_X + j] = s_R[i][j];
          green_layer[INIT_Y + i][INIT_X + j] = s_G[i][j];
          blue_layer[INIT_Y + i][INIT_X + j] = s_B[i][j];
        }
      }
    }
  }

  // Destructor de la clase (el codigo antigo el delete estaba sin [] pero segun yo esa wea no libera todo el arreglo, por ahora lo voy a dejar asi)
  ~moving_image() {
    for(int i = 0; i < H_IMG; i++) {
      delete[] red_layer[i];
      delete[] green_layer[i];
      delete[] blue_layer[i];
    }

    delete[] red_layer;
    delete[] green_layer;
    delete[] blue_layer;
  }

  // Función utilizada para guardar la imagen en formato .png
  void draw(const char* nb) {
    _draw(nb);
  }

  // mover a la izquierda
  void move_left(int d) {
    int shift = normalize_shift(d, W_IMG);
    //aplicado a las 3 capas del RGB
    shift_left_layer(red_layer, shift);
    shift_left_layer(green_layer, shift);
    shift_left_layer(blue_layer, shift);
  }

  // mover a la derecha (es equivalente a mover a la izquierda: wide - distant normalizado)
  void move_right(int d) {
    int shift = normalize_shift(d, W_IMG);
    int left_shift = normalize_shift(W_IMG - shift, W_IMG);
    //aplicado a las 3 capas del RGB
    shift_left_layer(red_layer, left_shift);
    shift_left_layer(green_layer, left_shift);
    shift_left_layer(blue_layer, left_shift);
  }
  
  // mover hacia arriba
  void move_up(int d) {
    int shift = normalize_shift(d, H_IMG);
    //aplicado a las 3 capas del RGB
    shift_up_layer(red_layer, shift);
    shift_up_layer(green_layer, shift);
    shift_up_layer(blue_layer, shift);
  }

  // mover hacia abajo (es equivalente a mover hacia arriba: height - distant normalizado)
  void move_down(int d) {
    int shift = normalize_shift(d, H_IMG);
    int up_shift = normalize_shift(H_IMG - shift, H_IMG);
    //aplicado a las 3 capas del RGB
    shift_up_layer(red_layer, up_shift);
    shift_up_layer(green_layer, up_shift);
    shift_up_layer(blue_layer, up_shift);
  }

  // rotacion 90 grados antihoraria 
  void rotate() {
    //aplicado a las 3 capas del RGB
    rotate_ccw_layer(red_layer);
    rotate_ccw_layer(green_layer);
    rotate_ccw_layer(blue_layer);   
  }

private:
  // convierte "d" a un rango valido [0, limit-1]
  int normalize_shift(int d, int limit) {
    int n = d % limit;
    if(n < 0)
      n += limit;
    return n;
  }

  // matriz temporal del mismo tamaño donde hacer las transformaciones
  unsigned char **create_temp_layer() {
    unsigned char **tmp = new unsigned char*[H_IMG];
    for(int i = 0; i < H_IMG; i++)
      tmp[i] = new unsigned char[W_IMG];
    return tmp;
  }

  // libera la matriz termporal 
  void destroy_temp_layer(unsigned char **tmp) {
    for(int i = 0; i < H_IMG; i++)
      delete[] tmp[i];
    delete[] tmp;
  }

  // desplazamiento circular a la izquierda en una matriz (se aplicara a cada capa del rgb)
  void shift_left_layer(unsigned char **layer, int d) {
    if(d == 0)
      return;

    unsigned char **tmp = create_temp_layer();

    for(int i = 0; i < H_IMG; i++) {
      for(int j = 0; j < W_IMG - d; j++)
        tmp[i][j] = layer[i][j + d];

      for(int j = W_IMG - d, k = 0; j < W_IMG; j++, k++)
        tmp[i][j] = layer[i][k];
    }

    for(int i = 0; i < H_IMG; i++)
      for(int j = 0; j < W_IMG; j++)
        layer[i][j] = tmp[i][j];

    destroy_temp_layer(tmp);
  }

  // desplazamiento circular hacia arriba en una matriz (se aplicara a cada capa del rgb)
  void shift_up_layer(unsigned char **layer, int d) {
    if(d == 0)
      return;

    unsigned char **tmp = create_temp_layer();

    for(int i = 0; i < H_IMG - d; i++)
      for(int j = 0; j < W_IMG; j++)
        tmp[i][j] = layer[i + d][j];

    for(int i = H_IMG - d, k = 0; i < H_IMG; i++, k++)
      for(int j = 0; j < W_IMG; j++)
        tmp[i][j] = layer[k][j];

    for(int i = 0; i < H_IMG; i++)
      for(int j = 0; j < W_IMG; j++)
        layer[i][j] = tmp[i][j];

    destroy_temp_layer(tmp);
  }

  // rota una matriz (cuadrada) 90 grados en sentido antihorario
  void rotate_ccw_layer(unsigned char **layer) {
    if(H_IMG != W_IMG)
      return;

    unsigned char **tmp = create_temp_layer();

    for(int y = 0; y < H_IMG; y++)
      for(int x = 0; x < W_IMG; x++)
        tmp[H_IMG - 1 - x][y] = layer[y][x];

    for(int i = 0; i < H_IMG; i++)
      for(int j = 0; j < W_IMG; j++)
        layer[i][j] = tmp[i][j];

    destroy_temp_layer(tmp);
  }

  // Función privada que guarda la imagen en formato .png
  void _draw(const char* nb) {
    //    unsigned char rgb[H_IMG * W_IMG * 3], *p = rgb;
    unsigned char *rgb = new unsigned char[H_IMG * W_IMG * 3];
    unsigned char *p = rgb;

    // La imagen resultante tendrá el nombre dado por la variable 'nb'
    FILE *fp = fopen(nb, "wb");
    // si no se pudo abrir el archivo libera la memoria y retorna
    if(!fp) {
      delete[] rgb;
      return;
    }

    // Transformamos las 3 matrices en una arreglo unidimensional
    for(unsigned y = 0; y < H_IMG; y++) {
      for(unsigned x = 0; x < W_IMG; x++) {
          *p++ = red_layer[y][x];    /* R */
          *p++ = green_layer[y][x];    /* G */
          *p++ = blue_layer[y][x];    /* B */
      }
    }

    // La función svpng() transforma las 3 matrices RGB en una imagen PNG 
    svpng(fp, W_IMG, H_IMG, rgb, 0);
    fclose(fp);
    delete[] rgb; // libera la memoria del arreglo 
  }
};

#endif
