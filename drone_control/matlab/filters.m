orthT=0.01;
f0=5; %Frecuencia de corte
w0=2*pi*f0;
Fs=tf(1,[1/w0 1]); %Filtro anal�gico
bode(Fs);
Fz=c2d(Fs,T) %Filtro digital