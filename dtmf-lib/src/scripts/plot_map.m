function [] = plot_map(filename, name, x, y)

    % Usage
    % plot_script("test.dat")
    
    % Set figure title
    figure('Name', name, 'NumberTitle', 'off');
    
    % Print information
    disp('MATLAB Map Plot Script 1.1.0');

    % Load data
    data = load(filename);

    % Plot
    plot(data(:,1), data(:,2))
    
   % Center Y-Axis
    ymax = max(abs(data(:, 2)));
    ymax = ymax + 0.1*ymax;
    ylim([-ymax, ymax]);
    
    % Labels
    title(name);
    xlabel(x);
    ylabel(y);

end