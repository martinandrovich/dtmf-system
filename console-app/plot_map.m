function [] = plot_map(filename)

    % Usage
    % plot_script("test.dat")
    
    % Print information
    disp('MATLAB Map Plot Script 1.0.0');

    % Load data
    data = load(filename);

    % Plot
    plot(data(:,1), data(:,2))

    % Labels
    xlabel('Element Number');
    ylabel('Magnitude');
    title('Map Plot');

end