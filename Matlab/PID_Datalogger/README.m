%% README.m for Plot_Serial
%
% Plot_Serial.m and Plot_Serial.fig are used to define the Graphical User
% Interface (GUI, or UI) for the incubator.
%
%% Plot_Serial.m Functions
% * *varargout = Plot_Serial(varargin)* - Creates the UI object, not much
% to edit here
% 
% * *Plot_Serial_OpeningFcn(hObject, eventdata, handles, varargin)* -
% Executes just before the UI is made visible. Setup information goes here.
% Global buffers are created here to pass data between the other methods of
% the UI. Globals are used in this context because UI operations are 
% conducted through callbacks (functions that are passed as a parameter to 
% other functions) and variables cannot be passed directly with callbacks 
% in Matlab. See:
% <https://www.mathworks.com/help/matlab/creating_guis/share-data-among-callbacks.html#bt9p4xi
% help document> for details. *This will be fixed to not use globals but to
% use guidata structure to pass data between callbacks in the UI*
%
% * *varargout = Plot_Serial_OutputFcn(hObject, eventdata, handles)* - 
% Outputs from this function are returned to the command line. Currently
% Unused.
%
% * *pushbuttonStop_Callback(hObject, eventdata, handles)* - Callback 
% action that occurs when the pushbuttonStop is pressed, which is the Stop 
% button. 
%
% * *axesTemp_CreateFcn(hObject, eventdata, handles)* - Creates the axes
% used to plot the Temperature data.