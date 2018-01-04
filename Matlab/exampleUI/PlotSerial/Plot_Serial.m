
function varargout = Plot_Serial(varargin)
    % PLOT_SERIAL MATLAB code for Plot_Serial.fig
    %      PLOT_SERIAL, by itself, creates a new PLOT_SERIAL or raises the existing
    %      singleton*.
    %
    %      H = PLOT_SERIAL returns the handle to a new PLOT_SERIAL or the handle to
    %      the existing singleton*.
    %
    %      PLOT_SERIAL('CALLBACK',hObject,eventData,handles,...) calls the local
    %      function named CALLBACK in PLOT_SERIAL.M with the given input arguments.
    %
    %      PLOT_SERIAL('Property','Value',...) creates a new PLOT_SERIAL or raises the
    %      existing singleton*.  Starting from the left, property value pairs are
    %      applied to the GUI before Plot_Serial_OpeningFcn gets called.  An
    %      unrecognized property name or invalid value makes property application
    %      stop.  All inputs are passed to Plot_Serial_OpeningFcn via varargin.
    %
    %      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
    %      instance to run (singleton)".
    %
    % See also: GUIDE, GUIDATA, GUIHANDLES

    % Edit the above text to modify the response to help Plot_Serial

    % Last Modified by GUIDE v2.5 19-Dec-2017 07:32:21
    
    % TODO - add PI&D test fields to change those values programmatically
    % on the Arduino. This will be done using json. This will also require
    % changing the existing serial output to set the temperature setpoint
    % using json.

    % Begin initialization code - DO NOT EDIT
    gui_Singleton = 1;
    gui_State = struct('gui_Name',       mfilename, ...
                       'gui_Singleton',  gui_Singleton, ...
                       'gui_OpeningFcn', @Plot_Serial_OpeningFcn, ...
                       'gui_OutputFcn',  @Plot_Serial_OutputFcn, ...
                       'gui_LayoutFcn',  [] , ...
                       'gui_Callback',   []);
    if nargin && ischar(varargin{1})
        gui_State.gui_Callback = str2func(varargin{1});
    end

    if nargout
        [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
    else
        gui_mainfcn(gui_State, varargin{:});
    end
end
% End initialization code - DO NOT EDIT


% --- Executes just before Plot_Serial is made visible.
function Plot_Serial_OpeningFcn(hObject, eventdata, handles, varargin) %#ok<*INUSL>
    % This function has no output args, see OutputFcn.
    % hObject    handle to figure
    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    structure with handles and user data (see GUIDATA)
    % varargin   command line arguments to Plot_Serial (see VARARGIN)

    % Reset the com ports in case the Arduino was not properly disconnected
    % before the program quit previously.
    instrreset

    global temperature_buffer
    temperature_buffer = zeros(100,1);
    global temperatureOutside_buffer
    temperatureOutside_buffer = zeros(size(temperature_buffer));
    global setpoint_buffer
    setpoint_buffer = zeros(size(temperature_buffer));
    global output_buffer
    output_buffer = zeros(size(temperature_buffer));
    
    
    % Choose default command line output for Plot_Serial
    handles.output = hObject;
    
    %handles.temperature_buffer = zeros(10,1);
    %handles.setpoint_buffer = zeros(size(handles.temperature_buffer));
    
    % Update handles structure
    guidata(hObject, handles);

    % UIWAIT makes Plot_Serial wait for user response (see UIRESUME)
    % uiwait(handles.figure1);
    
    % Make plots for the temperatures and PID values
    x_data = 1:1:length(temperature_buffer);
    handles.hTempPlot = plot(handles.axesTemp, x_data, temperature_buffer);
    hold on
    handles.hTempOutPlot = plot(handles.axesTemp, x_data, temperatureOutside_buffer);
    handles.hSPPlot = plot(handles.axesTemp, x_data, setpoint_buffer);
    hold off
    set(handles.axesTemp, 'YLim', [15,45]);
    title('Temperature Celsius');
    legend('Temp Inside', 'Temp Outside', 'Setpoint', 'Location', 'southwest');
    
    handles.hPIDPlot = plot(handles.axesPID, x_data, output_buffer);
    set(handles.axesPID, 'YLim', [0,260]);
    title('PID Output Signal 8-bit');
    guidata(hObject, handles);
    
    
    comPort = choose_usb_dialog;
    serialObj = setup_serial(comPort, hObject, handles);
    handles.comPort = comPort;
    handles.serialObj = serialObj;
    guidata(hObject, handles);
    
end


% --- Outputs from this function are returned to the command line.
function varargout = Plot_Serial_OutputFcn(hObject, eventdata, handles) 
    % varargout  cell array for returning output args (see VARARGOUT);
    % hObject    handle to figure
    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    structure with handles and user data (see GUIDATA)

    % Get default command line output from handles structure
    varargout{1} = handles.output;
end


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles) %#ok<DEFNU>
    % hObject    handle to pushbutton1 (see GCBO)
    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    structure with handles and user data (see GUIDATA)
    fclose(handles.serialObj);
    close(handles.figure1);
