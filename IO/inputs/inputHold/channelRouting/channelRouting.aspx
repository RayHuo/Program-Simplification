
% simplification
connectTop(N,X) :- connect(N,top,X).
connectBot(N,X) :- connect(N,bot,X).

% les + petits et + grands numéros de terminaux pour chaque réseau
nminX(N,X2) :- connect(N,L1,X1),connect(N,L2,X2), X1<X2.
minX(N,X1) :- connect(N,L1,X1), not nminX(N,X1).

nmaxX(N,X2) :- connect(N,L1,X1),connect(N,L2,X2), X1>X2.
maxX(N,X1) :- connect(N,L1,X1), not nmaxX(N,X1).

% les networks
net(X) :- minX(X,N).

% on repère les nets qui ont une intersection non vide sur un même track
inter(N1,N2) :- minX(N1,Xmin1), maxX(N1,Xmax1), connect(N2,L,X), N2 != N1, Xmin1 <= X, X <= Xmax1. 
inter(N2,N1) :- inter(N1,N2).


% les layers possibles
layer(1..N) :- layers(N).

% les tracks possibles
track(1..N) :- tracks(N).

% placer ou ne pas placer un réseau sur un track d'un niveau
pos(N,L,T) :- net(N), layer(L), track(T), not nopos(N,L,T).
%pos(N,L,T) :- net(N), track(T), layer(L), not nopos(N,L,T).
%nopos(N,L1,T1) :- pos(N,L,T), layer(L1), track(T1), t(L,T) != t(L1,T1).

oknet(N) :- pos(N,L,T).
:- net(N), not oknet(N). 

% propagation des choix
nopos(N2,L,T2) :- pos(N1,L,T1), connectTop(N1,X), connectBot(N2,X), N1!=N2, track(T2), T2<=T1.
nopos(N2,L,T2) :- pos(N1,L,T1), connectBot(N1,X), connectTop(N2,X), N1!=N2, track(T2), T2>=T1.

nopos(N2,L,T) :- pos(N1,L,T), inter(N1,N2).


#hide.
% #show pos/3.
