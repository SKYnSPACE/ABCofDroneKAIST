clc;
clear all;
close all;

addpath('./lib');

%% INIT. PARAMS.
drone1_params = containers.Map({'mass','armLength','Ixx','Iyy','Izz'},...
    {0.18, 0.086, 0.00025, 0.000232, 0.0003738});
drone1_initStates = [0, 0, 10, ...                                              % x, y, z
    0, 0, 0, ...                                                                % dx, dy, dz
    1, 0, 0, 0, ...                                                             % q1, q2, q3, q4
    0, 0, 0]';                                                                  % p, q, r
drone1_initInputs = [0, ...                                                     % ThrottleCMD
    0, 0, 0]';                                                                  % R, P, Y CMD

drone1_body = [ 0.1,    0,   0, 1; ...
                  0,  0.1,   0, 1; ...
               -0.1,    0,   0, 1; ...
                  0, -0.1,   0, 1; ...
                  0,    0,   0, 1; ...
                  0,    0, 0.15, 1]';
    
simulationTime = 2.0;                                                          % [sec]

%% BIRTH OF A DRONE
drone1 = Drone(drone1_initStates, drone1_initInputs, drone1_params, simulationTime);

%% INIT. FIG.
fig1 = figure('pos',[10 10 600 900]);
h = gca;
view(3);
axis equal;
grid on;
xlim([-1 1]);
ylim([-1 1]);
zlim([0 10]);
xlabel('X[m]');
ylabel('Y[m]');
zlabel('Height[m]');


hold(gca, 'on');
drone1_state = drone1.GetState();
wHb = [Quat2RM(drone1_state(7:10)) drone1_state(1:3); 0 0 0 1];
drone1_world = wHb * drone1_body;
drone1_atti = drone1_world(1:3, :);
    
fig1_ARM13 = plot3(gca, drone1_atti(1,[1 3]), drone1_atti(2,[1 3]), drone1_atti(3,[1 3]), ...
        '-ro', 'MarkerSize', 5);
fig1_ARM24 = plot3(gca, drone1_atti(1,[2 4]), drone1_atti(2,[2 4]), drone1_atti(3,[2 4]), ...
        '-bo', 'MarkerSize', 5);
fig1_payload = plot3(gca, drone1_atti(1,[5 6]), drone1_atti(2,[5 6]), drone1_atti(3,[5 6]), ...
        '-k', 'Linewidth', 2);  

hold(gca, 'off');

%% MAIN

for i = 1:simulationTime/0.01
    %%
    drone1_state = drone1.GetState();
    drone1.UpdateController(); %NOT TODAY! FOR SESSION #3. Uncomment to have some fun with the drone:)
    drone1.UpdateState();

    
    %%
    if (drone1_state(3) <= 0)
        h = msgbox('Crashed!!!', 'Error','error');
        break;
    end
    
    %% PLOTTING
    wHb = [Quat2RM(drone1_state(7:10)) drone1_state(1:3); 0 0 0 1];
    drone1_world = wHb * drone1_body;
    drone1_atti = drone1_world(1:3, :);
    
	set(fig1_ARM13, ...
        'XData', drone1_atti(1,[1 3]), ...
        'YData', drone1_atti(2,[1 3]), ...
        'ZData', drone1_atti(3,[1 3]));
    set(fig1_ARM24, ...
        'XData', drone1_atti(1,[2 4]), ...
        'YData', drone1_atti(2,[2 4]), ...
        'ZData', drone1_atti(3,[2 4]));
    set(fig1_payload, ...
        'XData', drone1_atti(1,[5 6]), ...
        'YData', drone1_atti(2,[5 6]), ...
        'ZData', drone1_atti(3,[5 6]));  
    drawnow;
    
end