end

% --- Executes during object creation, after setting all properties.
function axesTemp_CreateFcn(hObject, eventdata, handles) %#ok<DEFNU,*INUSD>
% hObject    handle to axesTemp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axesTemp


end



%%
function usbChoice = choose_usb_dialog()

    % Get the list of available ports connected to the computer
    ports = cellstr(seriallist);
    
    % Prepend 'none' selection to ports cell array to allow for running
    %  the UI without a connected Arduino, FUTURE FEATURE
    ports = [{'none'},ports];
    
    d = dialog('Position',[300 300 250 150],'Name','Select Port');

    txt = uicontrol('Parent',d,...
               'Style','text',...
               'Position',[20 80 210 40],...
               'String','Select Arduino Port.'); %#ok<NASGU>
           
    popup = uicontrol('Parent',d,...
           'Style','popup',...
           'Position',[75 70 100 25],...
           'String',ports,...
           'Callback',@popup_callback); %#ok<NASGU>

    btn = uicontrol('Parent',d,...
               'Position',[85 20 70 25],...
               'String','Close',...
               'Callback','delete(gcf)'); %#ok<NASGU>
           
    % Make default selection the first cell array element
    usbChoice = ports{1};
       
    % Wait for d to close before running to completion
    uiwait(d);
   
   function popup_callback(popup,event)
      idx = popup.Value;
      popup_items = popup.String;
      % This code uses dot notation to get properties.
      % Dot notation runs in R2014b and later.
      % For R2014a and earlier:
      % idx = get(popup,'Value');
      % popup_items = get(popup,'String');
      usbChoice = char(popup_items(idx,:));
   end
end


%%
function[obj,flag] = setup_serial(comPort, hObject, handles)
  % It accept as the entry value, the index of the serial port
  % Arduino is connected to, and as output values it returns the serial 
  % element obj and a flag value used to check if when the script is compiled
  % the serial element exists yet.
  flag = 1;
  % Initialize Serial object
  obj = serial(comPort);
  set(obj,'DataBits',8);
  set(obj,'StopBits',1);
  set(obj,'BaudRate',9600);
  set(obj,'Parity','none');
  
  
  % Set serial to read the data when terminator is reached
  %obj.BytesAvailableFcnCount = 128;
  obj.BytesAvailableFcnMode = 'terminator';
  %obj.BytesAvailableFcn = {@(h,event) instrcallback(obj,h,event), handles};
  obj.BytesAvailableFcn = {@instrcallback, hObject, handles};
  %obj.BytesAvailableFcn = @instrcallback;
  
  fopen(obj);
  flushinput(obj);
  %fread(obj, obj.BytesAvailable)
  %mbox = msgbox('Serial Communication setup'); uiwait(mbox);
end

function instrcallback(serialObj, event, hObject, handles)
    global temperature_buffer;
    global temperatureOutside_buffer;
    global setpoint_buffer;
    global output_buffer;
    %byteNum = serialObj.BytesAvailable;
    a = fscanf(serialObj,'%s\n');
    % Check that the json data is intact by checking that there is a { at
    % the beginning and a } at the end of the string.
    if length(regexp(a,'^{\w*|\W*}$')) == 2
        data = jsondecode(a);
        % Gather data to update axes
        temperature_buffer= circshift(temperature_buffer,1);
        temperature_buffer(1) = data.temperature;
        temperatureOutside_buffer= circshift(temperatureOutside_buffer,1);
        temperatureOutside_buffer(1) = data.temperature_outside;
        setpoint_buffer = circshift(setpoint_buffer,1);
        setpoint_buffer(1) = data.setpoint;
        output_buffer = circshift(output_buffer,1);
        output_buffer(1) = data.output;
        % Update Plots
        handles.hTempPlot.YData = temperature_buffer;
        handles.hTempOutPlot.YData = temperatureOutside_buffer;
        handles.hSPPlot.YData = setpoint_buffer;
        handles.hPIDPlot.YData = output_buffer;
        drawnow()
    end
end



function editSerialOut_Callback(hObject, eventdata, handles) %#ok<DEFNU>
    % hObject    handle to editSerialOut (see GCBO)
    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    structure with handles and user data (see GUIDATA)

    % Hints: get(hObject,'String') returns contents of editSerialOut as text
    %        str2double(get(hObject,'String')) returns contents of editSerialOut as a double

    % Get the datat from the text field and send it by serial to the attached
    % Arduino
    serialData = get(hObject,'String');
    
    fprintf(handles.serialObj,serialData); 
end

% --- Executes during object creation, after setting all properties.
function editSerialOut_CreateFcn(hObject, eventdata, handles) %#ok<DEFNU>
    % hObject    handle to editSerialOut (see GCBO)
    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    empty - handles not created until after all CreateFcns called

    % Hint: edit controls usually have a white background on Windows.
    %       See ISPC and COMPUTER.
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

end
