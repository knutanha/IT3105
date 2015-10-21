clear
clc
a = importdata('tsp_38.log');
b = importdata('dj38.tspm');

x = a.data(:,3);
y = a.data(:,4);
it = a.data(:,1);

cityx = b.data(:,1);
cityy = b.data(:,2);

figure

for i=unique(it)';
    hold off
    plot(cityx, cityy,'*');
    axis([0 1 0 1]);
    hold on;
    tempx = x(it==i);
    tempy = y(it==i);
    plot(tempx,tempy,'ro-');
    plot([tempx(end) tempx(1)], [tempy(end) tempy(1)],'ro-');
    fprintf(1,'Iteration %i\n',i);
    pause();
end
    