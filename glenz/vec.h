#ifndef U2_VEC_H_
#define U2_VEC_H_

int csetmatrix(int *,int,int,int);
int crotlist(int *,int *);
//int cclipedges(int *,int *,int *); // modifies given point list
int cprojlist(int *,int *);
int ccliplist(int *);
//int cdrawpolylist(int *);
int cmatrix_yxz(int,int,int,int *);
//int cpolylist(int *polylist,int *polys,int *edges,int *points3);
//int ceasypolylist(int *polylist,int *polys,int *points3);

void cglenzpolylist(int *polylist);
void cglenzinit(void);

#endif
