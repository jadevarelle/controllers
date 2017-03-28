L = 0.001023;
R = 2.25;
J = .283654;  
B = 1.153;

H_gain = 1;
Kt = 157.4542; %from datasheet
Kb = 0.001928; %back emf constant - units = V/(rad/s))

%%%% transfer functions %%%%
elec_num = 1;
elec_den = [L R];

mech_num = 1;
mech_den = [J B];

elec_tf = tf(elec_num,elec_den);
mech_tf = tf(mech_num, mech_den);

G = elec_tf*mech_tf*Kt;
F = feedback(G,Kb)*H_gain;
[k,poles] = rlocfind(F);