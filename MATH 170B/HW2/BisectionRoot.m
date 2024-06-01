function Xs = BisectionRoot(Fun,a,b)
    if Fun(a)*Fun(b) >= 0
        error("Does not bracket root")
    end
    Xs = (b+a)/2;
    while (b-a)/2 >= 0.000001 
        if Fun(a)*Fun(Xs) < 0
            b = Xs;
        elseif Fun(Xs)*Fun(b) < 0
            a = Xs;
        else
            return;
        end
        Xs = (b+a)/2;
    end
end

