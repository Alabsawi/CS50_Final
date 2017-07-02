clear
%declaring symbolic variables as there values are not known
syms phi1 phi2

%declaring 4 matrices that are going to continue symbolic variables
%each of size 4*4
T01 = sym('T01',[4 4]);
T12 = sym('T12',[4 4]);
T23 = sym('T23',[4 4]);


phi1 = 0;
phi2 = 0;

%intializing the declared matrices
%matrices are inntialized by inputing the first row then ";"
T01 = [cos(phi1) -sin(phi1) 0 0;sin(phi1) cos(phi1) 0 0;0 0 1 124.5; 0 0 0 1];
T12 = [-1 0 0 0;0 sin(phi2) cos(phi2) 150;0 cos(phi2) -sin(phi2) 362; 0 0 0 1];
T23 = [1 0 0 0; 0 1 0 700; 0 0 1 0;0 0 0 1];

%intializing final matrix and displaying its it to the command window
T = T01*T12*T23;
%disp (T);

%intializes a variable called X with the value in matrix T for element in
%the fourth column and first row 
X = T(1,4);

%intializes a variable called Y with the value in matrix T for element in
%the fourth column and second row
Y = T(2,4);

%intializes a variable called Z with the value in matrix T for element in
%the fourth column and third row
Z = T(3,4);

%display the values of X,Y, and Z variables
disp(X);
disp(Y);
disp(Z);
