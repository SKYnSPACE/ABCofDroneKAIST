classdef Drone < handle
    
%% MEMBERS    
    properties
        g
        t
        dt
        END_TIME
        
        mass
        L
        I
        
        X                                                                       %(x, y, z, dx, dy, dz, q1, q2, q3, q4, p, q, r)
        r                                                                       %(x, y, z)
        dr                                                                      %(dx, dy, dz)
        quat                                                                    %(q1, q2, q3, q4)
        w                                                                       %(p, q, r)
        
        phi
        psi
        theta
        
        dX
        
        input
        F
        M
    end
    
    properties
        Kp_phi
        Kd_phi
        Kp_theta
        Kd_theta
        Kp_psi
        Kd_psi
    end

%% METHODS
    methods
    %% CONSTRUCTOR: COMPLETED
        function obj = Drone(initStates, initInputs, params, tf)
            obj.g = 9.81;
            obj.t = 0.0;
            obj.dt = 0.01;
            obj.END_TIME = tf;
            
            obj.mass = params('mass');
            obj.L = params('armLength');
            obj.I = [params('Ixx'),0,0 ; 0,params('Iyy'),0; 0,0,params('Izz')];
            
            obj.X = initStates;
            obj.r = obj.X(1:3);
            obj.dr = obj.X(4:6);
            obj.quat = obj.X(7:10);
            [obj.phi, obj.psi, obj.theta] = Rot2RPY_ZXY(Quat2RM(obj.quat));
            obj.w = obj.X(11:13);
            
            obj.dX = zeros(13,1);
            
            obj.input = initInputs;
            obj.F = obj.input(1);
            obj.M = obj.input(2:4);
        end
    %% RETURNS DRONE STATE: COMPLETED 
        function state = GetState(obj)
            state = obj.X;
        end
        
    %% STATE SPACE (DIFFERENTIAL) EQUATIONS: INCOMPLETE!
        function obj = EvalEOM(obj)
            bRw = Quat2RM(obj.quat);                                       % World to Body Rotation Matrix
            wRb = bRw';                                                    % Body to World Rotation Matrix
            
            %%%%%%%%%%%%%%%%%%%%%%%%%%% QUIZ #1 %%%%%%%%%%%%%%%%%%%%%%%%%%%
            % Find dx/dt, dy/dt, dz/dt (velocity, obj.dX(1:3))
            obj.dX(1:3) = obj.dr;
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            
            %%%%%%%%%%%%%%%%%%%%%%%%%%% QUIZ #2 %%%%%%%%%%%%%%%%%%%%%%%%%%%
            % Find d^2x/dt^2, ... (acceleration, obj.dX(4:6))
            obj.dX(4:6) = 1 / obj.mass * (wRb * [0; 0; obj.F] - [0; 0; obj.mass * obj.g]);
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            
            % Angular velocity
            Kq = 2;
            qerr = 1 - (obj.quat(1)^2 + obj.quat(2)^2 + obj.quat(3)^2 + obj.quat(4)^2);
            obj.dX(7:10) = -1/2*[       0, -obj.w(1), -obj.w(2), -obj.w(3);...
                                 obj.w(1),         0, -obj.w(3),  obj.w(2);...
                                 obj.w(2),  obj.w(3),         0, -obj.w(1);...
                                 obj.w(3), -obj.w(2),  obj.w(1),         0] * obj.quat + Kq*qerr *obj.quat;

            % Angular acceleration
            obj.dX(11:13) = (obj.I) \ (obj.M - cross(obj.w, obj.I * obj.w));

        end

    %% PREDICT NEXT DRONE STATE: INCOMPLETE!
        function obj = UpdateState(obj)
            obj.t = obj.t + obj.dt;
            
            %%%%%%%%%%%%%%%%%%%%%%%%%%% QUIZ #3 %%%%%%%%%%%%%%%%%%%%%%%%%%%
            % Find(update) the next state of obj.X
            obj.EvalEOM();
            obj.X = obj.X + obj.dX.*obj.dt;
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            
            obj.r = obj.X(1:3);
            obj.dr = obj.X(4:6);
            obj.quat = obj.X(7:10);
            [obj.phi, obj.psi, obj.theta] = Rot2RPY_ZXY(Quat2RM(obj.quat));
            obj.w = obj.X(11:13);

        end
        
    %% CONTROLLER: NOT TODAY! FOR SESSION #3
        function obj = UpdateController(obj)
%             obj.F = obj.mass * obj.g;
%             obj.M(1,1) = 0.001; % Roll Moment
%             obj.M(2,1) = 0.001; % Pitch Moment
            obj.M(3,1) = 0.01; % Yaw Moment
        end
    end
end

