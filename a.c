#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
	int id_proc, i, j, k, l, m, w, x, y;

	for(i=0; i<2; i++){//2 procesos
		if((id_proc=fork())==0){//Creo 2 procesos verticales
			if(i==1){//Segundo hijo
				for(j=0; j<3; j++){//3 hijos
					if((id_proc=fork())==0){//Creo 3 procesos horizontales
						if(j==0){//Primer hijo - izquierda
							for(k=0; k<2; k++){//2 hijos
								if((id_proc=fork())==0){//Creo 2 procesos horizontales
									if(k==0){//Primer hijo - izquierda
										if((id_proc=fork())==0){//Creo 1 proceso vertical

										}
										else{
											break;
											exit(0);
										}//ACABAMOS
									}
									else if(k==1){//Segundo hijo - derecha
										if((id_proc=fork())==0){//Creo 1 proceso vertical
											for(l=0; l<2; l++){//2 hijos
												if((id_proc=fork())==0){//Creo 2 procesos horizontales
													exit(0);//ACABAMOS
												}
											}
										}
										else{
											break;
										}
									}
									exit(0);
								}
							}

						}
						else if(j==1){//Segundo hijo - centro
							if((id_proc=fork())==0){//Creo 1 proceso vertical

							}
							else{
								break;
								exit(0);
							}//ACABAMOS
						}
						else if(j==2){//Tercer hijo - derecho
							for(m=0; m<2; m++){//2 procesos
								if((id_proc=fork())==0){//Creamos 2 procesos horizontales
									if(m==0){//Primer hijo - izquierda
										if((id_proc=fork())==0){//Creo 1 proceso vertical
											for(w=0; w<2; w++){//2 hijos
												if((id_proc=fork())==0){//Creo 2 procesos horizontales
													exit(0); //ACABAMOS
												}
											}
										}
										else{
											break;
										}
									}
									else if(m==1){//Segundo hijo - derecha
										for(x=0; x<2; x++){//2 procesos
											if((id_proc=fork())==0){//Creo 2 procesos verticales
												if(x==1){//Segundo proceso vertical
													for(y=0; y<4; y++){//4 procesos
														if((id_proc=fork())==0){//Creamos 4 procesos horizontales
														exit(0);//ACABAMOS
													}
												}
											}
											else{
												break;
											}
										}
									}
									break;
									exit(0);
								}
							}
						}
						exit(0);
					}
				}
			}
		}
		else{
			break;
		}
	}
}