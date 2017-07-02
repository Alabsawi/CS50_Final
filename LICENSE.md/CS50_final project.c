/*
* File:  forward kinemtics calculator.c
* Author: Abdullah Alabsawi
* Created: 17.6.2017
* Last edit: 01.07.2017
* E-mail: abdullah.alabsawi@gmail.com
* Variant: robot arm forward kinemtics calculator
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#define PI 3.14159265

typedef struct offset//structure for the offsets between cordinate systems
{
	float X;
	float Y;
	float Z;
} offset;

typedef struct angle
{
	float alpha; //angle of rotation around the X axis
	float beta; //angle of rotation around the Y axis
	float gama; //angle of rotation around the Z axis
}angle;

typedef struct CS //containing details about the movie
{
	int order;
	char name [24];
	offset off;
	angle ang;
	struct CS* next_CS;
}CS;

float check_input ();
int get_CS (CS** head,float *X,float *Y,float *Z);
int ask_rotation_offset(char q [20]); //return 1 if yes

int main (void)
{
	float Xf,Yf,Zf,Xs,Ys,Zs;
	CS* head = (CS*) malloc (100*sizeof(CS)); // free it later on
	if (head == NULL)
	{
		printf ("memory allocation failed");
		exit (2);
	}
	get_CS(&head,&Xf,&Yf,&Zf);
	CS* curr;	
	
	while ((curr = head) != NULL) { // set curr to head, stop if list empty.
    	head = head->next_CS;          // advance head to next element.
    	free (curr);                // delete saved pointer.
	}
	
	printf ("This is the X-axis cordinates %.2f\n",Xf);
    printf ("This is the Y-axis cordinates %.2f\n",Yf);
    printf ("This is the Z-axis cordinates %.2f\n",Zf);

}

//takes a pointer to the head of the CS linked list and pointers to cordinates variables
//The function fills the CSs linked list and calculates each transformation matrrix and multiply them to get the final transformation matrix
int get_CS (CS** head,float* X,float*Y,float*Z)
{
	//cordinates
	float *px = X;
	float *py = Y;
	float *pz = Z;
	
	//To be used as counters and verification variables	
	int order = 0, u = 0, i = 0 ,k = 0, j = 0, op, p=0,l=0;;
	CS* current = *head;
	char q [20] = "rotation";
	float mul [4][4], prod [4][4];

	//pointer to transformation matrices
	float ***matrices = (float***) malloc(100*sizeof(float***));
	while (1)
	{
		printf ("Cordinate system number %d:\n",order);

		//to check if there are any rotations
		strcpy (q,"rotations");
		u = ask_rotation_offset (q);
		
		if (u==1)
		{
			printf("Angle of rotation around X-axis = ");
			(*current).ang.alpha = (PI/180)*check_input ();
			
			printf("Angle of rotation around Y-axis = ");
			(*current).ang.beta = (PI/180)*check_input ();

			printf("Angle of rotation around Z-axis = ");
			(*current).ang.gama = (PI/180)*check_input ();		
		}
		else
		{
			(*current).ang.alpha = 0;	
			(*current).ang.beta = 0;
			(*current).ang.gama = 0;	
		}
		
	//to check if there are any offsets
		strcpy (q,"offsets");
		u = ask_rotation_offset (q);
		
		if (u==1)
		{
			printf("Offset in X-axis direction = ");
			(*current).off.X = check_input ();
			
			printf("Offset in Y-axis direction= ");
			(*current).off.Y = check_input ();

			printf("Offset in Z-axis direction = ");
			(*current).off.Z = check_input ();		
		}
		else
		{
			(*current).off.X = 0;
			(*current).off.Y = 0;
			(*current).off.Z = 0;
		}				
		
		//pointer to the matrix of this loop
		matrices [order] = (float**) malloc (100*sizeof (float**));//free it
		if (matrices[order] == NULL)
		{
			printf ("memory allocation failed");
			exit (2);
		}
		
		//pointers to rows in the this loop matrix
		for (i=0; i < 4;i++)
		{
			matrices [order][i] = (float*) malloc (100*sizeof (float*));//free it
			if (matrices[order][i] == NULL)
			{
				printf ("memory allocation failed");
				exit (2);
			}
		}

		
		//Matrix
		//standard tranformation matrices
		// first column
		matrices [order][0][0] = cos((*current).ang.gama)*cos((*current).ang.beta);//0
		matrices [order][1][0] = cos((*current).ang.alpha)*sin((*current).ang.gama)+cos((*current).ang.gama)*sin((*current).ang.alpha)*sin((*current).ang.beta);//-1
		matrices [order][2][0] = sin((*current).ang.alpha)*sin((*current).ang.gama)-cos((*current).ang.alpha)*cos((*current).ang.gama)*sin((*current).ang.beta);//0
		matrices [order][3][0] = 0;
		
		//second column
		matrices [order][0][1] = -cos((*current).ang.beta)*sin((*current).ang.gama);
		matrices [order][1][1] = cos((*current).ang.alpha)*cos((*current).ang.gama)-sin((*current).ang.alpha)*sin((*current).ang.beta)*sin((*current).ang.beta);
		matrices [order][2][1] = cos((*current).ang.gama)*sin((*current).ang.alpha)+cos((*current).ang.alpha)*sin((*current).ang.beta)*sin((*current).ang.beta);
		matrices [order][3][1] = 0;
			
		//third column
		matrices [order][0][2] = sin((*current).ang.beta);
		matrices [order][1][2] = -cos((*current).ang.beta)*sin((*current).ang.alpha);
		matrices [order][2][2] = cos((*current).ang.alpha)*cos((*current).ang.beta);
		matrices [order][3][2] = 0;
			
		//fourth column
		matrices [order][0][3] = (*current).off.X;
		matrices [order][1][3] = (*current).off.Y;
		matrices [order][2][3] = (*current).off.Z;
		matrices [order][3][3] = 1;
		
		//printing the matrix
		printf ("\n");
		printf ("Transformation matrix for CS number %d\n",order);
		for (i = 0; i < 4; i++)
		{
        	for (j = 0; j < 4; j++) 
			{
            	printf("%f ", matrices [order][i][j]);
        	}
        	printf("\n");
    	}
    	printf ("\n");
		
		//check if there are no more CSs
		printf ("Done? [y or n]");
		scanf ("%s",q);
		order++;
		if ((strcmp ("y",q) == 0))
		{
			break;
		}
	}
	current->next_CS = (CS*) malloc (100*sizeof(CS));
	current = (*current).next_CS;
	if (current == NULL)
	{
		printf ("memory allocation failed");
		exit (2);
	}
	(*current).next_CS=NULL; 
		
	//intializing a temporary matrix with zeros
	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 4; ++j)
		{
			prod[i][j] = 0;
		}
	}
	

	for (i = 0; i < 4; i++)
	{
	    for (j = 0; j <4; j++) 
		{
	      	mul [i][j] = matrices[0][i][j];
	    }
	}

	order--;
	if (order > 0)
	{
	
		for (i = 0; i < 4; i++)
		{
	      	for (j = 0; j < 4; j++) 
			{
	           	prod [i][j] = 0;
	    	}
	       	printf("\n");
	    }
		for (op = 1;op<=order;op++)
		{		
			// Multiplying matrices firstMatrix and secondMatrix and storing in array mult.
			for(i = 0; i < 4; ++i)
			{
				for(j = 0; j < 4; ++j)
				{
					for(k=0; k<4; ++k)
					{
						prod[i][j] += mul[i][k] * matrices[op][k][j];
					}
				}
			}
		    	
			for (p = 0; p < 4; p++)
			{
			    for (l = 0; l <4; l++) 
				{
			      	mul [p][l] = prod[p][l];
			    }
			}
			for (i = 0; i < 4; i++)
			{
		        for (j = 0; j < 4; j++) 
				{
		           	prod [i][j] = 0;
		       	}
		   	}
		}
	}		
	
	//copying cordinates
    *X = mul [0][3];
    *Y = mul [1][3];
    *Z = mul [2][3];

	//freeing memry
	for (i=0;i<order;i++)
	{
		for (k=0;k<4;k++)
		{
			free (matrices[i][k]);
		}
		free (matrices[i]);
	}
	free (matrices);
	return 0;
}

//takes whether offset or rotation string as input
// returns 0 if the answer is no and 1 if the answer is yes
//keeps asking if input is incorrect
int ask_rotation_offset (char q [20])
{
	int t = 1;
	char R[20];
	
	while (t == 1)
	{
		printf ("are there any %s?[y or n]\n",q);
		scanf("%s",R);
		if ((strcmp ("y",R) == 0))
		{
			t = 0;
			return 1;
		}
		else if ((strcmp ("n",R) == 0))
		{
			t = 0;
			return 0;		
		}
		else
		{
			printf ("Wrong input\n");
		}
	}
	return 0;
}

//to check if the user's input is valid numerical value and keep asking untill it reads a valid input
float check_input ()
{
	char change [20];
	int condition = 0, i = 0, floating_points = 0, minus =0;
	int input_length;
	float len;
	int c = 0;
	do 
    {    	
		condition = 0;	
		floating_points = 0;
		minus = 0;
    	fgets (change,20,stdin);
		input_length = strlen (change);
		
		if (input_length > 0 && change[input_length - 1] == '\n'&& change[0] != '\n')
    	{
    		change[input_length - 1] = '\0';	
		}
		else 
		{
		    condition = 1;
		}
    	
    	// Checking if the user inputted a number
    	for (i=0;i<input_length-1;i++)
    	{
    		if (isdigit(change[i]) != 0)
    		{
    			// Doing and undoing the same thing
    			floating_points++;
    			floating_points--;
			}
			else if (change[i] == '.')
			{
				floating_points++;
			}
			else if (change[i] == '-')
			{
				minus++;
			}
			else 
			{
				condition = 1;
				break;
			}			
		}
		
		// Converting the user's input
		len = atof (change);
		c++;
	}while(condition == 1 || floating_points > 1 || change[input_length-1] != '\0' || minus > 1);
	return len;
}

