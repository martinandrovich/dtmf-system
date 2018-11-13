function [] = plot_samples(filename, name, x, y)

    % Usage
    % plot_script("test.dat", "title", "x", "y")
    
    % Set figure title
    figure('Name', name, 'NumberTitle', 'off');
    
    % Print information
    disp('MATLAB Samples Plot Script 1.1.0');

    % Load data
    data = load(filename);

    % Plot
    plot(data);
    
    % Center Y-Axis
    ymax = max(abs(data(:)));
    ymax = ymax + 0.1*ymax;
    ylim([-ymax, ymax]);

    % Labels
    title(name);
    xlabel(x);
    ylabel(y);

end