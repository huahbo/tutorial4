#include <mpi.h>
#include "helper.h"
#include "visual.h"
#include <stdio.h>


void output_uvp(
                double **U,
                double **V,
                double **P,
                int il,
                int ir,
                int jb,
                int jt,
                int omg_i,
                int omg_j,
                char *szProblem,
                int timeStepNumber,
                double dx,
                double dy,
                int rank,
                int imax,
                int jmax){
    
    int i,j ;
    char szFileName[80];
    FILE *fp=NULL;
    sprintf( szFileName, "%s.%i.%i.vtk", szProblem, rank, timeStepNumber );
    fp = fopen( szFileName, "w");
    if( fp == NULL )
    {
        char szBuff[80];
        sprintf( szBuff, "Failed to open %s", szFileName );
        ERROR( szBuff );
        return;
    }
    
    write_vtkHeader( fp, il, ir, jb, jt, dx, dy,imax,jmax);
    write_vtkPointCoordinates(fp, il, ir, jb, jt, dx, dy,imax,jmax,omg_i,omg_j);
    
    fprintf(fp,"POINT_DATA %i \n", ((ir)-(il-1)+1)*((jt)-(jb-1)+1) );
	
    fprintf(fp,"\n");
    fprintf(fp, "VECTORS velocity float\n");
    
    
    for(j = (jb-1); j <= (jt); j++) {
        for(i = (il-1); i <= (ir); i++) {
            fprintf(fp, "%f %f 0\n", (U[i][j] + U[i][j+1]) * 0.5, (V[i][j] + V[i+1][j]) * 0.5 );
        }
    }
    
    fprintf(fp,"\n");
    fprintf(fp,"CELL_DATA %i \n", ((ir-(il)+1)*(jt-(jb)+1)) );
    fprintf(fp, "SCALARS pressure float 1 \n");
    fprintf(fp, "LOOKUP_TABLE default \n");
    for(j = (jb); j <= jt; j++) {
        for(i = (il); i <= ir; i++) {
            fprintf(fp, "%f\n", P[i][j] );
        }
    }
    
    if( fclose(fp) )
    {
        char szBuff[80];
        sprintf( szBuff, "Failed to close %s", szFileName );
        ERROR( szBuff );
    }
}


void write_vtkHeader(
                     FILE *fp,
                     int il,
                     int ir,
                     int jb,
                     int jt,
                     double dx,
                     double dy,
                     int imax,
                     int jmax) {
    if( fp == NULL )
    {
        char szBuff[80];
        sprintf( szBuff, "Null pointer in write_vtkHeader" );
        ERROR( szBuff );
        return;
    }
    
    fprintf(fp,"# vtk DataFile Version 2.0\n");
    fprintf(fp,"generated by CFD-lab course output (written by Tobias Neckel) \n");
    fprintf(fp,"ASCII\n");
    fprintf(fp,"\n");
    fprintf(fp,"DATASET STRUCTURED_GRID\n");
    fprintf(fp,"DIMENSIONS  %i %i 1 \n", ir-(il-1)+1, jt-(jb-1)+1);
    fprintf(fp,"POINTS %i float\n", (ir-(il-1)+1)*(jt-(jb-1)+1) );
    fprintf(fp,"\n");
}


void write_vtkPointCoordinates(
                               FILE *fp,
                               int il,
                               int ir,
                               int jb,
                               int jt,
                               double dx,
                               double dy,
                               int imax,
                               int jmax,
                               int omg_i,
                               int omg_j
                               ) {
    double originX;
    double originY;
	int myrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    
    originX = (il)*dx; /*-(omg_i-1)*dx;*/
    originY = (jb)*dy; /*-(omg_j-1)*dy;*/
    
    int i = 0;
    int j = 0;
    
    for(j = 0; j <= jt-jb+1; j++) {
        for(i = 0; i <= ir-il+1; i++) {
            fprintf(fp, "%f %f 0\n", originX+(i*dx), originY+(j*dy) );
        }
    }
}
