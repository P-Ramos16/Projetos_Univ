clear all;
nVector = [10:1:50 55:5:100 110:10:200 220:20:800];
resultVector = [1.133e-06
                2.428e-06
                8.700e-07
                1.075e-06
                8.410e-07
                9.260e-07
                7.840e-07
                9.160e-07
                9.040e-07
                1.095e-06
                1.002e-06
                1.141e-06
                6.970e-07
                6.630e-07
                6.330e-07
                4.500e-07
                5.270e-07
                5.240e-07
                5.390e-07
                4.690e-07
                4.050e-07
                3.820e-07
                4.150e-07
                3.870e-07
                4.310e-07
                4.360e-07
                4.240e-07
                4.330e-07
                4.320e-07
                4.330e-07
                4.860e-07
                4.310e-07
                4.280e-07
                4.970e-07
                4.620e-07
                4.480e-07
                4.980e-07
                5.600e-07
                5.270e-07
                4.820e-07
                5.140e-07
                3.409e-06
                1.430e-06
                7.290e-07
                8.380e-07
                9.390e-07
                1.172e-06
                1.436e-06
                1.126e-06
                1.092e-06
                1.108e-06
                5.777e-06
                1.365e-06
                1.421e-06
                1.412e-06
                1.669e-06
                1.501e-06
                1.380e-06
                1.532e-06
                1.730e-06
                1.846e-06
                5.163e-06
                2.171e-06
                2.029e-06
                2.208e-06
                2.276e-06
                2.421e-06
                7.388e-06
                2.651e-06
                3.050e-06
                3.022e-06
                6.317e-06
                3.490e-06
                3.329e-06
                3.461e-06
                3.470e-06
                3.616e-06
                4.232e-06
                5.673e-06
                4.182e-06
                4.482e-06
                5.219e-06
                3.976e-06
                4.135e-06
                4.240e-06
                4.358e-06
                4.551e-06
                4.637e-06
                5.681e-06
                5.026e-06
                5.024e-06]';

figure(1);
hold on;
plot(nVector, resultVector, "+r");
plot(nVector, resultVector, "b");
legend("Solução recursíva final");
axis([0 800 0 10e-06]);
xlabel("n");
ylabel("CPU Time (segundos)");
grid on;
hold off;