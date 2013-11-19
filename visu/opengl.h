
int init_opengl(int width, int height);
void set_window(int, int, int, int);
void projection(void);
void draw_palette(void);
void draw_poly(short *, int, int);
void draw_poly_diffuse(short *, int *, int, int);
void setrgb(int, int, int, int);
void getrgb(int, char *);
void draw_rectangle1(void);
void draw_rectangle2(void);
void setrgba(int, int, int, int, float);
