function I = GaussQuad4(Fun)
    % over points -1, -1/3, 1/3, 1
    W0 = 0.3478548;
    W1 = 0.6521452;
    W2 = 0.6521452;
    W3 = 0.3478548;
    x1 = -0.86113631;
    x2 = -0.33998104;
    x3 = 0.33998104;
    x4 = 0.86113631;

    I = W0*Fun(x1) + W1*Fun(x2) + W2*Fun(x3) + W3*Fun(x4);
end

