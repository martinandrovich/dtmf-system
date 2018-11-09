function [] = plot_script(filename)

    % Usage
    % plot_script("test.dat")
    
    % Print information
    disp('MATLAB Plot Script 1.0.0');

    % Load data
    data = load(filename);

    % Plot
    plot(data);

    % Labels
    xlabel('Sample Number');
    ylabel('Amplitude');
    title('Samples Plot');

end